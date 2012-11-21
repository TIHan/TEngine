/*
  Copyright (c) 2012, William F. Smith
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

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

/*!
 *
 */
static int CreateSocket(const int& domain,
                        const int& type,
                        const int& protocol) {
  return static_cast<int>(socket(domain, type, protocol));
}

/*!
 *
 */
static int BindSocket(const int& sockfd,
                      const struct sockaddr& addr,
                      const int& addrlen) {
  return bind(sockfd, &addr, addrlen);
}

/*!
 *
 */
static int CloseSocket(const int& sockfd) {
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
    case AF_INET6:
      inet_ntop(addr.ss_family, &reinterpret_cast<struct sockaddr_in6*>(const_cast<struct sockaddr_storage*>(&addr))->sin6_addr,
        reinterpret_cast<char*>(const_cast<char*>(address.data())), INET6_ADDRSTRLEN);
      break;
    default:
      inet_ntop(addr.ss_family, &reinterpret_cast<struct sockaddr_in*>(const_cast<struct sockaddr_storage*>(&addr))->sin_addr,
        reinterpret_cast<char*>(const_cast<char*>(address.data())), INET_ADDRSTRLEN);
      break;
    }
    return address.data();
}

namespace engine {
namespace network {

std::string GetSocketAddress(const SocketAddress& address) {
  return GetSocketAddress(address.address);
}

/*!
  *
  */
void SocketBaseImpl::Open(const int& socket_type,
                          const int& flags,
                          const std::string& node_name,
                          const std::string& service_name) {
  socket_ = -1;
  current_address_info_ = nullptr;

  int family;
  switch(family_) {
  case SOCKET_UNSPECIFIED:
    family = AF_UNSPEC;
    break;
  case SOCKET_IPV4:
    family = AF_INET;
    break;
  case SOCKET_IPV6:
    family = AF_INET6;
    break;
  default:
    throw std::logic_error("Invalid socket type.");
  }

#ifdef _MSC_VER
  WSADATA wsa_data;
  // Initialize Winsock
  int wsa_result = WSAStartup(MAKEWORD(2,2), &wsa_data);
  if (wsa_result != 0) {
    exit(1); // "WSAStartup failed with result %i"
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

  if (getaddrinfo(new_node_name, service_name.c_str(), &hints, &address_info_) != 0) {
    throw std::domain_error("Unable to get address information.");
  }

  for (p = address_info_; p != 0; p = p->ai_next) {
    // If the family is unspecified, get the first one in the linked list.
    if (p->ai_family == family || family == AF_UNSPEC) {
      socket_ = CreateSocket(p->ai_family, p->ai_socktype, p->ai_protocol);
      if (socket_ != -1) {
        //m_bFamily = p->ai_family; // TODO: What to do here?
        current_address_info_ = p;
        break;
      }
    }
  }

  if (socket_ == -1) {
    throw std::domain_error("Unable to create socket.");
  }
}

/*!
  *
  */
SocketBase::SocketBase() : impl_(std::make_unique<SocketBaseImpl>()) {
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
int SocketBase::Bind(const uint16_t& port) {
  if (impl_->socket_ == -1 || !impl_->current_address_info_) {
    return -1;
  }

  struct sockaddr_in* sockaddr = reinterpret_cast<struct sockaddr_in*>(impl_->current_address_info_->ai_addr);
  sockaddr->sin_port = ntohs(port);
  if (BindSocket(impl_->socket_, *impl_->current_address_info_->ai_addr, static_cast<int>(impl_->current_address_info_->ai_addrlen)) != 0) {
    return -1;
  }
  return 0;
}

/*!
  *
  */
std::string SocketBase::GetAddress() {
  if (!impl_->current_address_info_) {
    return lib::EmptyString();
  }
  return GetSocketAddress(*reinterpret_cast<struct sockaddr_storage*>(impl_->current_address_info_->ai_addr));
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

} // end network namespace
} // end engine namespace
