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

#include "connection_manager_interface.h"
#include "udp_socket.h"

namespace engine {
namespace network {

// Address Adapter

class AddressAdapter {
public:
  explicit AddressAdapter(std::unique_ptr<SocketAddress> socket_address);

  virtual std::string GetIp() const;

  bool operator==(const AddressAdapter& compare) const;

private:
  std::unique_ptr<SocketAddress> socket_address_;
};

inline AddressAdapter::AddressAdapter(
    std::unique_ptr<SocketAddress> socket_address)
    : socket_address_(std::move(socket_address)) {
}

inline std::string AddressAdapter::GetIp() const {
  return socket_address_->GetText();
}

inline bool AddressAdapter::operator==(const AddressAdapter& compare) const {
  return *socket_address_ == *compare.socket_address_;
}

// Connection Manager

class ConnectionManager : public virtual ConnectionManagerInterface {
public:
  ConnectionManager(EventAggregator* event_aggregator);

  virtual void AcceptConnection(
      std::unique_ptr<AddressAdapterInterface> address,
      const std::unique_ptr<ByteStream> stream);
  virtual void KickConnection(Connection connection);
  virtual void BanConnection(Connection connection);
  virtual void UnbanConnection(Connection connection);

protected:
  Connection ConnectionExists(
      std::unique_ptr<AddressAdapterInterface> address);

private:
  std::map<
      std::shared_ptr<AddressAdapterInterface>, Connection> connections_;
  std::set<std::string> banned_ips_;

  // Events
  EventAggregator* event_aggregator_;
};

inline ConnectionManager::ConnectionManager(EventAggregator* event_aggregator)
    : event_aggregator_(event_aggregator) {
}

inline Connection ConnectionManager::ConnectionExists(
    std::unique_ptr<AddressAdapterInterface> address) {
  // See if the address is already connected.
  auto iter = connections_.find(std::move(address));
  if (iter != connections_.end()) {
    // Return valid connection.
    return iter->second;
  }
  return Connection(0, stdext::string::empty());
}

inline void ConnectionManager::AcceptConnection(
    std::unique_ptr<AddressAdapterInterface> address,
    const std::unique_ptr<ByteStream> stream) {
  // See if the address is banned.
  auto ip_iter = banned_ips_.find(address->GetIp());
  if (ip_iter != banned_ips_.end()) {
    return;
  }
  // If the connector's header isn't Connect, return.
  if (stream->ReadByte() != static_cast<uint8_t>(PacketHeader::kConnect))
    return;

  // Build a new connection.
  Connection connection(
      std::hash<std::unique_ptr<AddressAdapterInterface>>()(address),
      address->GetIp());
  connections_.emplace(std::move(address), connection);
  event_aggregator_->Publish<AcceptedConnectionMessage>(connection);
}

inline void ConnectionManager::KickConnection(Connection connection) {
  std::for_each(connections_.begin(), connections_.end(),
      [=] (std::pair<std::shared_ptr<AddressAdapterInterface>,
                     Connection> pair) {
    if (pair.second.hash == connection.hash) {
      connections_.erase(pair.first);
      event_aggregator_->Publish<KickedConnectionMessage>(connection);
      return;
    }
  });
}

inline void ConnectionManager::BanConnection(Connection connection) {
  KickConnection(connection);
  banned_ips_.emplace(connection.ip);
  event_aggregator_->Publish<BannedConnectionMessage>(connection);
}

inline void ConnectionManager::UnbanConnection(Connection connection) {
  banned_ips_.erase(connection.ip);
}

} // end network namespace
} // end engine namespace