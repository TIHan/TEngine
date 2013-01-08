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
#include <unordered_map>

namespace engine {
namespace network {

class ServerImpl {
public:
  ServerImpl();

  std::unique_ptr<UdpSocket> socket_;
  std::unordered_map<uint8_t, std::shared_ptr<SocketAddress>> recipients_;
};

ServerImpl::ServerImpl() {
  UdpSocketOptions options;
  options.max_receive_buffer = 128;
  options.family = SocketFamily::kIpv4;
  options.blocking = false;
  socket_ = std::make_unique<UdpSocket>(options);
}

std::atomic_uint8_t g_id_count;

Server::Server(int port)
    : impl_(std::make_unique<ServerImpl>()) {
  if (port <= 0) throw std::out_of_range("port is 0 or less.");
  port_ = port;

  message_processor_.RegisterMessageCallback(ReservedClientMessage::kConnect,
      [=] (std::shared_ptr<network::ReceiveMessage> message,
           uint8_t recipient_id) {
  });

  message_processor_.RegisterMessageCallback(ReservedClientMessage::kHeartbeat,
      [=] (std::shared_ptr<network::ReceiveMessage> message,
           uint8_t recipient_id) {
    std::cout << "Client Sent a Heartbeat!" << std::endl;
    auto ack = message_processor_.
        CreateMessage(ReservedServerMessage::kAckHeartbeat, recipient_id);
    ack->Send();
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

  message_processor_.StartReceiving([=] () {
    if (impl_->socket_->WaitToRead(2500)) {
      auto receive = impl_->socket_->ReceiveFrom();
      auto buffer = std::get<0>(receive);
      auto address = std::get<1>(receive);
      
      for (auto recipient : impl_->recipients_) {
        if (*recipient.second == *address) {
          return std::make_pair(recipient.first, buffer);
        }
      }

      if (buffer->ReadByte() == ReservedClientMessage::kConnect) {
        // Got a new recipient, add him to the list.
        uint8_t id = ++g_id_count;
        impl_->recipients_[id] = address;
        message_processor_.AddRecipientId(id);

        auto handshake = message_processor_.
            CreateMessage(ReservedServerMessage::kHandshake, id);
        handshake->Send();

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
  message_processor_.Stop();
  impl_->socket_->Close();
}

std::shared_ptr<ServerMessage> Server::CreateMessage(int type) {
  return message_processor_.CreateMessage(type);
}

void Server::ProcessMessages() {
  message_processor_.Process();
}

void Server::SendMessages() {
  message_processor_.Send([=] (const ByteStream& buffer, int recipient_id) {
    for (auto recipient : impl_->recipients_) {
      if (recipient.first == recipient_id) {
        impl_->socket_->SendTo(buffer, *recipient.second);
      }
    }
  });
}

int Server::port() const {
  return port_;
}

} // end network namespace
} // end engine namespace