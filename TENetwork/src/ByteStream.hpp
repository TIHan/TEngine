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
  class IByteStream {
  public:
    virtual ~IByteStream () {};
    virtual TEbyte* GetCopyOfStream () = 0;
    virtual TEuint GetSize () = 0;
    virtual TEuint GetMaxSize () = 0;
    virtual void Clear () = 0;

    virtual TEchar* ReadString () = 0;
    virtual void WriteString (const TEchar *value) = 0;
    virtual void WriteStream (const TEbyte *pbStream, const TEuint nSize) = 0;

    template <class T>
    T Read () {}

    template <class T>
    void Write (const T value) {}
  };

  class ByteStream : public IByteStream {
    TEbyte *m_pbStream;
    TEbyte *m_pbReadPosition;
    TEbyte *m_pbWritePosition;
    TEuint m_nSize;
    TEuint m_nMaxSize;

  public:
    explicit ByteStream(const TEuint nMaxSize);
    ~ByteStream();

    TEbyte* GetCopyOfStream ();
    TEuint GetSize ();
    TEuint GetMaxSize ();
    void Clear ();

    TEchar* ReadString ();
    void WriteString (const TEchar *value);
    void WriteStream (const TEbyte *pbStream, const TEuint nSize);

    template <class T>
    T Read ();

    template <class T>
    void Write (const T value);
  };

  /*!
   *
   */
  template <class T>
  T ByteStream::Read () {
    TEuint size = sizeof (T);
    if (m_nSize - size > m_nMaxSize) {
      ERROR_MESSAGE("Overflow on reading.")
    }

    union unpack_t {
      TEbyte byte[sizeof (T)];
      T val;
    } unpack;

    for (TEuint i = 0; i < size; i++) {
      unpack.byte[i] = *m_pbReadPosition;
      m_pbReadPosition++;
    }

    m_nSize -= size;
    T value = unpack.val;
    return value;
  }

  /*!
   *
   */
  template <class T>
  void ByteStream::Write (const T value) {
    TEuint size = sizeof (T);
    if (m_nSize + size > m_nMaxSize) {
      ERROR_MESSAGE("Overflow on writing.")
      return;
    }

    union pack_t {
      TEbyte byte[sizeof (T)];
      T val;
    } pack;

    pack.val = value;
    for (TEuint i = 0; i < size; i++) {
      *m_pbWritePosition = pack.byte[i];
      m_pbWritePosition++;
    }
    m_nSize += size;
  }
}

#endif /* __BYTESTREAM_HPP_ */
