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

#include "server.h"
#include "udp_socket.h"

#define REGISTER_MESSAGE_CALLBACK(type, func) \
  message_processor_->RegisterMessageCallback(type, \
      [=] (std::shared_ptr<network::ReceiveMessage> message, \
          uint8_t client_id) func) \

namespace engine {
namespace network {

class ServerImpl {
public:
  ServerImpl(); 

  std::unique_ptr<UdpSocket> socket_;
  std::unordered_map<uint8_t, std::shared_ptr<SocketAddress>> clients_;
};

ServerImpl::ServerImpl() {
  UdpSocketOptions options;
  options.max_receive_buffer = 128;
  options.family = SocketFamily::kIpv4;
  options.blocking = false;
  socket_ = std::make_unique<UdpSocket>(options);
}

Server::Server(int port)
    : impl_(std::make_unique<ServerImpl>()),
      message_processor_(std::make_shared<ServerMessageProcessor>()) {
  if (port <= 0) throw std::out_of_range("port is 0 or less.");
  port_ = port;

  REGISTER_MESSAGE_CALLBACK(ReservedClientMessage::kConnect, {
  });

  REGISTER_MESSAGE_CALLBACK(ReservedClientMessage::kHeartbeat, {
    std::cout << "Client Sent a Heartbeat!" << std::endl;
    auto ack = CreateMessage(ReservedServerMessage::kAckClientHeartbeat);
    ack->Send(client_id);
  });
}

Server::~Server() {
  Stop();
}

void Server::Start() {
  impl_->socket_->Open();
  
  int success = -1;
  for (int port = port_; port < port_ + 1000; ++port) {
    success = impl_->socket_->Bind(port);
    if (success == 0) {
      port_ = port;
      break;
    }
  }

  if (success == -1) throw std::domain_error("Unable to bind port.");

  message_processor_->StartReceiving([=] () {
    if (impl_->socket_->WaitToRead(2500)) {
      auto receive = impl_->socket_->ReceiveFrom();
      auto buffer = std::get<0>(receive);
      auto address = std::get<1>(receive);
      
      for (auto client : impl_->clients_) {
        if (*client.second == *address) {
          return std::make_pair(client.first, buffer);
        }
      }

      // Client is not connected, only accept connect messages.
      if (buffer->ReadByte() == ReservedClientMessage::kConnect) {
        auto id = ++id_count_;
        impl_->clients_[id] = address;

        auto handshake = CreateMessage(ReservedServerMessage::kAckClientConnect);
        handshake->Send(id);

        return std::make_pair(id, buffer);
      }

      return std::make_pair(static_cast<uint8_t>(0),
                            std::make_shared<ByteStream>());
    } else {
      return std::make_pair(static_cast<uint8_t>(0),
                            std::make_shared<ByteStream>());
    }
  });
}

void Server::Stop() {
  message_processor_->Stop();
  impl_->socket_->Close();
}

std::shared_ptr<ServerMessage> Server::CreateMessage(int type) {
  return message_processor_->CreateMessage(type, std::move(GetClientIds()));
}

void Server::ProcessMessages() {
  message_processor_->Process();
}

void Server::SendMessages() {
  message_processor_->Send([=] (const ByteStream& buffer, int client_id) {
    for (auto client : impl_->clients_) {
      if (client.first == client_id) {
        impl_->socket_->SendTo(buffer, *client.second);
      }
    }
  });
}

std::unique_ptr<std::list<int>> Server::GetClientIds() {
  auto clients = std::make_unique<std::list<int>>();
  for (auto client : impl_->clients_) {
    clients->push_back(client.first);
  }
  return std::move(clients);
}

int Server::port() const {
  return port_;
}

} // end network namespace
} // end engine namespace