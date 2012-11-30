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

namespace engine {
namespace network {

class ClientImpl {
public:
  ClientImpl();

  UdpSocket socket_;
};

ClientImpl::ClientImpl() : socket_(false) {
}

Client::Client() : impl_(std::make_unique<ClientImpl>()) {
}

Client::~Client() {
  Disconnect();
}

void Client::Connect(const std::string& address, const std::string& port) {
  impl_->socket_.Open(address, port);
  server_address_ = address;
  server_port_ = port;

  receive_process_.Run([=] () {
    receive_mutex_.lock(); // LOCK

    auto receive = impl_->socket_.ReceiveFrom();
    receive_stream_->WriteBuffer(*std::get<0>(receive));

    receive_mutex_.unlock(); // UNLOCK
  });
}

void Client::Disconnect() {
  receive_process_.Stop();
  impl_->socket_.Close();
}

std::shared_ptr<ClientMessage> Client::CreateMessage(const int& type) {
  return std::make_shared<ClientMessage>(send_stream_, type);
}

void Client::SendMessages() {
  impl_->socket_.Send(*send_stream_);
  send_stream_->Reset();
}

} // end network namespace
} // end engine namespace