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

#ifndef __BYTESTREAM_HPP_
#define __BYTESTREAM_HPP_

#include "Types.hpp"
#include "Output.hpp"
#include "ByteSequence.hpp"

namespace TE {
  class PByteStream;
  class ByteStream : public IByteData {
    unique_ptr<PByteStream> priv;

  protected:
    unsigned char ReadByte();
    void WriteByte(const unsigned char& byte);

  public:
    explicit ByteStream(const int& nMaxSize);
    explicit ByteStream(shared_ptr<ByteSequence> pByteSequence);
    virtual ~ByteStream();

    int GetSize();
    int GetMaxSize();
    void Clear();
    bool HasErrors();

    virtual const unsigned char* GetRawByteData() const;
    virtual int GetByteDataSize() const;

    string ReadString();
    void WriteString(const string& sz);
    void WriteStream(const shared_ptr<const ByteSequence>& pBuffer);

    template <typename T>
    T Read();

    template <typename T>
    void Write(const T& val);
  };

  /*!
   *
   */
  template <typename T>
  T ByteStream::Read() {
    int size = sizeof(T);

    union unpack_t {
      unsigned char byte[sizeof(T)];
      T val;
    } unpack;

    for (int i = 0; i < size; i++) {
      unpack.byte[i] = ReadByte();
    }
    return unpack.val;
  }

  /*!
   *
   */
  template <typename T>
  void ByteStream::Write(const T& val) {
    int size = sizeof(T);

    union pack_t {
      unsigned char byte[sizeof(T)];
      T val;
    } pack;
    pack.val = val;

    for (int i = 0; i < size; i++) {
      WriteByte(pack.byte[i]);
    }
  }
}

#endif /* __BYTESTREAM_HPP_ */
