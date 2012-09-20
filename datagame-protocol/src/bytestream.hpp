#ifndef BYTESTREAM_HPP
#define BYTESTREAM_HPP

#include "types.hpp"
#include <string>
#include "assert.hpp"
#include "warning.hpp"

#define MAX_BUFFER 512

namespace dgp {
  class byteStream {
    dgpByte *m_pbStream;
    dgpByte *m_pbPosition;
    dgpUint m_nSize;
    dgpUint m_nStreamRefCount;

  public:
    byteStream();
    ~byteStream();

    dgpByte* getStream ();
    dgpUint getSize ();
    void setSize (dgpUint nSize);

    void unrefStream ();
    void clear ();

    // Read
    template <class T>
    T read ();
    dgpChar* readString ();

    // Write
    template <class T>
    void write (T value);
    void writeString (const dgpChar *value);
  };

  template <class T>
  T byteStream::read () {
    ASSERT_RETURN_VAL (m_pbStream, 0)

    dgpUint size = sizeof (T);
    m_nSize -= size;
    if (m_nSize > MAX_BUFFER) {
      WARNING_MESSAGE("Buffer overflow when reading bytestream.")
      return 0;
    }

    union unpack_t {
      dgpByte byte[sizeof (T)];
      T val;
    } unpack;

    for (dgpUint i = 0; i < sizeof (T); i++) {
      unpack.byte[i] = *m_pbPosition;
      m_pbPosition++;
    }
    
    T value = unpack.val;
    return value;
  }

  template <class T>
  void byteStream::write (T value) {
    ASSERT_RETURN (m_pbStream)

    m_nSize += sizeof (T);
    if (m_nSize > MAX_BUFFER) {
      WARNING_MESSAGE_FORMAT("Bytestream is too large for %d size.", m_nSize)
      return;
    }

    union pack_t {
      dgpByte byte[sizeof (T)];
      T val;
    } pack;

    pack.val = value;
    for (dgpUint i = 0; i < sizeof (T); i++) {
      *m_pbPosition = pack.byte[i];
      m_pbPosition++;
    }
  }
}

#endif // BYTESTREAM_HPP
