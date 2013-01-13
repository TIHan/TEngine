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

#ifndef SERVER_H_
#define SERVER_H_

#include "server_message_processor.h"
#include "server_message.h"

namespace engine {
namespace network {

const int kMaxClients = 64;
const int kMaxServerPerClientTransfer = 8192;

class ServerInterface {
public:
  virtual ~ServerInterface() {}

  virtual void Start() = 0;
  virtual void Stop() = 0;

  virtual std::shared_ptr<ServerMessage> CreateMessage(int type) = 0;

  virtual void ProcessMessages() = 0;
  virtual void SendMessages() = 0;

  /* Accessors / Mutators */
  virtual int port() const = 0;
};

class ServerImpl;
class Server : public virtual ServerInterface {
public:
  explicit Server(int port);
  virtual ~Server();

  virtual void Start();
  virtual void Stop();

  virtual std::shared_ptr<ServerMessage> CreateMessage(int type);

  virtual void ProcessMessages();
  virtual void SendMessages();

  /* Accessors / Mutators */
  virtual int port() const;

private:
  std::unique_ptr<ServerImpl> impl_;

  std::shared_ptr<ServerMessageProcessor> message_processor_;
  std::atomic<uint8_t> id_count_;
  int port_;
};

} // end network namespace
} // end engine namespace

#endif // SERVER_H_
