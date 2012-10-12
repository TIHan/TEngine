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
  class PByteStream : public IByteStream {
    shared_ptr<TEbyte> m_pbStream;
    TEuint m_nSize;
    TEuint m_nMaxSize;
    TEuint m_iRead;
    TEuint m_iWrite;
    bool m_bError;

  public:
    explicit PByteStream(const TEuint nMaxSize);
    ~PByteStream();

    shared_ptr<TEbyte> GetStream ();
    TEuint GetSize ();
    TEuint GetMaxSize ();
    void Clear ();
    bool HasError ();

    TEbyte ReadByte ();
    void WriteByte (const TEbyte byte);
  };

  /*!
   *
   */
  TEchar* IByteStream::ReadString () {
    TEuint size = GetSize ();
    TEchar *val = new TEchar[size];

    for (TEuint i = 0; i < size; i++) {
      if (HasError ()) {
        delete [] val;
        return 0;
      }
      val[i] = ReadByte ();        
      if (val[i] == '\0') {
        return val;
      }
    }
    delete [] val;
    return 0;
  }

  /*!
   *
   */
  void IByteStream::WriteString (const TEchar *sz) {
    TEuint size = (TEint)strlen (sz);

    for (TEuint i = 0; i < size; i++) {
      if (HasError ()) {
        return;
      }
      WriteByte (sz[i]);
      if (i + 1 >= size) {
        WriteByte ('\0');
      }
    }
  }

  /*!
   *
   */
  void IByteStream::WriteStream (const TEbyte *pbStream, const TEuint nSize) {
    for (TEuint i = 0; i < nSize; i++) {
      if (HasError ()) {
        return;
      }
      WriteByte (pbStream[i]);
    }
  }

  /*!
   * \brief The constructor for the ByteStream that requires
   * a maximum size for the ByteStream buffer.
   *
   */
  PByteStream::PByteStream (const TEuint nMaxSize) :
    m_pbStream (new TEbyte[nMaxSize], default_delete<TEbyte[]> ()) {
    m_nMaxSize = nMaxSize;
    m_nSize = 0;
    m_bError = false;
    m_iRead = 0;
    m_iWrite = 0;
  }

  /****************************************************************************************************************************
  *****************************************************************************************************************************
  *****************************************************************************************************************************
  ****************************************************************************************************************************/

  /*!
   *
   */
  PByteStream::~PByteStream () {
  }

  /*!
   *
   */
  shared_ptr<TEbyte> PByteStream::GetStream () {
    return m_pbStream;
  }

  /*!
   *
   */
  TEuint PByteStream::GetSize () {
    return m_nSize;
  }

  /*!
   *
   */
  TEuint PByteStream::GetMaxSize () {
    return m_nMaxSize;
  }

  /*!
   *
   */
  void PByteStream::Clear () {
    m_pbStream.reset (new TEbyte[m_nMaxSize], default_delete<TEbyte[]> ());
    m_nSize = 0;
    m_bError = false;
    m_iRead = 0;
    m_iWrite = 0;
  }

  /*!
   *
   */
  bool PByteStream::HasError () {
    return m_bError;
  }

  /*!
   *
   */
  TEbyte PByteStream::ReadByte () {
    TEuint size = sizeof (TEbyte);

    if (m_nSize - size > m_nMaxSize) {
      m_bError = true;
      return 0;
    }

    TEbyte val = m_pbStream.get ()[m_iRead];
    m_iRead++;
    m_nSize -= size;
    return val;
  }

  /*!
   *
   */
  void PByteStream::WriteByte (const TEbyte byte) {
    TEuint size = sizeof (TEbyte);

    if (m_nSize + size > m_nMaxSize) {
      m_bError = true;
      return;
    }

    m_pbStream.get ()[m_iWrite] = byte;
    m_iWrite++;
    m_nSize += size;
  }

  /****************************************************************************************************************************
  *****************************************************************************************************************************
  *****************************************************************************************************************************
  ****************************************************************************************************************************/

  ByteStream::ByteStream (const TEuint nMaxSize) :
    priv (new PByteStream (nMaxSize)) {
  }

  ByteStream::~ByteStream () {
  }

  shared_ptr<TEbyte> ByteStream::GetStream () {
    return priv->GetStream ();
  }

  TEuint ByteStream::GetSize () {
    return priv->GetSize ();
  }

  TEuint ByteStream::GetMaxSize () {
    return priv->GetMaxSize ();
  }

  void ByteStream::Clear () {
    priv->Clear ();
  }

  bool ByteStream::HasError () {
    return priv->HasError ();
  }

  TEbyte ByteStream::ReadByte () {
    return priv->ReadByte ();
  }

  void ByteStream::WriteByte (const TEbyte byte) {
    priv->WriteByte (byte);
  }
}
