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

#ifndef MESSAGE_BASE_H_
#define MESSAGE_BASE_H_

#include <engine_lib.h>

namespace engine {
namespace network {

#define MTU 1400

typedef std::queue<std::shared_ptr<lib::ByteStream>> SendQueue;

class MessageBase {
public:
  virtual ~MessageBase();

  void WriteString(const std::string& string);

  template <typename T>
  void Write(const T& value);

  /* Accessors / Mutators */
  int type() const;

protected:
  MessageBase(const int& type);

  std::shared_ptr<lib::ByteStream> buffer_;
  int type_;
};

inline MessageBase::MessageBase(const int& type)
    : buffer_(std::make_shared<lib::ByteStream>()) {;
  if (type < 0) throw std::out_of_range("type is below 0.");

  type_ = type;
  buffer_->WriteByte(type);
}

inline MessageBase::~MessageBase() {
}

inline void MessageBase::WriteString(const std::string& string) {
  buffer_->WriteString(string);
}

template <typename T>
inline void MessageBase::Write(const T& value) {
  buffer_->Write<T>(value);
}

inline int MessageBase::type() const {
  return type_;
}

} // end network namespace
} // end engine namespace

#endif // MESSAGE_BASE_H_