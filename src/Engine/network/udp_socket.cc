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

#include "udp_socket.h"
#include "socket_base-impl.h"

namespace engine {
namespace network {

/*!
  *
  */
UdpSocket::UdpSocket() {
  Init();
}

/*!
  *
  */
UdpSocket::UdpSocket(const UdpSocketOptions& options) {
  options_ = options;
  Init();
}

/*!
  *
  */
UdpSocket::~UdpSocket() {
}

/*!
  *
  */
void UdpSocket::Init() {
  receive_buffer_ = std::make_unique<std::vector<uint8_t>>(
      options_.max_receive_buffer);
  impl_->family_ = options_.family;
  impl_->blocking_ = options_.blocking;
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
void UdpSocket::Open(const std::string& address, const std::string& port) {
  impl_->Open(SOCK_DGRAM, AI_PASSIVE, address, port);
}

/*!
  *
  */
std::tuple<std::shared_ptr<lib::ByteStream>,
           std::shared_ptr<SocketAddress>> UdpSocket::ReceiveFrom() {
  struct sockaddr_storage sock_addr_storage;
  socklen_t addr_len = sizeof(sock_addr_storage);

  char* data = reinterpret_cast<char*>(const_cast<uint8_t*>(
       receive_buffer_->data()));
  int data_size = static_cast<int>(receive_buffer_->size());
  auto sock_addr = reinterpret_cast<sockaddr*>(
      const_cast<struct sockaddr_storage*>(&sock_addr_storage));

  int bytes = recvfrom(impl_->socket_, data,
                       data_size, 0, sock_addr, &addr_len);

  auto buffer = std::make_shared<lib::ByteStream>();
  if (bytes != -1)
    buffer->WriteBuffer(*receive_buffer_, bytes);
  
  auto address_data =
      std::make_shared<SocketAddressData>(sock_addr_storage, addr_len);
  auto address = std::make_shared<SocketAddress>(address_data);
  return std::make_tuple(buffer, address);
}

/*!
  *
  */
int UdpSocket::Send(const lib::ByteStream& data) {
  char* data_send = reinterpret_cast<char*>
      (const_cast<uint8_t*>(data.GetRaw()));
  int data_size = static_cast<int>(data.GetSize());
  int addr_len = static_cast<int>(impl_->current_address_info_->ai_addrlen);

  int bytes = sendto(impl_->socket_, data_send, data_size, 0,
                     impl_->current_address_info_->ai_addr, addr_len);
  return bytes;
}

/*!
  *
  */
int UdpSocket::SendTo(const lib::ByteStream& data,
                      const SocketAddress& address) {
  char* data_send = reinterpret_cast<char*>(const_cast<uint8_t*>(
                                            data.GetRaw()));
  int data_size = static_cast<int>(data.GetSize());
  auto sock_addr = reinterpret_cast<sockaddr*>(
      const_cast<void*>(address.GetRaw()));

  int bytes = sendto(impl_->socket_, data_send, data_size, 0, sock_addr,
                     address.GetLength());
  return bytes;
}

/*!
  *
  */
bool UdpSocket::WaitToRead(int usec) {
  timeval tv;
  fd_set read_set;

  tv.tv_sec = 0;
  tv.tv_usec = usec;

  FD_ZERO(&read_set);
  FD_SET(impl_->socket_, &read_set);

  // Is this right? Works for now in Windows + Linux.
  select(impl_->socket_ + 1, &read_set, 0, 0, &tv);

  if (FD_ISSET(impl_->socket_, &read_set)) {
    return true;
  } else {
    return false;
  }
}

} // end network namespace
} // end 
