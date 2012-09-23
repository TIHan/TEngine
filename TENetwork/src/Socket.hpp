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

#ifndef __SOCKET_HPP_
#define __SOCKET_HPP_

#ifdef __GNUC__
  #include <sys/socket.h>
  #include <netdb.h>
  #include <arpa/inet.h>
  #include <netinet/in.h>
  #include <cstring>
  #include <stdlib.h>
  #include <unistd.h>
#elif _MSC_VER
  #include <winsock2.h>
  #include <ws2tcpip.h>

  #pragma comment(lib, "ws2_32.lib")
#endif

#include "Types.hpp"

#define IP_STRLEN INET6_ADDRSTRLEN

namespace TE {
  enum SocketFamily {
    IPV4,
    IPV6
  };

  class ISocket {
  public:
    virtual ~ISocket () {};
    virtual void Close () = 0;
    virtual TEint Bind (const TEushort usPort) = 0;
    virtual void GetAddressText (TEchar *pszAddress) = 0;
    virtual TEint Receive (TEbyte *pBuffer, const TEuint nBufferSize) = 0;
    virtual TEint Send (const TEbyte *pBuffer, const TEuint nBufferSize, const TEchar *szNodeName, const TEchar *szServiceName) = 0;
  };

  class Socket : public ISocket {
    TEint m_iSocket;
    struct addrinfo *m_pAddressInfo;
    struct addrinfo *m_pAddress;
    TEbyte m_bFamily;

    void Initialize (const TEbyte bFamily);
  public:
    Socket ();
    explicit Socket (const TEbyte bFamily);
    ~Socket ();

    void Close ();
    TEint Bind (const TEushort usPort);
    void GetAddressText (TEchar *pszAddress);
    TEint Receive (TEbyte *pBuffer, const TEuint nBufferSize);
    TEint Send (const TEbyte *pBuffer, const TEuint nBufferSize, const TEchar *szNodeName, const TEchar *szServiceName);
  };
}

#endif /* __SOCKET_HPP_ */
