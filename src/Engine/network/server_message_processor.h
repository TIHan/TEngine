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

#ifndef SERVER_MESSAGE_PROCESSOR_H_
#define SERVER_MESSAGE_PROCESSOR_H_

#include "receive_message.h"
#include "server_message.h"
#include <unordered_map>

using namespace engine::lib;

namespace engine {
namespace network {

struct TwoWayBuffer {
  TwoWayBuffer() {
    send = std::make_shared<std::queue<std::shared_ptr<ByteStream>>>();
    receive = std::make_shared<std::queue<std::shared_ptr<ByteStream>>>();
  }

  std::shared_ptr<std::queue<std::shared_ptr<ByteStream>>> send;
  std::shared_ptr<std::queue<std::shared_ptr<ByteStream>>> receive;
};

class ServerMessageProcessorInterface {
public:
  virtual ~ServerMessageProcessorInterface() {}

  virtual void StartReceiving(
      std::function<std::pair<int, std::shared_ptr<ByteStream>>()> func) = 0;

  virtual void Send(
      std::function<
          void(const ByteStream& buffer, uint8_t recipient_id)> func) = 0;

  virtual void Stop() = 0;
  virtual void Process() = 0;
  virtual std::shared_ptr<ServerMessage> CreateMessage(int type) = 0;
  virtual std::shared_ptr<ServerMessage> CreateMessage(int type,
                                                       uint8_t recipient_id) = 0;
  virtual void RegisterMessageCallback(int type,
      std::function<void(std::shared_ptr<ReceiveMessage>, int)> func) = 0;

  virtual void AddRecipientId(uint8_t id) = 0;
  virtual void RemoveRecipientId(uint8_t id) = 0;
};

class ServerMessageProcessor : public virtual ServerMessageProcessorInterface {
public:
  ServerMessageProcessor();
  virtual ~ServerMessageProcessor();

  virtual void StartReceiving(
      std::function<std::pair<int, std::shared_ptr<ByteStream>>()> func);

  virtual void Send(
      std::function<void(const ByteStream& buffer, uint8_t recipient_id)> func);

  virtual void Stop();
  virtual void Process();
  virtual std::shared_ptr<ServerMessage> CreateMessage(int type);
  virtual std::shared_ptr<ServerMessage> CreateMessage(int type,
                                                       uint8_t recipient_id);
  virtual void RegisterMessageCallback(int type,
      std::function<void(std::shared_ptr<ReceiveMessage>, int)> func);

  virtual void AddRecipientId(uint8_t id);
  virtual void RemoveRecipientId(uint8_t id);

private:
  // RECEVE
  std::mutex receive_mutex_;
  std::thread receive_thread_;
  std::atomic_bool receive_close_;
  std::map<int,
           std::function<void(std::shared_ptr<ReceiveMessage>,
                              int)>> callbacks_;
  // END RECEIVE

  // SEND
  std::shared_ptr<std::queue<std::shared_ptr<ByteStream>>> send_buffer_;
  std::queue<std::shared_ptr<ByteStream>> send_queue_;
  std::mutex send_mutex_;
  std::future<void> send_async_;
  // END SEND

  std::unordered_map<uint8_t, std::shared_ptr<TwoWayBuffer>> recipient_buffers_;
};

} // end network namespace
} // end engine namespace

#endif // CLIENT_MESSAGE_PROCESSOR_H_
