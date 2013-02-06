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

#ifndef BYTESTREAM_H_
#define BYTESTREAM_H_

#include "common.h"

namespace engine {
namespace lib {

class ByteStream {
public:
  ByteStream();
  ByteStream(const ByteStream& copy);
  explicit ByteStream(int reserve);
  explicit ByteStream(const std::vector<uint8_t>& buffer);
  ByteStream(const std::vector<uint8_t>& buffer, int size);
  ByteStream(const ByteStream& stream, int start_position);

  int GetSize() const;
  void Clear();

  const uint8_t* GetRaw() const;

  std::string ReadString();
  void WriteString(const std::string& string);
  void WriteBuffer(const std::vector<uint8_t>& buffer);
  void WriteBuffer(const std::vector<uint8_t>& buffer, int size);
  void WriteStream(const ByteStream& stream);
  void WriteStream(const ByteStream& stream, int start_position);

  template <typename T>
  T Read();

  template <typename T>
  void Write(T value);

  uint8_t ReadByte();
  void WriteByte(uint8_t byte);

  bool CanRead() const;

  /* A+M */
  int read_position() const { return read_position_; }

private:
  void Init();

  std::vector<uint8_t> buffer_;
  int read_position_;
};

/*!
  *
  */
template <typename T>
T ByteStream::Read() {
  int size = sizeof(T);

  union unpack_t {
    uint8_t byte[sizeof(T)];
    T value;
  } unpack;

  for (int i = 0; i < size; ++i) {
    unpack.byte[i] = ReadByte();
  }
  return unpack.value;
}

/*!
  *
  */
template <typename T>
void ByteStream::Write(T value) {
  int size = sizeof(T);

  union pack_t {
    uint8_t byte[sizeof(T)];
    T value;
  } pack;
  pack.value = value;

  for (int i = 0; i < size; ++i) {
    WriteByte(pack.byte[i]);
  }
}

/*! \brief The constructor for the ByteStream.
  *
  */
inline ByteStream::ByteStream() {
  Init();
}

/*!
  *
  */
inline ByteStream::ByteStream(const ByteStream& copy) {
  buffer_ = copy.buffer_;
  read_position_ = copy.read_position_;
}

/*!
  *
  */
inline ByteStream::ByteStream(int reserve) {
  Init();
  buffer_.reserve(reserve);
}

/*!
  *
  */
inline ByteStream::ByteStream(const std::vector<uint8_t>& buffer) {
  Init();
  WriteBuffer(buffer);
}

/*!
  *
  */
inline ByteStream::ByteStream(const std::vector<uint8_t>& buffer, int size) {
  Init();
  WriteBuffer(buffer, size);
}

/*!
  *
  */
inline ByteStream::ByteStream(const ByteStream& stream, int start_position) {
  Init();
  WriteStream(stream, start_position);
}

/*!
  *
  */
inline void ByteStream::Init() {
  read_position_ = 0;
}

/*!
  *
  */
inline int ByteStream::GetSize() const {
  return static_cast<int>(buffer_.size());
}

/*!
  *
  */
inline void ByteStream::Clear() {
  buffer_.clear();
  read_position_ = 0;
}

/*!
  *
  */
inline const uint8_t* ByteStream::GetRaw() const {
  return buffer_.data();
}

/*!
  *
  */
inline std::string ByteStream::ReadString() {
  std::string string;
  for (int i = 0; i < GetSize(); ++i) {
    string.insert(string.end(), ReadByte());
    if (string.at(i) == '\0') {
      return string.data();
    }
  }
  return stdext::string::empty();
}

/*!
  *
  */
inline void ByteStream::WriteString(const std::string& string) {
  int size = static_cast<int>(string.length());

  for (int i = 0; i < size; ++i) {
    WriteByte(string.data()[i]);
    if (i + 1 >= size) {
      WriteByte('\0');
    }
  }
}

/*!
  *
  */
inline void ByteStream::WriteBuffer(const std::vector<uint8_t>& buffer) {
  buffer_.reserve(buffer_.size() + buffer.size());
  for (int i = 0; i < static_cast<int>(buffer.size()); ++i) {
    WriteByte(buffer.at(i));
  }
}

/*!
  *
  */
inline void ByteStream::WriteBuffer(const std::vector<uint8_t>& buffer,
                                    int size) {
  buffer_.reserve(buffer_.size() + size);
  for (int i = 0; i < static_cast<int>(size); ++i) {
    WriteByte(buffer.at(i));
  }
}

/*!
  *
  */
inline void ByteStream::WriteStream(const ByteStream& stream) {
  for (int i = 0; i < stream.GetSize(); ++i) {
    WriteByte(stream.buffer_.at(i));
  }
}

/*!
  *
  */
inline void ByteStream::WriteStream(const ByteStream& stream, int start_position) {
  for (int i = start_position; i < stream.GetSize(); ++i) {
    WriteByte(stream.buffer_.at(i));
  }
}

/*!
  *
  */
inline uint8_t ByteStream::ReadByte() {
  return buffer_.at(read_position_++);
}

/*!
  *
  */
inline void ByteStream::WriteByte(uint8_t byte) {
  buffer_.push_back(byte);
}

/*!
  *
  */
inline bool ByteStream::CanRead() const {
  return read_position_ < GetSize();
}

} // end lib namespace
} // end engine namespace

#endif // BYTESTREAM_H_
