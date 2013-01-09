/*
  Copyright (c) 2012, William F. Smith
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation 
  and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
  THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "socket_base.h"
#include "socket_base-impl.h"

#define CAST_IN_ADDR(storage) \
  &reinterpret_cast<struct sockaddr_in*>( \
    const_cast<struct sockaddr_storage*>(&storage))->sin_addr \

#define CAST_IN6_ADDR(storage) \
  &reinterpret_cast<struct sockaddr_in6*>( \
    const_cast<struct sockaddr_storage*>(&storage))->sin6_addr \

/*!
 *
 */
static int CreateSocket(int domain, int type, int protocol) {
  return static_cast<int>(socket(domain, type, protocol));
}

/*!
 *
 */
static int BindSocket(int sockfd, const struct sockaddr& addr, int addrlen) {
  return bind(sockfd, &addr, addrlen);
}

/*!
 *
 */
static int CloseSocket(int sockfd) {
#ifdef __GNUC__
    return close(sockfd);
#elif _MSC_VER
    return closesocket(sockfd);
#endif
}

/*!
 *
 */
static std::string GetSocketAddress(const struct sockaddr_storage& addr) {
    std::string address(INET6_ADDRSTRLEN, '\0');

    switch (addr.ss_family) {
      case AF_INET6: {
        auto sock_addr = CAST_IN6_ADDR(addr);
        char* data = reinterpret_cast<char*>(
            const_cast<char*>(address.data()));

        inet_ntop(addr.ss_family, sock_addr, data, INET6_ADDRSTRLEN);
        break;
      }
      default: {
        auto sock_addr = CAST_IN_ADDR(addr);
        char* data = reinterpret_cast<char*>(
            const_cast<char*>(address.data()));

        inet_ntop(addr.ss_family, sock_addr, data, INET_ADDRSTRLEN);
        break;
      }
    }
    return address.data();
}

namespace engine {
namespace network {

/*!
  *
  */
SocketAddress::SocketAddress(std::shared_ptr<SocketAddressData> addr_data)
    : impl_(std::make_unique<SocketAddressImpl>(addr_data)) {
}

/*!
  *
  */
SocketAddress::~SocketAddress() {
}

/*!
  *
  */
std::string SocketAddress::GetText() const {
  return GetSocketAddress(impl_->data_->address);
}

/*!
  *
  */
int SocketAddress::GetLength() const {
  return impl_->data_->length;
}

/*!
  *
  */
bool SocketAddress::operator==(const SocketAddress& compare) const {
  auto addr = impl_->data_->address;
  auto addr_cmp = compare.impl_->data_->address;

  if (addr.ss_family == addr_cmp.ss_family) {
    if (addr.ss_family == AF_INET6 && 
        memcmp(CAST_IN6_ADDR(addr), CAST_IN6_ADDR(addr_cmp),
               sizeof(struct sockaddr_in6)) == 0) {
      return true;
    } else if (memcmp(CAST_IN_ADDR(addr), CAST_IN_ADDR(addr_cmp),
               sizeof(struct sockaddr_in)) == 0) {
      return true;
    }
  }
  return false;
}

/*!
  *
  */
SocketAddressImpl::SocketAddressImpl(
    std::shared_ptr<SocketAddressData> addr_data) : data_(addr_data) {
}

/*!
  *
  */
SocketAddressImpl::~SocketAddressImpl() {
}

/*!
  *
  */
const void* SocketAddress::GetRaw() const {
  return &impl_->data_->address;
}

/*!
  *
  */
void SocketBaseImpl::Open(int socket_type, int flags,
                          const std::string& node_name,
                          const std::string& service_name) {
  int family;
  switch(family_) {
    case SocketFamily::kUnspecified:
      family = AF_UNSPEC;
      break;
    case SocketFamily::kIpv4:
      family = AF_INET;
      break;
    case SocketFamily::kIpv6:
      family = AF_INET6;
      break;
    default:
      throw std::logic_error("Invalid socket type.");
  }

#ifdef _MSC_VER
  static std::atomic_bool once;
  if (!once) {
    once = true;
    WSADATA wsa_data;
    // Initialize Winsock
    int wsa_result = WSAStartup(MAKEWORD(2,2), &wsa_data);
    if (wsa_result != 0) {
      throw std::runtime_error("Unable to initialize Winsock.");
    }
  }
#endif

  struct addrinfo hints, *p;
  const char* new_node_name = nullptr;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = family;
  hints.ai_socktype = socket_type;
  hints.ai_flags = flags;

  // If the node name is empty, only pass 0 to getaddrinfo.
  if (!node_name.empty()) {
    new_node_name = static_cast<const char*>(node_name.c_str());
  }

  if (getaddrinfo(new_node_name, service_name.c_str(), &hints,
                  &address_info_) != 0) {
    throw std::domain_error("Unable to get address information.");
  }

  for (p = address_info_; p != 0; p = p->ai_next) {
    // If the family is unspecified, get the first one in the linked list.
    if (p->ai_family == family || family == AF_UNSPEC) {
      socket_ = CreateSocket(p->ai_family, p->ai_socktype, p->ai_protocol);
      if (socket_ != -1) {
        current_address_info_ = p;
        break;
      }
    }
  }

  if (socket_ == -1) throw std::domain_error("Unable to create socket.");

#ifdef _MSC_VER
  if (!blocking_) {
    u_long non_blocking = 1;
    ioctlsocket(socket_, FIONBIO, &non_blocking);
  }
#elif __GNUC__
  if (!blocking_) {
    fcntl(socket_, F_SETFL, O_NONBLOCK);
  }
#endif
}

/*!
  *
  */
SocketBase::SocketBase() : impl_(std::make_unique<SocketBaseImpl>()) {
  impl_->socket_ = -1;
  impl_->address_info_ = nullptr;
  impl_->current_address_info_ = nullptr;
  impl_->family_ = SocketFamily::kUnspecified;
  impl_->blocking_ = true;
}

/*!
  *
  */
SocketBase::~SocketBase() {
  Close();
}

/*!
  *
  */
int SocketBase::Bind(uint16_t port) {
  if (impl_->socket_ == -1 || !impl_->current_address_info_)
    return -1;

  // TODO: Currently not handling ipv6.
  auto sockaddr = reinterpret_cast<struct sockaddr_in*>(
      impl_->current_address_info_->ai_addr);
  sockaddr->sin_port = ntohs(port);
  int addr_len = static_cast<int>(impl_->current_address_info_->ai_addrlen);

  if (BindSocket(impl_->socket_, *impl_->current_address_info_->ai_addr,
                 addr_len) != 0) {
    return -1;
  }
  return 0;
}

/*!
  *
  */
std::string SocketBase::GetAddress() {
  if (!impl_->current_address_info_)
    return lib::EmptyString();

  return GetSocketAddress(*reinterpret_cast<struct sockaddr_storage*>(
                          impl_->current_address_info_->ai_addr));
}

/*!
  *
  */
void SocketBase::Close() {
  if (impl_->address_info_) {
    freeaddrinfo(impl_->address_info_);
    impl_->address_info_ = nullptr;
  }

  if (impl_->socket_ != -1) {
    CloseSocket(impl_->socket_);
    impl_->socket_ = -1;
  }
}

/*!
  *
  */
SocketFamily SocketBase::family() {
  return impl_->family_;
}

/*!
  *
  */
bool SocketBase::blocking() {
  return impl_->blocking_;
}

} // end network namespace
} // end engine namespace
