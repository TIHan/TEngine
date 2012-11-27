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

#ifndef CLIENT_H_
#define CLIENT_H_

#include <engine_lib.h>
#include "send_message.h"
#include "receive_message.h"

namespace engine {
namespace network {

enum ClientMessage {
  kConnect = 248,
  kDisconnect = 249
};

class ClientInterface {
public:
  virtual ~ClientInterface() {}

  virtual void Connect(const std::string& address,
                       const std::string& port) = 0;
  virtual void Disconnect() = 0;

  virtual std::shared_ptr<SendMessage> CreateMessage(const int& type) = 0;
  virtual void RegisterMessageCallback(const int& type,
      std::function<void(std::shared_ptr<ReceiveMessage>)>& func) = 0;
  virtual void ProcessMessages() = 0;
  virtual void SendMessages() = 0;
};

class ClientImpl;
class Client : public virtual ClientInterface {
public:
  Client();
  virtual ~Client();

  virtual void Connect(const std::string& address, const std::string& port);
  virtual void Disconnect();

  virtual std::shared_ptr<SendMessage> CreateMessage(const int& type);
  virtual void RegisterMessageCallback(const int& type,
      std::function<void(std::shared_ptr<ReceiveMessage>)>& func);
  virtual void ProcessMessages();
  virtual void SendMessages();

private:
  std::unique_ptr<ClientImpl> impl_;
  std::shared_ptr<lib::ByteStream> send_stream_;
  std::shared_ptr<lib::ByteStream> receive_stream_;
  std::map<int,
           std::function<void(std::shared_ptr<ReceiveMessage>)>> callbacks_;
  std::unique_ptr<std::thread> receive_thread_;
  std::mutex receive_mutex_;
  int port_;

  /* Atomics */
  std::atomic<bool> close_receive_thread_;
};

} // end network namespace
} // end engine namespace

#endif // CLIENT_H_