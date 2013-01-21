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

#include "server_message_processor.h"

namespace engine {
namespace network {

ServerMessageProcessor::ServerMessageProcessor()
    : send_channel_(std::make_shared<ServerChannel>()) {
  receive_close_ = false;
}

ServerMessageProcessor::~ServerMessageProcessor() {
  Stop();
}

void ServerMessageProcessor::StartReceiving(
    std::function<std::pair<int, std::shared_ptr<ByteStream>>()> func) {
  receive_thread_ = std::thread([=] () {
    while (!receive_close_) {
        receive_channel_.Push(func());
    }
  });
}

void ServerMessageProcessor::Send(
    std::function<void(const ByteStream& buffer, uint8_t recipient_id)> func) {
  send_async_ = std::async(std::launch::async, [=] {
    send_channel_->Flush([=] (std::shared_ptr<ServerPacket> packet) {
      func(*packet, packet->client_id());
    });
  });
}

void ServerMessageProcessor::Stop() {
  receive_close_ = true;
  if (receive_thread_.joinable()) receive_thread_.join();
  if (send_async_.valid()) send_async_.wait();
}

void ServerMessageProcessor::Process() {
  receive_channel_.Flush([=] (std::shared_ptr<ServerPacket> packet) {
    while (packet->CanRead()) {
      try {
        message_handler_.Handle(packet);
      } catch (const std::runtime_error& e) {
        throw e;
      }
    }
  });
}

void ServerMessageProcessor::RegisterMessageCallback(int type,
    std::function<void(std::shared_ptr<ReceiveMessage>, int)> func) {
  message_handler_.RegisterHandler(type, func);
}

std::unique_ptr<ServerMessage> ServerMessageProcessor::CreateMessage(int type,
    std::shared_ptr<std::list<int>> client_ids) {
  return std::make_unique<ServerMessage>(type, client_ids, send_channel_);
}

} // end network namespace
} // end engine namespace