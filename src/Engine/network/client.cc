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

#include "client.h"
#include "udp_socket.h"

#define REGISTER_MESSAGE_CALLBACK(type, func) \
  message_processor_.RegisterMessageCallback(type, \
      [=] (std::shared_ptr<network::ReceiveMessage> message) func) \

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
  connected_ = false;

  REGISTER_MESSAGE_CALLBACK(ReservedServerMessage::kAckClientConnect, {
    connected_ = true;
    std::cout << "CONNECTED!" << std::endl;
  });

  REGISTER_MESSAGE_CALLBACK(ReservedServerMessage::kAckClientHeartbeat, {
    std::cout << "Server Acknowledged Heartbeat!" << std::endl;
  });
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

  auto connect_msg = CreateMessage(ReservedClientMessage::kConnect);
  connect_msg->Send();
}

void Client::OnConnect(std::function<void()> func) {
  connect_func_ = func;
}

void Client::OnDisconnect(std::function<void()> func) {
  disconnect_func_ = func;
}

void Client::Disconnect() {
  if (connected_) {
    auto disconnect_msg = CreateMessage(ReservedClientMessage::kDisconnect);
    disconnect_msg->Send();
    SendMessages();
  }

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

void Client::SendHeartbeat() {
  auto connect_msg = CreateMessage(ReservedClientMessage::kHeartbeat);
  connect_msg->Send();
}

} // end network namespace
} // end engine namespace