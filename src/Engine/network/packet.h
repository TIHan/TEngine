/*
  Copyright (c) 2012-2013, William F. Smith
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation 
  and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
  THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef PACKET_H_
#define PACKET_H_

#include <engine_lib.h>

namespace engine {
namespace network {

enum class PacketHeader : uint8_t {
  kUnreliable,
  kUnreliableSequenced,
  kReliable,
  kReliableSequenced,
  kReliableOrdered,
  kConnect,
  kDisconnect,
  kHeartbeat
};

enum class TransferMethod : uint8_t {
  kUnreliable,
  kUnreliableSequenced,
  kReliable,
  kReliableSequenced,
  kReliableOrdered
};

class Packet : ByteStream {
public:
  Packet(TransferMethod transfer_method, int connection_id,
         int sequence_number, const ByteStream& stream);

  /* A+M */
  TransferMethod transfer_method() { return transfer_method_; }
  int connection_id() { return connection_id_; }
  int sequence_number() { return sequence_number_; }

private:
  TransferMethod transfer_method_;
  int connection_id_;
  int sequence_number_;
};

inline Packet::Packet(TransferMethod transfer_method, int connection_id,
                      int sequence_number, const ByteStream& stream)
    : ByteStream(stream, stream.read_position()) {
  transfer_method_ = transfer_method;
  connection_id_ = connection_id;
  sequence_number_ = sequence_number;
}

} // end network namespace
} // end engine namespace

#endif /* PACKET_H_ */