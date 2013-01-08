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

#include "server_message_processor.h"

using namespace engine::lib;

namespace engine {
namespace network {

ServerMessageProcessor::ServerMessageProcessor()
    : send_buffer_(
        std::make_shared<std::queue<std::shared_ptr<lib::ByteStream>>>()) {
  receive_close_ = false;
}

ServerMessageProcessor::~ServerMessageProcessor() {
  Stop();
}

void ServerMessageProcessor::StartReceiving(
    std::function<std::pair<int, std::shared_ptr<ByteStream>>()> func) {
  receive_thread_ = std::thread([=] () {
    while (!receive_close_) {
      auto receive = func();

      if (receive.second && receive.second->GetSize() != 0) {
        receive_mutex_.lock(); // LOCK

        recipient_buffers_[receive.first]->receive->push(receive.second);

        receive_mutex_.unlock(); // UNLOCK
      }
    }
  });
}

void ServerMessageProcessor::Send(
    std::function<void(const ByteStream& buffer, uint8_t recipient_id)> func) {
  send_async_ = std::async(std::launch::async, [=] {
    for (auto recipient_buffer : recipient_buffers_) {
      while (!recipient_buffer.second->send->empty()) {
        func(*recipient_buffer.second->send->front(), recipient_buffer.first);
        recipient_buffer.second->send->pop();
      }
    }
  });
}

void ServerMessageProcessor::Stop() {
  receive_close_ = true;
  if (receive_thread_.joinable()) receive_thread_.join();
  if (send_async_.valid()) send_async_.wait();
}

void ServerMessageProcessor::Process() {
  for (auto recipient_buffer : recipient_buffers_) {
    std::queue<std::shared_ptr<ByteStream>> receive_buffer;

    receive_mutex_.lock(); // LOCK
    receive_buffer.swap(*recipient_buffer.second->receive);
    receive_mutex_.unlock(); // UNLOCK

    while (!receive_buffer.empty()) {
      auto stream = receive_buffer.front();
      while (stream->read_position() < stream->GetSize()) {
        try {
          uint8_t first_byte = stream->ReadByte();

          auto iter = callbacks_.find(first_byte);

          if (iter != callbacks_.end()) {
            auto message = iter->second;
            message(std::make_shared<ReceiveMessage>(stream, first_byte),
                    recipient_buffer.first);
          } else {
            throw std::logic_error("Invalid message.");
          }
        } catch (const std::exception& e) {
          throw e;
        }
      }
      receive_buffer.pop();
    }
  }
}

std::shared_ptr<ServerMessage> ServerMessageProcessor::CreateMessage(
    int type) {
  return std::make_shared<ServerMessage>(send_buffer_, type);
}

std::shared_ptr<ServerMessage> ServerMessageProcessor::CreateMessage(
    int type, uint8_t recipient_id) {
  return std::make_shared<ServerMessage>(
      recipient_buffers_[recipient_id]->send, type);
}

void ServerMessageProcessor::RegisterMessageCallback(int type,
    std::function<void(std::shared_ptr<ReceiveMessage>, int)> func) {
  callbacks_[type] = func;
}

void ServerMessageProcessor::AddRecipientId(uint8_t id) {
  recipient_buffers_[id] = std::make_shared<TwoWayBuffer>();
}

void ServerMessageProcessor::RemoveRecipientId(uint8_t id) {
  recipient_buffers_.erase(id);
}

} // end network namespace
} // end engine namespace