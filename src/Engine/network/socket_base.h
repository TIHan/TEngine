/*
  Copyright (c) 2012, William F. Smith
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

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

#ifndef SOCKETBASE_H_
#define SOCKETBASE_H_

#include <engine_lib.h>

#define SOCKET_MAX_BUFFER 1400

namespace engine {
namespace network {

enum SocketFamily {
  SOCKET_UNSPECIFIED,
  SOCKET_IPV4,
  SOCKET_IPV6
};

typedef struct SocketAddressImpl SocketAddress;

std::string GetSocketAddress(const SocketAddress& address);

class SocketInterface {
public:
  virtual ~SocketInterface() {};

  virtual int Bind(const uint16_t& port) = 0;
  virtual std::string GetAddress() = 0;
  virtual void Close() = 0;

  /* Accessors / Mutators */
  virtual SocketFamily family() = 0;
};

class SocketBaseImpl;
class SocketBase : public virtual SocketInterface {
public:
  virtual ~SocketBase();

  virtual int Bind(const uint16_t& port);
  virtual std::string GetAddress();
  virtual void Close();

  /* Accessors / Mutators */
  virtual SocketFamily family();

protected:
  std::unique_ptr<SocketBaseImpl> impl_;

  SocketBase(); // Abstract class
};

} // end network namespace
} // end engine namespace

#endif // SOCKET_BASE_H_
