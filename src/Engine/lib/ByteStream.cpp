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
    shared_ptr<ByteSequence> m_pBuffer;
    TEint m_iRead;
    TEint m_iWrite;
    TEboolean m_bError;
  };

  /*!
   * \brief The constructor for the ByteStream that requires
   * a maximum size for the ByteStream buffer.
   *
   */
  ByteStream::ByteStream(const TEint& nMaxSize) :
      priv(make_unique<PByteStream>()) {
    priv->m_pBuffer = make_shared<ByteSequence>();
    priv->m_pBuffer->SetCapacity(nMaxSize);
    priv->m_bError = false;
    priv->m_iRead = 0;
    priv->m_iWrite = 0;
  }

  /*!
   *
   */
  ByteStream::ByteStream(const TEint& nMaxSize, shared_ptr<ByteSequence> pByteSequence) :
      priv(make_unique<PByteStream>()) {
    priv->m_pBuffer.swap(pByteSequence);
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

    TEint size = sizeof(TEbyte);
    if (GetSize() - size > GetMaxSize()) {
      priv->m_bError = true;
      return 0;
    }

    TEbyte val = priv->m_pBuffer->GetRawData()[priv->m_iRead];
    priv->m_iRead++;
    return val;
  }

  /*!
   *
   */
  void ByteStream::WriteByte(const TEbyte& byte) {
    if (HasErrors()) {
      return;
    }

    TEint size = sizeof(TEbyte);
    if (GetSize() + size > GetMaxSize()) {
      priv->m_bError = true;
      return;
    }

    priv->m_pBuffer->Add(byte);
    priv->m_iWrite++;
  }

  /*!
   *
   */
  TEint ByteStream::GetSize() {
    return priv->m_pBuffer->GetSize();
  }

  /*!
   *
   */
  TEint ByteStream::GetMaxSize() {
    return priv->m_pBuffer->GetCapacity();
  }

  /*!
   *
   */
  void ByteStream::Clear() {
    priv->m_pBuffer->Clear();
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
  const TEbyte* ByteStream::GetRawByteData() const {
    return priv->m_pBuffer->GetRawByteData();
  }

  /*!
   *
   */
  TEint ByteStream::GetByteDataSize() const {
    return priv->m_pBuffer->GetByteDataSize();
  }

  /*!
   *
   */
  string ByteStream::ReadString() {
    TEint size = GetSize();
    string sz;

    for (TEint i = 0; i < size; i++) {
      sz.insert(sz.end(), ReadByte());
      if (sz.data()[i] == '\0') {
        return sz.data();
      }
    }
    return String::Empty();
  }

  /*!
   *
   */
  void ByteStream::WriteString(const string& sz) {
    TEint size = sz.length();

    for (TEint i = 0; i < size; i++) {
      WriteByte(sz.data()[i]);
      if (i + 1 >= size) {
        WriteByte('\0');
      }
    }
  }

  /*!
   *
   */
  void ByteStream::WriteStream(const shared_ptr<const ByteSequence>& pBuffer) {
    for (TEint i = 0; i < pBuffer->GetSize(); i++) {
      WriteByte(pBuffer.get()->GetRawData()[i]);
    }
  }
}
