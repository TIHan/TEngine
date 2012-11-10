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

#ifndef __PASOCKET_HPP_
#define __PASOCKET_HPP_

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

#ifndef NETWORK_NO_TELIB
  #include <TELib.hpp>
#endif

namespace TE {
  struct address_s {
    address_s(const struct sockaddr_storage& ss, const TEint& n) {
      ssAddress = ss;
      nLength = n;
    }
    struct sockaddr_storage ssAddress;
    TEint nLength;
  };

  class PASocket {
    void Create(const TEbyte& bFamily, const TEbyte& bSocketType, const TEbyte& bFlags, const string& szNodeName, const string& szServiceName);
    void SetFamily(const SocketFamily& family);

  public:
    TEint m_iSocket;
    struct addrinfo* m_pAddressInfo;
    struct addrinfo* m_pAddress;
    TEbyte m_bFamily;
    TEboolean m_bError;

    void Initialize(const TEbyte& bSocketType, const SocketFamily& family, const string& szNodeName, const string& szServiceName);
    void Close();
  };
}

#endif /* __PASOCKET_HPP_ */