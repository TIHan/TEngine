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

#include "connection_manager.h"

namespace engine {
namespace network {

// Address Adapter

inline AddressAdapter::AddressAdapter(
    std::unique_ptr<SocketAddress> socket_address)
    : socket_address_(std::move(socket_address)) {
}

inline std::string AddressAdapter::GetIp() const {
  return socket_address_->GetText();
}

inline bool AddressAdapter::operator==(const AddressAdapterInterface& compare) const {
  return false;
}

// Connection Manager

inline ConnectionManager::ConnectionManager() {
}

inline void ConnectionManager::AcceptAddress(
    std::unique_ptr<AddressAdapterInterface> address,
    ByteStream* stream) {
  // See if the address is banned.
  auto ip_iter = banned_ips_.find(address->GetIp());
  if (ip_iter != banned_ips_.end()) {
    return;
  }
  // If the connector's header isn't Connect, return.
  if (stream->ReadByte() != static_cast<uint8_t>(PacketHeader::kConnect))
    return;

  // Add connection.
  // TODO: When GCC 4.8 is released, use emplace here.
  connections_.insert(std::make_pair(
      std::hash<std::unique_ptr<AddressAdapterInterface>>()(address), std::move(address)));
}

inline size_t ConnectionManager::AddressExists(
    const AddressAdapterInterface& address) {
  // See if the address is already connected.
  for (auto connection : connections_) {
    if (*connection.second == address)
      return connection.first;
  }
  return -1;
}

inline void ConnectionManager::DisconnectByHash(size_t hash) {
  connections_.erase(hash);
}

inline void ConnectionManager::KickByHash(size_t hash) {
  DisconnectByHash(hash);
}

inline void ConnectionManager::BanByHash(size_t hash) {
  auto iter = connections_.find(hash);
  if (iter != connections_.end()) {
    // TODO: When GCC 4.8 is released, use emplace here.
    banned_ips_.insert(iter->second->GetIp());
    KickByHash(hash);
  }
}

inline void ConnectionManager::BanByIp(const std::string& ip) {
  // TODO: When GCC 4.8 is released, use emplace here.
  banned_ips_.insert(ip);
  for (auto connection : connections_) {
    if (connection.second->GetIp() == ip) {
      KickByHash(connection.first);
      return;
    }
  }
}

inline void ConnectionManager::UnbanIp(const std::string& ip) {
  banned_ips_.erase(ip);
}

} // end network namespace
} // end engine namespace
