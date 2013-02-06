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

#ifndef UDP_SOCKET_H_
#define UDP_SOCKET_H_

#include "socket_base.h"

namespace engine {
namespace network {

struct UdpSocketOptions {
  UdpSocketOptions() {
    blocking = true;
    family = SocketFamily::kUnspecified;
    max_receive_buffer = 1024;
  }
  int max_receive_buffer;
  SocketFamily family;
  bool blocking;
};

class UdpSocket : public SocketBase {
public:
  UdpSocket();
  explicit UdpSocket(const UdpSocketOptions& options);

  virtual ~UdpSocket();

  virtual void Open();
  virtual void Open(const std::string& address, const std::string& port);

  virtual std::tuple<ByteStream, SocketAddress> ReceiveFrom();

  virtual int Send(const ByteStream& data);
  virtual int SendTo(const ByteStream& data,
                     const SocketAddress& address);

  virtual bool WaitToRead(int usec);

private:
  void Init();

  UdpSocketOptions options_;
  std::unique_ptr<std::vector<uint8_t>> receive_buffer_;
};

} // end network namespace
} // end engine namespace

#endif // UDP_SOCKET_H_