/*
  Copyright (c) 2012-2013, William F. Smith
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

#define CAST_AS_SOCKADDR(storage) \
  reinterpret_cast<struct sockaddr*>( \
    const_cast<struct sockaddr_storage*>(storage)) \

#define CAST_AS_SOCKADDR_IN(addr) \
  reinterpret_cast<struct sockaddr_in*>( \
    const_cast<struct sockaddr*>(addr)) \

#define CAST_AS_SOCKADDR_IN6(addr) \
  reinterpret_cast<struct sockaddr_in6*>( \
    const_cast<struct sockaddr*>(addr)) \

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
#elif _WIN32
    return closesocket(sockfd);
#endif
}

/*!
 *
 */
static std::string GetSocketAddressString(const struct sockaddr& addr) {
    std::string address(INET6_ADDRSTRLEN, '\0');

    if (addr.sa_family == AF_INET6) {
        auto sock_addr = CAST_AS_SOCKADDR_IN6(&addr)->sin6_addr;
        char* data = reinterpret_cast<char*>(
            const_cast<char*>(address.data()));

        inet_ntop(addr.sa_family, &sock_addr, data, INET6_ADDRSTRLEN);
    } else {
        auto sock_addr = CAST_AS_SOCKADDR_IN(&addr)->sin_addr;
        char* data = reinterpret_cast<char*>(
            const_cast<char*>(address.data()));

        inet_ntop(addr.sa_family, &sock_addr, data, INET_ADDRSTRLEN);
    }
    return address.data();
}

static std::string GetSocketAddressString(
    const struct sockaddr_storage& storage) {
  return GetSocketAddressString(*CAST_AS_SOCKADDR(&storage));
}

static int GetSocketPort(const struct sockaddr& addr) {
  int port;

  if (addr.sa_family == AF_INET6) {
    auto sock_addr = CAST_AS_SOCKADDR_IN6(&addr);
    port = ntohs(sock_addr->sin6_port);
  } else {
    auto sock_addr = CAST_AS_SOCKADDR_IN(&addr);
    port = ntohs(sock_addr->sin_port);
  }
  return port;
}

static int GetSocketPort(const struct sockaddr_storage& storage) {
  return GetSocketPort(*CAST_AS_SOCKADDR(&storage));
}


namespace engine {
namespace network {

/*!
  *
  */
SocketAddress::SocketAddress(const SocketAddressData& addr_data)
    : impl_(std::make_unique<SocketAddressImpl>(addr_data)) {
}

/*!
  *
  */
SocketAddress::SocketAddress(const SocketAddress& copy)
    : impl_(std::make_unique<SocketAddressImpl>(copy.impl_->data_)) {
}

/*!
  *
  */
SocketAddress::~SocketAddress() {}

/*!
  *
  */
std::string SocketAddress::GetText() const {
  return GetSocketAddressString(impl_->data_.address);
}

/*!
  *
  */
int SocketAddress::GetLength() const {
  return impl_->data_.length;
}

/*!
  *
  */
bool SocketAddress::operator==(const SocketAddress& compare) const {
  auto storage = impl_->data_.address;
  auto storage_cmp = compare.impl_->data_.address;

  if (storage.ss_family == storage_cmp.ss_family) {
    if (storage.ss_family == AF_INET6 && 
        memcmp(&CAST_AS_SOCKADDR_IN6(CAST_AS_SOCKADDR(&storage))->sin6_addr,
        &CAST_AS_SOCKADDR_IN6(CAST_AS_SOCKADDR(&storage_cmp))->sin6_addr,
        sizeof(struct sockaddr_in6)) == 0) {
      return true;
    } else if (memcmp(
        &CAST_AS_SOCKADDR_IN(CAST_AS_SOCKADDR(&storage))->sin_addr,
        &CAST_AS_SOCKADDR_IN(CAST_AS_SOCKADDR(&storage_cmp))->sin_addr,
        sizeof(struct sockaddr_in)) == 0) {
      return true;
    }
  }
  return false;
}

/*!
  *
  */
SocketAddressImpl::SocketAddressImpl(const SocketAddressData& addr_data)
    : data_(addr_data) {
}

/*!
  *
  */
const void* SocketAddress::GetRaw() const {
  return &impl_->data_.address;
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

#ifdef _WIN32
  static std::atomic<bool> once;
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
  } else {
    new_node_name = INADDR_ANY;
  }

  if (getaddrinfo(new_node_name, service_name.c_str(), &hints,
                  &address_info_) != 0) {
    throw std::runtime_error("Unable to get address information.");
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

  if (socket_ == -1) throw std::runtime_error("Unable to create socket.");

#ifdef __GNUC__
  if (!blocking_) {
    fcntl(socket_, F_SETFL, O_NONBLOCK);
  }
#elif _WIN32
  if (!blocking_) {
    u_long non_blocking = 1;
    ioctlsocket(socket_, FIONBIO, &non_blocking);
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
std::string SocketBase::GetAddressText() {
  if (!impl_->current_address_info_)
    return stdext::string::empty();

  return GetSocketAddressString(*impl_->current_address_info_->ai_addr);
}

int SocketBase::GetPort() {
  if (!impl_->current_address_info_)
    return -1;

  return GetSocketPort(*impl_->current_address_info_->ai_addr);
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
