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

#ifndef CLIENT_MESSAGE_PROCESSOR_H_
#define CLIENT_MESSAGE_PROCESSOR_H_

#include "receive_message.h"
#include "client_message.h"

namespace engine {
namespace network {

class ClientMessageProcessorInterface {
public:
  virtual ~ClientMessageProcessorInterface() {}

  virtual void StartReceiving(
      std::function<std::shared_ptr<ByteStream>()> func) = 0;

  virtual void Send(
      std::function<void(const ByteStream& buffer)> func) = 0;

  virtual void Stop() = 0;
  virtual void Process() = 0;
  virtual std::shared_ptr<ClientMessage> CreateMessage(int type) = 0;
  virtual void RegisterMessageCallback(int type,
      std::function<void(std::shared_ptr<ReceiveMessage>)> func) = 0;
};

class ClientMessageProcessor : public virtual ClientMessageProcessorInterface {
public:
  ClientMessageProcessor();
  virtual ~ClientMessageProcessor();

  virtual void StartReceiving(
      std::function<std::shared_ptr<ByteStream>()> func);

  virtual void Send(
      std::function<void(const ByteStream& buffer)> func);

  virtual void Stop();
  virtual void Process();
  virtual std::shared_ptr<ClientMessage> CreateMessage(int type);
  virtual void RegisterMessageCallback(int type,
      std::function<void(std::shared_ptr<ReceiveMessage>)> func);

private:
  // RECEVE
  std::queue<std::shared_ptr<ByteStream>> receive_buffer_;
  std::queue<std::shared_ptr<ByteStream>> receive_queue_;
  std::mutex receive_mutex_;
  std::thread receive_thread_;
  std::atomic_bool receive_close_;
  std::map<int,
           std::function<void(std::shared_ptr<ReceiveMessage>)>> callbacks_;
  // END RECEIVE

  // SEND
  std::shared_ptr<std::queue<std::shared_ptr<ByteStream>>> send_buffer_;
  std::queue<std::shared_ptr<ByteStream>> send_queue_;
  std::mutex send_mutex_;
  std::future<void> send_async_;
  // END SEND
};

} // end network namespace
} // end engine namespace

#endif // CLIENT_MESSAGE_PROCESSOR_H_
