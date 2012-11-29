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

#ifndef SEND_MESSAGE_H_
#define SEND_MESSAGE_H_

#include "message_interface.h"

namespace engine {
namespace network {

class SendMessage : public virtual MessageInterface {
public:
  SendMessage(std::shared_ptr<lib::ByteStream> sendStream, const int& type);
  virtual ~SendMessage();

  void WriteString(const std::string& string);

  template <typename T>
  void Write(const T& value);

  void Send();

  /* Accessors / Mutators */
  virtual int type() const;

private:
  int type_;
  std::shared_ptr<lib::ByteStream> byte_stream_;
  std::shared_ptr<lib::ByteStream> send_stream_;
};

template <typename T>
void SendMessage::Write(const T& value) {
  byte_stream_->Write<T>(value);
}

} // end network namespace
} // end engine namespace

#endif // SEND_MESSAGE_H_
