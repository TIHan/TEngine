#ifndef BYTESTREAM_HPP
#define BYTESTREAM_HPP

#include "types.hpp"

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
    dgpChar readChar ();
    dgpInt8 readInt8 ();
    dgpByte readByte ();
    dgpShort readShort ();
    dgpUshort readUshort ();
    dgpInt readInt ();
    dgpUint readUint ();
    dgpLong readLong ();
    dgpUlong readUlong ();

    // Write
    void writeChar (dgpChar value);
    void writeInt8 (dgpInt8 value);
    void writeByte (dgpByte value);
    void writeShort (dgpShort value);
    void writeUshort (dgpUshort value);
    void writeInt (dgpInt value);
    void writeUint (dgpUint value);
    void writeLong (dgpLong value);
    void writeUlong (dgpUlong value);
  };
}

#endif // BYTESTREAM_HPP
