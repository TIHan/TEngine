#include <string.h>
#include "Types.hpp"
#include "Messages.hpp"
#include "ByteStream.hpp"

namespace TE {
  ByteStream::ByteStream () {
    m_pbStream = new TEbyte[MAX_BUFFER];
    m_pbPosition = m_pbStream;
    m_nStreamRefCount = 0;
    m_nSize = 0;
  }

  ByteStream::~ByteStream () {    
    if (m_nSize != 0) {
      WARNING_MESSAGE_FORMAT("Not all bytes were read. Please clear or read the remaining %d byte(s).", m_nSize)
    }
    delete [] m_pbStream;
  }

  TEbyte* ByteStream::GetStream () {
    m_nStreamRefCount++;
    return m_pbStream;
  }

  TEuint ByteStream::GetSize () {
    return m_nSize;
  }

  void ByteStream::SetSize (TEuint nSize) {
    if (nSize > MAX_BUFFER) {
      WARNING_MESSAGE("Size is too large.")
      return;
    }
    m_nSize = nSize;
  }

  void ByteStream::UnrefStream () {
    m_nStreamRefCount--;
  }

  void ByteStream::Clear () {
    delete [] m_pbStream;
    m_pbStream = new TEbyte[MAX_BUFFER];
    m_pbPosition = m_pbStream;
    m_nSize = 0;
  }

  TEchar* ByteStream::ReadString () {    
    TEuint size = 1;
    for (TEbyte *p = m_pbPosition; *p != '\0'; p++) {
      size++;
    }

    if (m_nSize - size > MAX_BUFFER) {
      WARNING_MESSAGE("Overflow on reading.")
      return 0;
    }

    TEchar *value = new TEchar[size];
    for (TEuint i = 0; i < size; i++) {
      value[i] = *m_pbPosition;
      m_pbPosition++;
    }
    m_nSize -= size;
    return value;
  }

  void ByteStream::WriteString (const TEchar *value) {
    TEuint size = strlen (value);
    if (m_nSize + size + 1 > MAX_BUFFER) {
      WARNING_MESSAGE("Overflow on writing.")
      return;
    }

    for (TEuint i = 0; i < size; i++) {
        *m_pbPosition = value[i];
        m_pbPosition++;
    }

    *m_pbPosition = '\0';
    m_pbPosition++;
    m_nSize += size + 1;
  }
}
