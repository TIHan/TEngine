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
      WARNING_MESSAGE_FORMAT("Not all bytes were read. Please clear or read the remaining %ld byte(s).", m_nSize)
    }
    delete [] m_pbStream;
  }

  dgpByte* byteStream::getStream () {
    ASSERT_RETURN_VAL (m_pbStream, 0)
    m_nStreamRefCount++;
    return m_pbStream;
  }

  size_t byteStream::getSize () {
    return m_nSize;
  }

  void byteStream::setSize (size_t nSize) {
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

  dgpByte byteStream::readByte () {
    ASSERT_RETURN_VAL (m_pbStream, 0)
    
    dgpByte value = *m_pbPosition;
    m_nSize -= sizeof (value);

    if (m_nSize > MAX_BUFFER) {
      WARNING_MESSAGE("Buffer overflow when reading bytestream.")
      return 0;
    }
    m_pbPosition += sizeof (dgpByte);
    return value;
  }

  void byteStream::writeByte (dgpByte value) {
    ASSERT_RETURN (m_pbStream)

    m_nSize += sizeof (value);
    if (m_nSize > MAX_BUFFER) {
      WARNING_MESSAGE_FORMAT("Bytestream is too large for %ld size.", m_nSize)
      return;
    }
    m_pbStream[m_nSize - 1] = value;
  }
}
