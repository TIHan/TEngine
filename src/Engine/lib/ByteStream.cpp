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
    int m_iRead;
    bool m_bError;
  };

  /*!
   * \brief The constructor for the ByteStream that requires
   * a maximum size for the ByteStream buffer.
   *
   */
  ByteStream::ByteStream(const int& nMaxSize) :
      priv(make_unique<PByteStream>()) {
    priv->m_pBuffer = make_shared<ByteSequence>();
    priv->m_pBuffer->SetCapacity(nMaxSize);
    priv->m_bError = false;
    priv->m_iRead = 0;
  }

  /*!
   *
   */
  ByteStream::ByteStream(shared_ptr<ByteSequence> pByteSequence) :
      priv(make_unique<PByteStream>()) {
    priv->m_pBuffer.swap(pByteSequence);
    priv->m_bError = false;
    priv->m_iRead = 0;
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
    if (HasErrors()) {
      return 0;
    }

    int size = static_cast<int>(sizeof(unsigned char));
    if (GetSize() - size > GetMaxSize()) {
      priv->m_bError = true;
      return 0;
    }

    unsigned char val = priv->m_pBuffer->GetRawData()[priv->m_iRead];
    priv->m_iRead++;
    return val;
  }

  /*!
   *
   */
  void ByteStream::WriteByte(const unsigned char& byte) {
    if (HasErrors()) {
      return;
    }

    int size = static_cast<int>(sizeof(unsigned char));
    if (GetSize() + size > GetMaxSize()) {
      priv->m_bError = true;
      return;
    }

    priv->m_pBuffer->Add(byte);
  }

  /*!
   *
   */
  int ByteStream::GetSize() {
    return priv->m_pBuffer->GetSize();
  }

  /*!
   *
   */
  int ByteStream::GetMaxSize() {
    return priv->m_pBuffer->GetCapacity();
  }

  /*!
   *
   */
  void ByteStream::Clear() {
    priv->m_pBuffer->Clear();
    priv->m_bError = false;
    priv->m_iRead = 0;
  }

  /*!
   *
   */
  bool ByteStream::HasErrors() {
    return priv->m_bError;
  }

  /*!
   *
   */
  const unsigned char* ByteStream::GetRawByteData() const {
    return priv->m_pBuffer->GetRawByteData();
  }

  /*!
   *
   */
  int ByteStream::GetByteDataSize() const {
    return priv->m_pBuffer->GetByteDataSize();
  }

  /*!
   *
   */
  string ByteStream::ReadString() {
    int size = GetSize();
    string sz;

    for (int i = 0; i < size; i++) {
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
    int size = static_cast<int>(sz.length());

    for (int i = 0; i < size; i++) {
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
    for (int i = 0; i < pBuffer->GetSize(); i++) {
      WriteByte(pBuffer.get()->GetRawData()[i]);
    }
  }
}
