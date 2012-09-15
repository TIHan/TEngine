#ifndef BYTEIO_HPP
#define BYTEIO_HPP

#include "types.hpp"

namespace dgp {
  // Read  
  dgpChar readChar (dgpByte *bStream);
  dgpInt8 readInt8 (dgpByte *bStream);
  dgpByte readByte (dgpByte *bStream);
  dgpShort readShort (dgpByte *bStream);
  dgpUshort readUshort (dgpByte *bStream);
  dgpInt readInt (dgpByte *bStream);
  dgpUint readUint (dgpByte *bStream);
  dgpLong readLong (dgpByte *bStream);
  dgpUlong readUlong (dgpByte *bStream);

  // Write
  void writeChar (dgpByte *bStream, dgpChar);
  void writeInt8 (dgpByte *bStream, dgpInt8);
  void writeByte (dgpByte *bStream, dgpByte);
  void writeShort (dgpByte *bStream, dgpShort);
  void writeUshort (dgpByte *bStream, dgpUshort);
  void writeInt (dgpByte *bStream, dgpInt);
  void writeUint (dgpByte *bStream, dgpUint);
  void writeLong (dgpByte *bStream, dgpLong);
  void writeUlong (dgpByte *bStream, dgpUlong);
}

#endif // BYTEIO_HPP