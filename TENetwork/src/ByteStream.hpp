#ifndef __BYTESTREAM_HPP_
#define __BYTESTREAM_HPP_

#include "Types.hpp"
#include "Messages.hpp"

#define MAX_BUFFER 512

namespace TE {
  class IByteStream {
  public:
    virtual TEbyte* GetStream () = 0;
    virtual TEuint GetSize () = 0;
    virtual void SetSize (TEuint nSize) = 0;
    virtual void UnrefStream () = 0;
    virtual void Clear () = 0;
  };

  class ByteStream : IByteStream {
    TEbyte *m_pbStream;
    TEbyte *m_pbPosition;
    TEuint m_nSize;
    TEuint m_nStreamRefCount;

  public:
    ByteStream();
    ~ByteStream();

    TEbyte* GetStream ();
    TEuint GetSize ();
    void SetSize (TEuint nSize);

    void UnrefStream ();
    void Clear ();

    // Read
    template <class T>
    T Read ();
    TEchar* ReadString ();

    // Write
    template <class T>
    void Write (T value);
    void WriteString (const TEchar *value);
  };

  template <class T>
  T ByteStream::Read () {
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
  void ByteStream::Write (T value) {
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
