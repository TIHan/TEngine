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
    shared_ptr<vector<TEbyte>> m_pBuffer;
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
      priv(make_unique<PByteStream>()) {
    priv->m_pBuffer = make_shared<vector<TEbyte>>(nMaxSize);
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
    if (HasErrors()) {
      return 0;
    }

    TEuint size = sizeof(TEbyte);
    if (GetSize() - size > GetMaxSize()) {
      priv->m_bError = true;
      return 0;
    }

    TEbyte val = priv->m_pBuffer->data()[priv->m_iRead];
    priv->m_iRead++;
    return val;
  }

  /*!
   *
   */
  void ByteStream::WriteByte(const TEbyte byte) {
    if (HasErrors()) {
      return;
    }

    TEuint size = sizeof(TEbyte);
    if (GetSize() + size > GetMaxSize()) {
      priv->m_bError = true;
      return;
    }

    priv->m_pBuffer->insert(priv->m_pBuffer->end(), byte);
    priv->m_iWrite++;
  }

  /*!
   *
   */
  shared_ptr<vector<TEbyte>> ByteStream::GetBuffer() {
    if (GetSize() != 0) {
      return priv->m_pBuffer;
    } else {
      return nullptr;
    }
  }

  /*!
   *
   */
  TEuint ByteStream::GetSize() {
    return priv->m_pBuffer->size();
  }

  /*!
   *
   */
  TEuint ByteStream::GetMaxSize() {
    return priv->m_pBuffer->capacity();
  }

  /*!
   *
   */
  void ByteStream::Clear() {
    priv->m_pBuffer.get()->clear();
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
  string ByteStream::ReadString() {
    TEuint size = GetSize();
    shared_ptr<TEchar> val(new TEchar[size], default_delete<TEchar[]>());
    string sz;

    for (TEuint i = 0; i < size; i++) {
      val.get()[i] = ReadByte();
      if (val.get()[i] == '\0') {
        sz.assign(val.get());
        return sz;
      }
    }
    return String::Empty();
  }

  /*!
   *
   */
  void ByteStream::WriteString(const string sz) {
    TEuint size = (TEint)strlen(sz.c_str());

    for (TEuint i = 0; i < size; i++) {
      WriteByte(sz.c_str()[i]);
      if (i + 1 >= size) {
        WriteByte('\0');
      }
    }
  }

  /*!
   *
   */
  void ByteStream::WriteStream(const shared_ptr<vector<TEbyte>> pBuffer) {
    for (TEuint i = 0; i < pBuffer.get()->size(); i++) {
      WriteByte(pBuffer.get()->data()[i]);
    }
  }
}
