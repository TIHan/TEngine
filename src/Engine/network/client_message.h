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

#ifndef CLIENT_MESSAGE_H_
#define CLIENT_MESSAGE_H_

#include "message_base.h"

namespace engine {
namespace network {

class ClientMessage : public MessageBase {
public:
  ClientMessage(
      std::shared_ptr<std::queue<std::shared_ptr<lib::ByteStream>>> send_buffer,
      const int& type);
  virtual ~ClientMessage();

  void Send();

private:
  std::shared_ptr<std::queue<std::shared_ptr<lib::ByteStream>>> send_buffer_;
};

inline ClientMessage::ClientMessage(
      std::shared_ptr<std::queue<std::shared_ptr<lib::ByteStream>>> send_buffer,
      const int& type)
    : MessageBase(type), send_buffer_(send_buffer) {
  if (send_buffer_->empty()) {
    send_buffer_->push(std::make_shared<lib::ByteStream>());
  }
}

inline ClientMessage::~ClientMessage() {
}

inline void ClientMessage::Send() {
  if (buffer_->GetSize() > 128)
    throw std::out_of_range("Message is too big.");
  auto newest_stream = send_buffer_->back();

  // Do we have enough room?
  if (newest_stream->GetSize() + buffer_->GetSize() > 128) {
    send_buffer_->push(buffer_);
  } else {
    newest_stream->WriteStream(buffer_);
  }
}

} // end network namespace
} // end engine namespace

#endif // CLIENT_MESSAGE_H_