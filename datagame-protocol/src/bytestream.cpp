#include "types.hpp"
#include "assert.hpp"
#include "error.hpp"
#include "warning.hpp"
#include "bytestream.hpp"

namespace dgp {
  byteStream::byteStream () {
    m_pbStream = new dgpByte[MAX_BUFFER];
    m_pbPosition = m_pbStream;
    m_nStreamRefCount = 0;
    m_nSize = 0;
  }

  byteStream::~byteStream () {
    ASSERT_RETURN (m_pbStream && m_nStreamRefCount == 0)
    
    if (m_nSize != 0) {
      WARNING_MESSAGE_FORMAT("Not all bytes were read. Please clear or read the remaining %d byte(s).", m_nSize)
    }
    delete [] m_pbStream;
  }

  dgpByte* byteStream::getStream () {
    ASSERT_RETURN_VAL (m_pbStream, 0)
    m_nStreamRefCount++;
    return m_pbStream;
  }

  dgpUint byteStream::getSize () {
    return m_nSize;
  }

  void byteStream::setSize (dgpUint nSize) {
    if (nSize > MAX_BUFFER) {
      WARNING_MESSAGE("Size is too large.")
      return;
    }
    m_nSize = nSize;
  }

  void byteStream::unrefStream () {
    ASSERT_RETURN (m_nStreamRefCount > 0)
    m_nStreamRefCount--;
  }

  void byteStream::clear () {
    ASSERT_RETURN (m_pbStream && m_nStreamRefCount == 0)
    delete [] m_pbStream;
    m_pbStream = new dgpByte[MAX_BUFFER];
    m_pbPosition = m_pbStream;
    m_nSize = 0;
  }

  dgpChar* byteStream::readString () {
    ASSERT_RETURN_VAL (m_pbStream, 0)
    
    dgpUint size = 1;
    for (dgpByte *p = m_pbPosition; *p != '\0'; p++) {
      size++;
    }

    m_nSize -= size;
    if (m_nSize > MAX_BUFFER) {
      WARNING_MESSAGE("Buffer overflow when reading bytestream.")
      return 0;
    }

    dgpChar *value = new dgpChar[size];
    for (dgpUint i = 0; i < size; i++) {
      value[i] = *m_pbPosition;
      m_pbPosition++;
    }
    return value;
  }

  void byteStream::writeString (const dgpChar *value) {
    ASSERT_RETURN (m_pbStream)

    dgpUint size = strlen (value);
    m_nSize += size;
    if (m_nSize > MAX_BUFFER) {
      WARNING_MESSAGE_FORMAT("Bytestream is too large for %d size.", m_nSize)
      return;
    }

    for (dgpUint i = 0; i < size; i++) {
        *m_pbPosition = value[i];
        m_pbPosition++;
    }

    *m_pbPosition = '\0';
    m_pbPosition++;
    m_nSize++;
  }
}
