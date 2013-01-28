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

#ifndef CLIENT_INTERFACE_H_
#define CLIENT_INTERFACE_H_

#include "receive_message.h"
#include "client_message.h"

namespace engine {
namespace network {

class ClientInterface {
public:
  virtual ~ClientInterface() {}

  virtual void Connect(const std::string& address,
                       const std::string& port) = 0;
  virtual void Disconnect() = 0;

  virtual std::shared_ptr<ClientMessage> CreateMessage(int type) = 0;
  virtual void RegisterMessageCallback(int type,
      std::function<void(std::shared_ptr<ReceiveMessage>)> func) = 0;

  // Message Handlers
  virtual void OnConnect(std::function<void()> func) = 0;
  virtual void OnDisconnect(std::function<void()> func) = 0;
};

} // end network namespace
} // end engine namespace

#endif // CLIENT_INTERFACE_H_
