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

#include "server.h"
#include "udp_socket.h"

namespace engine {
namespace network {

class ServerImpl {
public:
  UdpSocket socket_;
  std::list<std::shared_ptr<SocketAddress>> addresses_;
};

Server::Server(const int& port) : impl_(std::make_unique<ServerImpl>()),
      sendStream_(std::make_shared<lib::ByteStream>()),
      receiveStream_(std::make_shared<lib::ByteStream>()) {
  if (port <= 0) throw std::out_of_range("port is 0 or less.");
  port_ = port;
}

Server::~Server() {
}

void Server::Start() {
  impl_->socket_.Open();
  
  int success = -1;
  for (int port = port_; port < port_ + 1000; ++port) {
    success = impl_->socket_.Bind(port);
    if (success == 0) {
      port_ = port;
      break;
    }
  }

  if (success == -1) throw std::domain_error("Unable to bind port.");
}

void Server::Stop() {
  impl_->socket_.Close();
}

std::shared_ptr<SendMessage> Server::CreateMessage(const int& type) {
  return std::make_shared<SendMessage>(sendStream_, type);
}

void Server::RegisterMessageCallback(const int& type,
    std::function<void(std::unique_ptr<ReceiveMessage>)>& func) {
  callbacks_[type] = func;
}

void Server::ProcessMessages() {
  auto stream = receiveStream_;
  while (stream->read_position() < stream->GetSize()) {
    uint8_t first_byte = stream->Read<uint8_t>();

    std::map<int,
        std::function<void(std::unique_ptr<ReceiveMessage>)>>
        ::iterator iter = callbacks_.find(first_byte);

    if (iter != callbacks_.end()) {
      auto message = iter->second;
      message(std::make_unique<ReceiveMessage>(stream, first_byte));
    } else {
      throw std::logic_error("Invalid message.");
    }
  }
}

void Server::SendMessages() {
  auto addresses = impl_->addresses_;

  for_each(addresses.cbegin(), addresses.cend(),
           [&] (std::shared_ptr<SocketAddress> address) {
    impl_->socket_.SendTo(*sendStream_, *address);
  });
}

int Server::port() const {
  return port_;
}

void Server::set_port(const int& port) {
  port_ = port;
}

} // end network namespace
} // end engine namespace