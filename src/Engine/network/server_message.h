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

#ifndef SERVER_MESSAGE_H_
#define SERVER_MESSAGE_H_

#include "message_interface.h"

namespace engine {
namespace network {

class ServerMessage : public virtual MessageInterface {
public:
  ServerMessage(std::shared_ptr<lib::ByteStream> send_stream, const int& type);
  virtual ~ServerMessage();

  void WriteString(const std::string& string);

  template <typename T>
  void Write(const T& value);

  void Send();

  /* Accessors / Mutators */
  virtual int type() const;

private:
  std::shared_ptr<lib::ByteStream> buffer_;
  std::shared_ptr<lib::ByteStream> send_stream_;
  int type_;
};

inline ServerMessage::ServerMessage(
    std::shared_ptr<lib::ByteStream> send_stream, const int& type)
    : buffer_(std::make_shared<lib::ByteStream>()) {
  if (!send_stream) throw std::invalid_argument("send_stream is null.");
  if (type < 0) throw std::out_of_range("type is below 0.");

  type_ = type;
  send_stream_ = send_stream;
  buffer_->WriteByte(type);
}

inline ServerMessage::~ServerMessage() {
}

inline void ServerMessage::WriteString(const std::string& string) {
  buffer_->WriteString(string);
}

template <typename T>
inline void ServerMessage::Write(const T& value) {
  buffer_->Write<T>(value);
}

inline void ServerMessage::Send() {
  send_stream_->WriteStream(buffer_);
}

inline int ServerMessage::type() const {
  return type_;
}

} // end network namespace
} // end engine namespace

#endif // SERVER_MESSAGE_H_
