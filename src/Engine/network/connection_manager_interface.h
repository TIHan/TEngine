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
#include "udp_socket.h"

namespace engine {
namespace network {

struct Connection {
  Connection(size_t hash, std::string ip) : hash(hash), ip(ip) {}
  size_t hash;
  std::string ip;
};

struct AcceptedConnectionMessage : MessageBase<AcceptedConnectionMessage> {
  AcceptedConnectionMessage(Connection connection) : connection(connection) {}
  Connection connection;
};

struct KickedConnectionMessage : MessageBase<KickedConnectionMessage> {
  KickedConnectionMessage(Connection connection) : connection(connection) {}
  Connection connection;
};

struct BannedConnectionMessage : MessageBase<BannedConnectionMessage> {
  BannedConnectionMessage(Connection connection) : connection(connection) {}
  Connection connection;
};

class AddressAdapterInterface {
public:
  virtual ~AddressAdapterInterface() {}

  virtual std::string GetIp() = 0;
};

class ConnectionManagerInterface {
public:
  virtual ~ConnectionManagerInterface() {}

  virtual void AcceptConnection(
      std::unique_ptr<AddressAdapterInterface> address,
      const std::unique_ptr<ByteStream> stream) = 0;
  // TODO: DisbandConnection or DisconnectConnection
  virtual void KickConnection(Connection connection) = 0;
  virtual void BanConnection(Connection connection) = 0;
  virtual void UnbanConnection(Connection connection) = 0;
};

class PacketMultiplexerInterface {
public:
  virtual ~PacketMultiplexerInterface() {}

  virtual std::unique_ptr<Packet> Insert(Connection connection,
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