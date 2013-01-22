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

#ifndef SERVER_REACTOR_H_
#define SERVER_REACTOR_H_

#include "server_message.h"
#include "server_message_handler.h"

namespace engine {
namespace network {

/********************************************//**
 * The more I think about it, the more I keep telling myself that this
 * class is very similar to the "Reactor" pattern.
 *
 * Quoting from "Pattern-Oriented Software Architecture
 *               Volume 2
 *               Patterns for Concurrent and Networked Objects
 *               Page 179":
 *               - "The Reactor architectural pattern allows
 *               event-driven applications to demultiplex and dispatch
 *               service requests that are delivered to an application
 *               from one or more clients."
 *
 * I finally came to this conclusion after noticing very similar ideas
 * and constructs between what has been coded here and what was described
 * in the book.
 *
 * Remember that patterns are guidelines, not concrete solutions
 * for every problem. This class isn't a pure implementation of the "Reactor"
 * pattern, but rather a few subset of ideas from the pattern; notably, the
 * message handler and its synchronous dispatching.
 ***********************************************/
class ServerReactor {
public:
  ServerReactor();
  virtual ~ServerReactor();

  virtual void StartReceiving(
      std::function<std::pair<int, std::shared_ptr<ByteStream>>()> func);

  virtual void Send(
      std::function<void(const ByteStream& buffer,
                         uint8_t client_id)> func);

  virtual void Stop();
  virtual void Process();
  virtual void RegisterMessageCallback(int type,
    std::function<void(std::shared_ptr<ReceiveMessage>, int)> func);

  virtual std::unique_ptr<ServerMessage> CreateMessage(int type,
      std::shared_ptr<std::list<int>> client_ids);

private:
  std::thread receive_thread_;
  std::atomic<bool> receive_close_;
  std::future<void> send_async_;

  ServerChannel receive_channel_;
  std::shared_ptr<ServerChannel> send_channel_;
  ServerMessageHandler message_handler_;
};

} // end network namespace
} // end engine namespace

#endif // SERVER_REACTOR_H_
