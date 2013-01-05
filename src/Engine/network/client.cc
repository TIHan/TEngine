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

#include "client.h"
#include "udp_socket.h"

using namespace engine::lib;

namespace engine {
namespace network {

class ClientImpl {
public:
  ClientImpl();

  std::unique_ptr<UdpSocket> socket_;
};

ClientImpl::ClientImpl() {
  UdpSocketOptions options;
  options.max_receive_buffer = 1024;
  options.blocking = false;
  socket_ = std::make_unique<UdpSocket>(options);
}

Client::Client()
    : impl_(std::make_unique<ClientImpl>()) {
}

Client::~Client() {
  Disconnect();
}

void Client::Connect(const std::string& address, const std::string& port) {
  impl_->socket_->Open(address, port);
  server_address_ = address;
  server_port_ = port;

  message_processor_.StartReceiving([=] () {
    if (impl_->socket_->WaitToRead(2500)) {
      auto receive = impl_->socket_->ReceiveFrom();
      return std::get<0>(receive);
    } else {
      return std::make_shared<ByteStream>();
    }
  });
}

void Client::Disconnect() {
  message_processor_.Stop();
  impl_->socket_->Close();
}

std::shared_ptr<ClientMessage> Client::CreateMessage(int type) {
  return message_processor_.CreateMessage(type);
}

void Client::RegisterMessageCallback(int type,
    std::function<void(std::shared_ptr<ReceiveMessage>)> func) {
  message_processor_.RegisterMessageCallback(type, func);
}

void Client::ProcessMessages() {
  message_processor_.Process();
}

void Client::SendMessages() {
  message_processor_.Send([=] (const ByteStream& buffer) {
    impl_->socket_->Send(buffer);
  });
}

} // end network namespace
} // end engine namespace