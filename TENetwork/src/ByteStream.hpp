#ifndef __BYTESTREAM_HPP_
#define __BYTESTREAM_HPP_

#include "Types.hpp"
#include "Messages.hpp"

#define MAX_BUFFER 512

namespace TE {
  class ByteStream {
    TEbyte *m_pbStream;
    TEbyte *m_pbPosition;
    TEuint m_nSize;
    TEuint m_nStreamRefCount;

  public:
    ByteStream();
    ~ByteStream();

    TEbyte* getStream ();
    TEuint getSize ();
    void setSize (TEuint nSize);

    void unrefStream ();
    void clear ();

    // Read
    template <class T>
    T read ();
    TEchar* readString ();

    // Write
    template <class T>
    void write (T value);
    void writeString (const TEchar *value);
  };

  template <class T>
  T ByteStream::read () {
    TEuint size = sizeof (T);
    if (m_nSize - size > MAX_BUFFER) {
      WARNING_MESSAGE("Overflow on reading.")
      return NULL;
    }

    union unpack_t {
      TEbyte byte[sizeof (T)];
      T val;
    } unpack;

    for (TEuint i = 0; i < size; i++) {
      unpack.byte[i] = *m_pbPosition;
      m_pbPosition++;
    }
    m_nSize -= size;
    T value = unpack.val;
    return value;
  }

  template <class T>
  void ByteStream::write (T value) {
    TEuint size = sizeof (T);
    if (m_nSize + size > MAX_BUFFER) {
      WARNING_MESSAGE("Overflow on writing.")
      return;
    }

    union pack_t {
      TEbyte byte[sizeof (T)];
      T val;
    } pack;

    pack.val = value;
    for (TEuint i = 0; i < size; i++) {
      *m_pbPosition = pack.byte[i];
      m_pbPosition++;
    }
    m_nSize += size;
  }
}

#endif /* __BYTESTREAM_HPP_ */
