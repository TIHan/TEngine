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

#ifndef CONNECTION_MANAGER_INTERFACE_H_
#define CONNECTION_MANAGER_INTERFACE_H_

#include "packet.h"

namespace engine {
namespace network {

class AddressAdapterInterface {
public:
  virtual ~AddressAdapterInterface() {}

  virtual std::string GetIp() = 0;

  virtual bool operator==(const AddressAdapterInterface& compare) const = 0;
};

class ConnectionManagerInterface {
public:
  virtual ~ConnectionManagerInterface() {}

  virtual void AcceptAddress(std::unique_ptr<AddressAdapterInterface> address,
                             ByteStream* stream) = 0;
  virtual size_t AddressExists(const AddressAdapterInterface& address) = 0;
  virtual void DisconnectByHash(size_t hash) = 0;
  virtual void KickByHash(size_t hash) = 0;
  virtual void BanByHash(size_t hash) = 0;
  virtual void BanByIp(const std::string& ip) = 0;
  virtual void UnbanIp(const std::string& ip) = 0;
};

class PacketMultiplexerInterface {
public:
  virtual ~PacketMultiplexerInterface() {}

  virtual std::unique_ptr<Packet> Insert(size_t connection_hash,
      std::unique_ptr<ByteStream> stream) = 0;
};

class MessageMapperInterface {
public:
  virtual ~MessageMapperInterface() {}

  virtual MessageType Map(std::shared_ptr<Packet> packet) = 0;
};

class MessageProcessorInterface {
public:
  virtual ~MessageProcessorInterface() {}

  virtual void Process() = 0;
};

class MessageChannelInterface {
public:
  virtual ~MessageChannelInterface() {}

  virtual void PushPacket(std::unique_ptr<Packet> packet) = 0;
  virtual void Flush() = 0;
};

class PacketRouterInterface {
public:
  virtual ~PacketRouterInterface() {}

  virtual void Push(std::unique_ptr<Packet> packet) = 0;
};

} // end network namespace
} // end engine namespace

#endif /* CONNECTION_MANAGER_INTERFACE_H_ */