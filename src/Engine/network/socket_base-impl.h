/*
  Copyright (c) 2012, William F. Smith
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

#ifndef SOCKET_BASE_IMPL_H_
#define SOCKET_BASE_IMPL_H_

#include <engine_lib.h>

#ifdef __GNUC__
# include <sys/socket.h>
#include <sys/unistd.h>
#include <sys/fcntl.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <cstring>
# include <stdlib.h>
# include <unistd.h>
#elif _MSC_VER
# include <winsock2.h>
# include <ws2tcpip.h>

# pragma comment(lib, "ws2_32.lib")
#endif

namespace engine {
namespace network {

struct SocketAddressData {
  SocketAddressData(const struct sockaddr_storage& address, const int& length)
      : address(address), length(length) {}
  struct sockaddr_storage address;
  int length;
};

class SocketAddressImpl {
public:
  SocketAddressImpl(std::shared_ptr<SocketAddressData> addr_data);
  virtual ~SocketAddressImpl();

  std::shared_ptr<SocketAddressData> data_;
};

class SocketBaseImpl {
public:
  void Open(const int& socket_type, const int& flags,
            const std::string& node_name, const std::string& service_name);

  int socket_;
  struct addrinfo* address_info_;
  struct addrinfo* current_address_info_;
  SocketFamily family_;
  bool blocking_;
};

} // end network namespace
} // end engine namespace

#endif // SOCKET_BASE_IMPL_H_