#ifndef __BYTESTREAM_HPP_
#define __BYTESTREAM_HPP_

#include "Types.hpp"
#include "Messages.hpp"

namespace TE {
  class IByteStream {
  public:
    virtual TEuint GetSize () = 0;
    virtual void Clear () = 0;
  };

  class ByteStream : IByteStream {
    TEbyte *m_pbStream;
    TEbyte *m_pbReadPosition;
    TEbyte *m_pbWritePosition;
    TEuint m_nSize;
    TEuint m_nMaxSize;

  public:
    explicit ByteStream(TEuint nMaxSize);
    ~ByteStream();

    TEbyte* GetCopyOfStream ();
    TEuint GetSize ();
    TEuint GetMaxSize ();
    void Clear ();

    // Read
    template <class T>
    T Read ();
    TEchar* ReadString ();

    // Write
    template <class T>
    void Write (const T value);
    void WriteString (const TEchar *value);
    void WriteStream (const TEbyte *pbStream, const TEuint nSize);
  };

  /*!
   *
   */
  template <class T>
  T ByteStream::Read () {
    TEuint size = sizeof (T);
    if (m_nSize - size > MAX_BUFFER) {
      WARNING_MESSAGE("Overflow on reading.")
      return 0;
    }

    union unpack_t {
      TEbyte byte[sizeof (T)];
      T val;
    } unpack;

    for (TEuint i = 0; i < size; i++) {
      unpack.byte[i] = *m_pbReadPosition;
      m_pbReadPosition++;
    }
    m_nSize -= size;
    T value = unpack.val;
    return value;
  }

  /*!
   *
   */
  template <class T>
  void ByteStream::Write (const T value) {
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
      *m_pbWritePosition = pack.byte[i];
      m_pbWritePosition++;
    }
    m_nSize += size;
  }
}

#endif /* __BYTESTREAM_HPP_ */
