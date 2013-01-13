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

#ifndef RECEIVE_MESSAGE_H_
#define RECEIVE_MESSAGE_H_

#include <engine_lib.h>

namespace engine {
namespace network {

enum ReservedServerMessage {
  kAckClientConnect = 248,
  kAckClientDisconnect = 250,
  kAckClientHeartbeat = 249
};

enum ReservedClientMessage {
  kConnect = 248,
  kDisconnect = 249,
  kHeartbeat = 250
};

class ReceiveMessage {
public:
  ReceiveMessage(std::shared_ptr<ByteStream> receive_stream,
                 const int& type);
  virtual ~ReceiveMessage();

  std::string ReadString();

  template <typename T>
  T Read();

  /* Accessors / Mutators */
  virtual int type() const;

private:
  int type_;
  std::shared_ptr<ByteStream> receive_stream_;
};

inline ReceiveMessage::ReceiveMessage(std::shared_ptr<ByteStream> receive_stream,
                               const int& type) {
  if (!receive_stream) throw std::invalid_argument("receiveStream is null.");
  if (type < 0) throw std::out_of_range("type is below 0.");

  type_ = type;
  receive_stream_ = receive_stream;
}

inline ReceiveMessage::~ReceiveMessage() {
}

inline std::string ReceiveMessage::ReadString() {
  return receive_stream_->ReadString();
}

template <typename T>
inline T ReceiveMessage::Read() {
  return receive_stream_->Read<T>();
}

inline int ReceiveMessage::type() const {
  return type_;
}

} // end network namespace
} // end engine namespace

#endif // RECEIVE_MESSAGE_H_
