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

#ifndef SERVER_MESSAGE_H_
#define SERVER_MESSAGE_H_

#include "message_base.h"
#include "server_message_processor.h"

namespace engine {
namespace network {

class ServerMessage : public MessageBase {
public:
  ServerMessage(std::shared_ptr<ServerMessageProcessor> processor, int type);
  virtual ~ServerMessage();

  void Send();
  void Send(int client_id);

private:
  std::shared_ptr<ServerMessageProcessor> processor_;
};

inline ServerMessage::ServerMessage(
    std::shared_ptr<ServerMessageProcessor> processor, int type)
    : MessageBase(type), processor_(processor) {
}

inline ServerMessage::~ServerMessage() {
}

inline void ServerMessage::Send(int client_id) {
  if (buffer_->GetSize() > kMaximumTransmissionUnit)
    throw std::out_of_range("Message is too big.");

  processor_->PushOnSend(buffer_, client_id);
}

} // end network namespace
} // end engine namespace

#endif // SERVER_MESSAGE_H_
