/*
  Copyright (c) 2012, William F. Smith
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

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

#include "bytestream.h"

namespace engine {
namespace lib {

class ByteStreamImpl {
public:
  std::vector<unsigned char> buffer_;
  int read_position_;
};

/*!
  * \brief The constructor for the ByteStream that requires
  * a maximum size for the ByteStream buffer.
  *
  */
ByteStream::ByteStream(const int& max_size) :
    impl(make_unique<ByteStreamImpl>()) {
  impl->buffer_.reserve(max_size);
  impl->read_position_ = 0;
}

/*!
  *
  */
ByteStream::~ByteStream() {
}

/*!
  *
  */
unsigned char ByteStream::ReadByte() {
  unsigned char value = impl->buffer_.at(impl->read_position_);
  impl->read_position_++;
  return value;
}

/*!
  *
  */
void ByteStream::WriteByte(const unsigned char& byte) {
  impl->buffer_.push_back(byte);
}

/*!
  *
  */
int ByteStream::GetSize() const {
  return impl->buffer_.size();
}

/*!
  *
  */
int ByteStream::GetMaxSize() const {
  return impl->buffer_.capacity();
}

/*!
  *
  */
void ByteStream::Reset() {
  impl->buffer_.clear();
  impl->read_position_ = 0;
}

/*!
  *
  */
const unsigned char* ByteStream::GetRaw() const {
  return impl->buffer_.data();
}

/*!
  *
  */
std::string ByteStream::ReadString() {
  std::string string;
  for (int i = 0; i < GetSize(); i++) {
    string.insert(string.end(), ReadByte());
    if (string.data()[i] == '\0') {
      return string.data();
    }
  }
  return EmptyString();
}

/*!
  *
  */
void ByteStream::WriteString(const std::string& string) {
  int size = static_cast<int>(string.length());

  for (int i = 0; i < size; i++) {
    WriteByte(string.data()[i]);
    if (i + 1 >= size) {
      WriteByte('\0');
    }
  }
}

/*!
  *
  */
void ByteStream::WriteStream(const std::vector<unsigned char>& buffer) {
  for (int i = 0; i < static_cast<int>(buffer.size()); i++) {
    WriteByte(buffer.at(i));
  }
}

} // end lib namespace
} // end engine namespace
