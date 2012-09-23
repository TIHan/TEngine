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

#include <string.h>
#include "Types.hpp"
#include "Messages.hpp"
#include "ByteStream.hpp"

namespace TE {
  /*!
   * \brief The constructor for the ByteStream that requires
   * a maximum size for the ByteStream buffer.
   *
   */
  ByteStream::ByteStream (const TEuint nMaxSize) {
    m_nMaxSize = nMaxSize;
    m_pbStream = new TEbyte[m_nMaxSize];
    m_pbReadPosition = m_pbStream;
    m_pbWritePosition = m_pbStream;
    m_nSize = 0;
  }

  /*!
   *
   */
  ByteStream::~ByteStream () {    
    if (m_nSize != 0) {
      WARNING_MESSAGE_FORMAT("Not all bytes were read. Please clear or read the remaining %d byte(s).", m_nSize)
    }
    delete [] m_pbStream;
  }

  TEbyte* ByteStream::GetCopyOfStream () {
    TEbyte *stream = new TEbyte[m_nMaxSize];
    memcpy (stream, m_pbStream, m_nMaxSize);
    return stream;
  }

  /*!
   *
   */
  TEuint ByteStream::GetSize () {
    return m_nSize;
  }

  /*!
   *
   */
  TEuint ByteStream::GetMaxSize () {
    return m_nMaxSize;
  }

  /*!
   *
   */
  void ByteStream::Clear () {
    delete [] m_pbStream;
    m_pbStream = new TEbyte[m_nMaxSize];
    m_pbReadPosition = m_pbStream;
    m_pbWritePosition = m_pbStream;
    m_nSize = 0;
  }

  /*!
   *
   */
  TEchar* ByteStream::ReadString () {    
    TEuint size = 1;
    for (TEbyte *p = m_pbReadPosition; *p != '\0'; p++) {
      size++;
    }

    if (m_nSize - size > m_nMaxSize) {
      ERROR_MESSAGE("Overflow on reading string.")
      return 0;
    }

    TEchar *value = new TEchar[size];
    for (TEuint i = 0; i < size; i++) {
      value[i] = *m_pbReadPosition;
      m_pbReadPosition++;
    }
    m_nSize -= size;
    return value;
  }

  /*!
   *
   */
  void ByteStream::WriteString (const TEchar *value) {
    TEuint size = strlen (value);
    if (m_nSize + size + 1 > m_nMaxSize) {
      ERROR_MESSAGE("Overflow on writing string.")
    }

    for (TEuint i = 0; i < size; i++) {
        *m_pbWritePosition = value[i];
        m_pbWritePosition++;
    }

    *m_pbWritePosition = '\0';
    m_pbWritePosition++;
    m_nSize += size + 1;
  }

  /*!
   *
   */
  void ByteStream::WriteStream (const TEbyte *pbStream, const TEuint nSize) {
    if (nSize + m_nSize > m_nMaxSize) {
      ERROR_MESSAGE("Overflow on writing stream.")
    }

    for (TEuint i = 0; i < nSize; i++) {
      *m_pbWritePosition = pbStream[i];
      m_pbWritePosition++;
    }
    m_nSize += nSize;
  }
}
