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
#include "Messages.hpp"

namespace TE {
  class AByteStream {
  public:
    virtual ~AByteStream() {}

    virtual shared_ptr<TEbyte> GetStream () = 0;
    virtual TEuint GetSize() = 0;
    virtual TEuint GetMaxSize() = 0;
    virtual void Clear() = 0;
    virtual bool HasError() = 0;

    virtual TEbyte ReadByte() = 0;
    virtual void WriteByte(const TEbyte byte) = 0;
    shared_ptr<TEchar> ReadString();
    void WriteString(const string sz);
    void WriteStream(const shared_ptr<TEbyte> pbStream, const TEuint nSize);

    /*!
     *
     */
    template <class T>
    T Read() {
      TEuint size = sizeof(T);

      union unpack_t {
        TEbyte byte[sizeof(T)];
        T val;
      } unpack;

      for (TEuint i = 0; i < size; i++) {
        if (HasError()) {
          return 0;
        }
        unpack.byte[i] = ReadByte();
      }
      return unpack.val;
    }

    /*!
     *
     */
    template <class T>
    void Write (const T val) {
      TEuint size = sizeof(T);

      union pack_t {
        TEbyte byte[sizeof(T)];
        T val;
      } pack;
      pack.val = val;

      for (TEuint i = 0; i < size; i++) {
        if (HasError()) {
          return;
        }
        WriteByte (pack.byte[i]);
      }
    }
  };

  class PByteStream;
  class ByteStream : public AByteStream {
    unique_ptr<PByteStream> priv;

  public:
    explicit ByteStream(const TEuint nMaxSize);
    ~ByteStream();

    shared_ptr<TEbyte> GetStream();
    TEuint GetSize();
    TEuint GetMaxSize();
    void Clear();
    bool HasError();

    TEbyte ReadByte();
    void WriteByte(const TEbyte byte);
  };
}

#endif /* __BYTESTREAM_HPP_ */
