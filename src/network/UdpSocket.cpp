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

#include "UdpSocket.hpp"
#include "PASocket.hpp"

namespace TE {
  /*!
   *
   */
  UdpSocket::UdpSocket(const SocketFamily family, const string szNodeName) {
    priv->Initialize(family);
    priv->Create(szNodeName, SOCK_DGRAM);
  }

  /*!
   *
   */
  UdpSocket::~UdpSocket() {
  }

  /*!
   *
   */
  tuple<shared_ptr<TEbyte>, TEint, string> UdpSocket::Receive() {
    struct sockaddr_storage sock_addr;
    string szAddress;
    TEchar *address = new TEchar[INET6_ADDRSTRLEN];
    socklen_t addr_len = sizeof sock_addr;
    shared_ptr<TEbyte> pBuffer (new TEbyte[SOCKET_MAX_BUFFER]);

    TEint bytes = recvfrom(priv->m_iSocket, (TEchar *)pBuffer.get(), SOCKET_MAX_BUFFER, 0, (sockaddr *)&sock_addr, &addr_len);
    switch (sock_addr.ss_family) {
    case AF_INET6:
      inet_ntop(priv->m_bFamily, &(((struct sockaddr_in6 *)&sock_addr)->sin6_addr), address, INET6_ADDRSTRLEN);
      break;
    default:
      inet_ntop(priv->m_bFamily, &(((struct sockaddr_in *)&sock_addr)->sin_addr), address, INET_ADDRSTRLEN);
      break;
    }
    szAddress.assign(address);
    return tuple<shared_ptr<TEbyte>, TEint, string>(pBuffer, bytes, szAddress);
  }

  /*!
   *
   */
  TEint UdpSocket::Send(const shared_ptr<TEbyte> pBuffer,
    const TEuint nBufferSize) {
    TEint bytes;
    bytes = sendto(priv->m_iSocket, (const TEchar *)pBuffer.get(), nBufferSize, 0, priv->m_pAddressInfo->ai_addr, (TEint)priv->m_pAddressInfo->ai_addrlen);
    return bytes;
  }
}