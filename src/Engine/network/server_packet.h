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

#ifndef SERVER_PACKET_H_
#define SERVER_PACKET_H_

#include <engine_lib.h>

namespace engine {
namespace network {

const int kAllClients = -1;

class ServerPacket : public ByteStream {
public:
  explicit ServerPacket(int client_id);
  ServerPacket(std::pair<int, std::shared_ptr<ByteStream>> key_stream);
  virtual ~ServerPacket();

  // Accessors / Mutators
  int client_id();

private:
  int client_id_;
};

inline ServerPacket::ServerPacket(int client_id) {
  client_id_ = client_id;
}

inline ServerPacket::ServerPacket(
    std::pair<int, std::shared_ptr<ByteStream>> key_stream) {
  auto stream = key_stream.second;

  client_id_ = key_stream.first;
  if (stream && stream->GetSize() > 0) {
      WriteStream(*key_stream.second);
  }
}

inline ServerPacket::~ServerPacket() {
}

inline int ServerPacket::client_id() {
  return client_id_;
}

} // end network namespace
} // end engine namespace

#endif // SERVER_PACKET_H_
