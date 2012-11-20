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

#ifndef BYTESTREAM_H_
#define BYTESTREAM_H_

#include "common.h"

namespace engine {
namespace lib {

class ByteStreamImpl;
class ByteStream {
  std::unique_ptr<ByteStreamImpl> impl;

protected:
  unsigned char ReadByte();
  void WriteByte(const unsigned char& byte);

public:
  explicit ByteStream(const int& max_size);
  virtual ~ByteStream();

  int GetSize() const;
  int GetMaxSize() const;
  void Reset();

  const unsigned char* GetRaw() const;

  std::string ReadString();
  void WriteString(const std::string& string);
  void WriteStream(const std::vector<unsigned char>& buffer);

  template <typename T>
  T Read();

  template <typename T>
  void Write(const T& value);
};

/*!
  *
  */
template <typename T>
T ByteStream::Read() {
  int size = sizeof(T);

  union unpack_t {
    unsigned char byte[sizeof(T)];
    T value;
  } unpack;

  for (int i = 0; i < size; i++) {
    unpack.byte[i] = ReadByte();
  }
  return unpack.value;
}

/*!
  *
  */
template <typename T>
void ByteStream::Write(const T& value) {
  int size = sizeof(T);

  union pack_t {
    unsigned char byte[sizeof(T)];
    T value;
  } pack;
  pack.value = value;

  for (int i = 0; i < size; i++) {
    WriteByte(pack.byte[i]);
  }
}

} // end lib namespace
} // end engine namespace

#endif // BYTESTREAM_H_