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

#include "ByteStream.hpp"

namespace TE {
  class PByteStream {
		public:
    shared_ptr<TEbyte> m_pBuffer;
    TEuint m_nSize;
    TEuint m_nMaxSize;
    TEuint m_iRead;
    TEuint m_iWrite;
    TEboolean m_bError;
  };

  /*!
   * \brief The constructor for the ByteStream that requires
   * a maximum size for the ByteStream buffer.
   *
   */
  ByteStream::ByteStream(const TEuint nMaxSize) :
      priv(new PByteStream()) {
    priv->m_pBuffer.reset(new TEbyte[nMaxSize], default_delete<TEbyte[]>());
    priv->m_nMaxSize = nMaxSize;
    priv->m_nSize = 0;
    priv->m_bError = false;
    priv->m_iRead = 0;
    priv->m_iWrite = 0;
  }

  /*!
   *
   */
  ByteStream::~ByteStream() {
  }

  /*!
   *
   */
  TEbyte ByteStream::ReadByte() {
    TEuint size = sizeof(TEbyte);

    if (priv->m_nSize - size > priv->m_nMaxSize) {
      priv->m_bError = true;
      return 0;
    }

    TEbyte val = priv->m_pBuffer.get()[priv->m_iRead];
    priv->m_iRead++;
    priv->m_nSize -= size;
    return val;
  }

  /*!
   *
   */
  void ByteStream::WriteByte(const TEbyte byte) {
    TEuint size = sizeof(TEbyte);

    if (priv->m_nSize + size > priv->m_nMaxSize) {
      priv->m_bError = true;
      return;
    }

    priv->m_pBuffer.get()[priv->m_iWrite] = byte;
    priv->m_iWrite++;
    priv->m_nSize += size;
  }

  /*!
   *
   */
  shared_ptr<TEbyte> ByteStream::GetBuffer() {
    return priv->m_pBuffer;
  }

  /*!
   *
   */
  TEuint ByteStream::GetSize() {
    return priv->m_nSize;
  }

  /*!
   *
   */
  TEuint ByteStream::GetMaxSize() {
    return priv->m_nMaxSize;
  }

  /*!
   *
   */
  void ByteStream::Clear() {
    priv->m_pBuffer.reset(new TEbyte[priv->m_nMaxSize], default_delete<TEbyte[]>());
    priv->m_nSize = 0;
    priv->m_bError = false;
    priv->m_iRead = 0;
    priv->m_iWrite = 0;
  }

  /*!
   *
   */
  TEboolean ByteStream::HasErrors() {
    return priv->m_bError;
  }

  /*!
   *
   */
  shared_ptr<TEchar> ByteStream::ReadString() {
    TEuint size = GetSize();
    shared_ptr<TEchar> val(new TEchar[size], default_delete<TEchar[]>());

    for (TEuint i = 0; i < size; i++) {
      if (HasErrors()) {
        return 0;
      }
      val.get()[i] = ReadByte();
      if (val.get()[i] == '\0') {
        return val;
      }
    }
    return 0;
  }

  /*!
   *
   */
  void ByteStream::WriteString(const string sz) {
    TEuint size = (TEint)strlen(sz.c_str());

    for (TEuint i = 0; i < size; i++) {
      if (HasErrors()) {
        return;
      }
      WriteByte(sz.c_str()[i]);
      if (i + 1 >= size) {
        WriteByte('\0');
      }
    }
  }

  /*!
   *
   */
  void ByteStream::WriteStream(const shared_ptr<TEbyte> pbStream,
      const TEuint nSize) {
    for (TEuint i = 0; i < nSize; i++) {
      if (HasErrors()) {
        return;
      }
      WriteByte(pbStream.get()[i]);
    }
  }

  /*!
    *
    */
  template <class T>
  T ByteStream::Read() {
    TEuint size = sizeof(T);

    union unpack_t {
      TEbyte byte[sizeof(T)];
      T val;
    } unpack;

    for (TEuint i = 0; i < size; i++) {
      if (HasErrors()) {
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
  void ByteStream::Write(const T val) {
    TEuint size = sizeof(T);

    union pack_t {
      TEbyte byte[sizeof(T)];
      T val;
    } pack;
    pack.val = val;

    for (TEuint i = 0; i < size; i++) {
      if (HasErrors()) {
        return;
      }
      WriteByte(pack.byte[i]);
    }
  }
}
