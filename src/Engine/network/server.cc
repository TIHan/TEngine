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

typedef std::list<std::shared_ptr<SocketAddress>> AddressList;

class ServerImpl {
public:
  ServerImpl();

  UdpSocket socket_;
  AddressList addresses_;
};

ServerImpl::ServerImpl() : socket_(SocketFamily::kIpv4, false) {
}

Server::Server(const int& port)
    : impl_(std::make_unique<ServerImpl>()),
      send_queue_(std::make_shared<SendQueue>()) {
  if (port <= 0) throw std::out_of_range("port is 0 or less.");
  port_ = port;
}

Server::~Server() {
  Stop();
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

  receive_process_.Run([=] () {
    receive_mutex_.lock(); // LOCK

    auto receive = impl_->socket_.ReceiveFrom();
    receive_stream_->WriteBuffer(*std::get<0>(receive));

    receive_mutex_.unlock(); // UNLOCK
  });
}

void Server::Stop() {
  receive_process_.Stop();
  impl_->socket_.Close();
}

std::shared_ptr<ServerMessage> Server::CreateMessage(const int& type) {
  return std::make_shared<ServerMessage>(send_queue_, type);
}

void Server::ProcessMessages() {
  receive_mutex_.lock(); // LOCK

  while (receive_stream_->read_position() < receive_stream_->GetSize()) {
    try {
      uint8_t first_byte = receive_stream_->ReadByte();

      auto iter = callbacks_.find(first_byte);

      if (iter != callbacks_.end()) {
        auto message = iter->second;
        message(std::make_shared<ReceiveMessage>(receive_stream_, first_byte));
      } else {
        throw std::logic_error("Invalid message.");
      }
    } catch (const std::exception& e) {
      receive_mutex_.unlock(); // UNLOCK
      throw e;
    }
  }

  receive_mutex_.unlock(); // UNLOCK
}

void Server::SendMessages() {
  auto addresses = impl_->addresses_;

  while (!send_queue_->empty()) {
    auto data = send_queue_->front();
    for_each(addresses.cbegin(), addresses.cend(),
             [&] (std::shared_ptr<SocketAddress> address) {
      impl_->socket_.SendTo(*data, *address);
    });
    send_queue_->pop();
  }
}

int Server::port() const {
  return port_;
}

} // end network namespace
} // end engine namespace