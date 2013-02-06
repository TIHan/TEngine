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

#ifndef SOCKETBASE_H_
#define SOCKETBASE_H_

#include <engine_lib.h>

namespace engine {
namespace network {

enum class SocketFamily : uint8_t {
  kUnspecified,
  kIpv4,
  kIpv6
};

// Socket Address

struct SocketAddressData;

class SocketAddressImpl;
class SocketAddress {
public:
  SocketAddress(const SocketAddress& copy);
  explicit SocketAddress(const SocketAddressData& addr_data);
  virtual ~SocketAddress();

  std::string GetText() const;
  int GetLength() const;
  const void* GetRaw() const;

  bool operator==(const SocketAddress& compare) const;

private:
  std::unique_ptr<SocketAddressImpl> impl_;
};

// Socket Base

class SocketBaseImpl;
class SocketBase {
public:
  virtual ~SocketBase();

  virtual int Bind(uint16_t port);
  virtual std::string GetAddressText();
  virtual int GetPort();
  virtual void Close();

  /* Accessors / Mutators */
  virtual SocketFamily family();
  virtual bool blocking();

protected:
  SocketBase();

  std::unique_ptr<SocketBaseImpl> impl_;
};

} // end network namespace
} // end engine namespace

#endif // SOCKET_BASE_H_
