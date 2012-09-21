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

  TEbyte* ByteStream::getStream () {
    m_nStreamRefCount++;
    return m_pbStream;
  }

  TEuint ByteStream::getSize () {
    return m_nSize;
  }

  void ByteStream::setSize (TEuint nSize) {
    if (nSize > MAX_BUFFER) {
      WARNING_MESSAGE("Size is too large.")
      return;
    }
    m_nSize = nSize;
  }

  void ByteStream::unrefStream () {
    m_nStreamRefCount--;
  }

  void ByteStream::clear () {
    delete [] m_pbStream;
    m_pbStream = new TEbyte[MAX_BUFFER];
    m_pbPosition = m_pbStream;
    m_nSize = 0;
  }

  TEchar* ByteStream::readString () {    
    TEuint size = 1;
    for (TEbyte *p = m_pbPosition; *p != '\0'; p++) {
      size++;
    }

    m_nSize -= size;
    if (m_nSize > MAX_BUFFER) {
      WARNING_MESSAGE("Buffer overflow when reading ByteStream.")
      return 0;
    }

    TEchar *value = new TEchar[size];
    for (TEuint i = 0; i < size; i++) {
      value[i] = *m_pbPosition;
      m_pbPosition++;
    }
    return value;
  }

  void ByteStream::writeString (const TEchar *value) {
    TEuint size = strlen (value);
    m_nSize += size + 1;
    if (m_nSize > MAX_BUFFER) {
      WARNING_MESSAGE_FORMAT("ByteStream is too large for %d size.", m_nSize)
      return;
    }

    for (TEuint i = 0; i < size; i++) {
        *m_pbPosition = value[i];
        m_pbPosition++;
    }

    *m_pbPosition = '\0';
    m_pbPosition++;
  }
}
