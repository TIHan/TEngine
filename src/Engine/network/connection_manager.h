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

#ifndef CONNECTION_MANAGER_H_
#define CONNECTION_MANAGER_H_

#include "connection_manager_interface.h"
#include "udp_socket.h"

namespace engine {
namespace network {

class AddressAdapter {
public:
  explicit AddressAdapter(std::unique_ptr<SocketAddress> socket_address);
  virtual ~AddressAdapter() {}

  virtual std::string GetIp() const;

  virtual bool operator==(const AddressAdapterInterface& compare) const;

private:
  std::unique_ptr<SocketAddress> socket_address_;
};

class ConnectionManager : public virtual ConnectionManagerInterface {
public:
  ConnectionManager();
  virtual ~ConnectionManager() {}

  virtual void AcceptAddress(std::unique_ptr<AddressAdapterInterface> address,
                             ByteStream* stream);
  virtual size_t AddressExists(const AddressAdapterInterface& address);
  virtual void DisconnectByHash(size_t hash);
  virtual void KickByHash(size_t hash);
  virtual void BanByHash(size_t hash);
  virtual void BanByIp(const std::string& ip);
  virtual void UnbanIp(const std::string& ip);

private:
  std::map<size_t, std::shared_ptr<AddressAdapterInterface>> connections_;
  std::set<std::string> banned_ips_;
};

} // end network namespace
} // end engine namespace

#endif /* CONNECTION_MANAGER_H_ */