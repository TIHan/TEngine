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

#ifndef SERVER_MESSAGE_PROCESSOR_H_
#define SERVER_MESSAGE_PROCESSOR_H_

#include "receive_message.h"
#include "server_message.h"

namespace engine {
namespace network {

class ServerMessageHandler {
public:
  ServerMessageHandler();
  virtual ~ServerMessageHandler();

  void RegisterHandler(int type,
      std::function<void(std::shared_ptr<ReceiveMessage>, int)> handler);

  void RemoveHandler(int type);

  void Handle(std::shared_ptr<ServerPacket> packet);


private:
  std::map<int,
           std::function<void(std::shared_ptr<ReceiveMessage>, 
                              int)>> handlers_;
};

inline ServerMessageHandler::ServerMessageHandler() {

}

inline ServerMessageHandler::~ServerMessageHandler() {

}

inline void ServerMessageHandler::RegisterHandler(int type, 
    std::function<void(std::shared_ptr<ReceiveMessage>, int)> handler) {
  handlers_[type] = handler;
}

inline void ServerMessageHandler::RemoveHandler(int type) {
  handlers_.erase(type);
}

inline void ServerMessageHandler::Handle(
    std::shared_ptr<ServerPacket> packet) {
  auto message_byte = packet->ReadByte();
  auto iter = handlers_.find(message_byte);
  if (iter != handlers_.end()) {
    auto handler = iter->second;
    handler(std::make_shared<ReceiveMessage>(packet, message_byte),
                                             packet->client_id());
  } else {
    throw std::logic_error("Invalid message.");
  }
}

class ServerMessageProcessor {
public:
  ServerMessageProcessor();
  virtual ~ServerMessageProcessor();

  virtual void StartReceiving(
      std::function<std::pair<int, std::shared_ptr<ByteStream>>()> func);

  virtual void Send(
      std::function<void(const ByteStream& buffer, uint8_t recipient_id)> func);

  virtual void Stop();
  virtual void Process();
  virtual void RegisterMessageCallback(int type,
    std::function<void(std::shared_ptr<ReceiveMessage>, int)> func);

  virtual std::unique_ptr<ServerMessage> CreateMessage(int type,
      std::shared_ptr<std::list<int>> client_ids);

private:
  std::thread receive_thread_;
  std::atomic_bool receive_close_;
  std::future<void> send_async_;

  ServerChannel receive_channel_;
  std::shared_ptr<ServerChannel> send_channel_;
  ServerMessageHandler message_handler_;
};

} // end network namespace
} // end engine namespace

#endif // CLIENT_MESSAGE_PROCESSOR_H_
