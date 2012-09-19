#include "types.hpp"
#include "assert.hpp"
#include "bytestream.hpp"

namespace dgp {
  byteStream::byteStream () {
    m_pbStream = new dgpByte[MAX_BUFFER];
    m_pbPosition = m_pbStream;
    m_nStreamRefCount = 0;
    m_nSize = 0;
  }

  byteStream::~byteStream () {
    assertReturn (m_pbStream && m_nStreamRefCount == 0)
    delete [] m_pbStream;
  }

  dgpByte* byteStream::getStream () {
    assertReturnVal (m_pbStream, 0)
    m_nStreamRefCount++;
    return m_pbStream;
  }

  size_t byteStream::getSize () {
    return m_nSize;
  }

  void byteStream::unrefStream () {
    assertReturn (m_nStreamRefCount > 0)
    m_nStreamRefCount--;
  }

  void byteStream::clear () {
    assertReturn (m_pbStream && m_nStreamRefCount == 0)
    delete [] m_pbStream;
    m_pbStream = new dgpByte[MAX_BUFFER];
    m_pbPosition = m_pbStream;
  }

  dgpByte byteStream::readByte () {
    assertReturnVal (m_pbStream, 0)
    
    dgpByte value = *m_pbPosition;
    m_pbPosition += sizeof (dgpByte);
    return value;
  }

  void byteStream::writeByte (dgpByte value) {
    assertReturn (m_pbStream)

    m_pbStream[m_nSize] = value;
    m_nSize += sizeof value;
  }
}
