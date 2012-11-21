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

#ifndef UDP_SOCKET_H_
#define UDP_SOCKET_H_

#include "socket_base.h"

// Microsoft gives a warning about virtual inheritance. Turn it off.
#ifdef _MSC_VER
# pragma warning( disable : 4250 ) // C4250 - 'class1' : inherits 'class2::member' via dominance
#endif

namespace engine {
namespace network {

class UdpSocketInterface : public virtual SocketInterface {
public:
  virtual ~UdpSocketInterface() {};

  virtual void Open() = 0;
  virtual void Open(const std::string& address, const std::string& port) = 0;
  virtual std::tuple<std::shared_ptr<std::vector<uint8_t>>, std::shared_ptr<SocketAddress>> ReceiveFrom() = 0;
  virtual int Send(const std::vector<uint8_t>& data) = 0;
  virtual int Send(const lib::ByteStream& data) = 0;
  virtual int SendTo(const std::vector<uint8_t>& data, const SocketAddress& address) = 0;
  virtual int SendTo(const lib::ByteStream& data, const SocketAddress& address) = 0;
};

class UdpSocket : public SocketBase, public UdpSocketInterface {
public:
  UdpSocket();
  explicit UdpSocket(const SocketFamily& family);
  virtual ~UdpSocket();

  virtual void Open();
  virtual void Open(const std::string& address, const std::string& port);
  virtual std::tuple<std::shared_ptr<std::vector<uint8_t>>, std::shared_ptr<SocketAddress>> ReceiveFrom();
  virtual int Send(const std::vector<uint8_t>& data);
  virtual int Send(const lib::ByteStream& data);
  virtual int SendTo(const std::vector<uint8_t>& data, const SocketAddress& address);
  virtual int SendTo(const lib::ByteStream& data, const SocketAddress& address);
};

} // end network namespace
} // end engine namespace

#endif // UDP_SOCKET_H_