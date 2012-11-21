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

#include "udp_socket.h"
#include "socket_base-impl.h"

namespace engine {
namespace network {

/*!
  *
  */
UdpSocket::UdpSocket(const SocketFamily& family) {
  impl_->family_ = family;
}

/*!
  *
  */
UdpSocket::UdpSocket() {
  impl_->family_ = (SOCKET_UNSPECIFIED);
}

/*!
  *
  */
UdpSocket::~UdpSocket() {
}

/*!
  *
  */
void UdpSocket::Open() {
  impl_->Open(SOCK_DGRAM, AI_PASSIVE, lib::EmptyString(), lib::EmptyString());
}

/*!
  *
  */
void UdpSocket::Open(const std::string& address,
                     const std::string& port) {
  impl_->Open(SOCK_DGRAM, AI_PASSIVE, address, port);
}

/*!
  *
  */
std::tuple<std::shared_ptr<std::vector<uint8_t>>, std::shared_ptr<SocketAddress>> UdpSocket::ReceiveFrom() {
  struct sockaddr_storage sock_addr;
  socklen_t addr_len = sizeof(sock_addr);
  auto buffer = std::make_shared<std::vector<uint8_t>>(SOCKET_MAX_BUFFER);

  int bytes = recvfrom(impl_->socket_, reinterpret_cast<char*>(const_cast<uint8_t*>(buffer->data())),
    buffer->size(), 0, reinterpret_cast<sockaddr*>(const_cast<struct sockaddr_storage*>(&sock_addr)), &addr_len);

  if (bytes != -1) {
    buffer->resize(bytes);
    buffer->shrink_to_fit();
  } else {
    buffer->clear();
  }

  auto address = std::make_shared<SocketAddress>(sock_addr, static_cast<int>(addr_len));
  return std::tuple<std::shared_ptr<std::vector<uint8_t>>, std::shared_ptr<SocketAddress>>(buffer, address);
}

/*!
  *
  */
int UdpSocket::Send(const std::vector<uint8_t>& data) {
  int bytes = sendto(impl_->socket_, reinterpret_cast<char*>(const_cast<uint8_t*>(data.data())),
    data.size(), 0, impl_->current_address_info_->ai_addr, static_cast<int>(impl_->current_address_info_->ai_addrlen));
  return bytes;
}

/*!
  *
  */
int UdpSocket::Send(const lib::ByteStream& data) {
  int bytes = sendto(impl_->socket_, reinterpret_cast<const char*>(const_cast<uint8_t*>(data.GetRaw())),
    data.GetSize(), 0, impl_->current_address_info_->ai_addr, static_cast<int>(impl_->current_address_info_->ai_addrlen));
  return bytes;
}

/*!
  *
  */
int UdpSocket::SendTo(const std::vector<uint8_t>& data,
                      const SocketAddress& address) {
  int bytes = sendto(impl_->socket_, reinterpret_cast<const char*>(const_cast<uint8_t*>(data.data())),
    data.size(), 0, reinterpret_cast<sockaddr*>(const_cast<struct sockaddr_storage*>(&address.address)), address.length);
  return bytes;
}

/*!
  *
  */
int UdpSocket::SendTo(const lib::ByteStream& data,
                      const SocketAddress& address) {
  int bytes = sendto(impl_->socket_, reinterpret_cast<const char*>(const_cast<const uint8_t*>(data.GetRaw())),
    data.GetSize(), 0, reinterpret_cast<sockaddr*>(const_cast<struct sockaddr_storage*>(&address.address)), address.length);
  return bytes;
}

} // end network namespace
} // end 