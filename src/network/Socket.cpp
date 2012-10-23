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

#include "Socket.hpp"

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

/*!
 *
 */
static TE::TEint CreateSocket(TE::TEint domain,
                              TE::TEint type,
                              TE::TEint protocol) {
  return (TE::TEint)socket(domain, type, protocol);
}

/*!
 *
 */
static TE::TEint BindSocket(TE::TEint sockfd,
                            struct sockaddr *my_addr,
                            TE::TEint addrlen) {
  return bind(sockfd, my_addr, addrlen);
}

/*!
 *
 */
static TE::TEint CloseSocket(TE::TEint sockfd) {
#ifdef __GNUC__
    return close(sockfd);
#elif _MSC_VER
    return closesocket(sockfd);
#endif
}

namespace TE {
  class PSocket {
		public:
    TEint m_iSocket;
    struct addrinfo *m_pAddressInfo;
    struct addrinfo *m_pAddress;
    TEbyte m_bFamily;
    TEboolean m_bError;

    void Initialize(const TEbyte bFamily);
    void Create(const string szNodeName, const string szServiceName);
  };

  /*!
   *
   */
  void PSocket::Initialize(const TEbyte bFamily) {
    m_iSocket = -1;
    m_pAddress = 0;

    switch (bFamily) {
    case SOCKET_IPV6:
      m_bFamily = AF_INET6;
      break;
    default:
      m_bFamily = AF_INET;
    }

#ifdef _MSC_VER
      WSADATA wsaData;
      TEint wsaResult;

      // Initialize Winsock
      wsaResult = WSAStartup(MAKEWORD(2,2), &wsaData);
      ERROR_IF_FORMAT(wsaResult != 0, "WSAStartup failed with result %i", wsaResult)
#endif
  }

  /*!
   *
   */
  void PSocket::Create(const string szNodeName,
                       const string szServiceName) {
    struct addrinfo hints, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = m_bFamily;
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo(szNodeName.c_str(), szServiceName.c_str(), &hints, &m_pAddressInfo) != 0) {
      m_bError = true;
      return;
    }

    for (p = m_pAddressInfo; p != 0; p = p->ai_next) {
      if (p->ai_family == m_bFamily) {
        m_iSocket = CreateSocket(p->ai_family, p->ai_socktype, p->ai_protocol);
        m_pAddress = p;
      }
    }

    if (m_iSocket == -1) {
      m_bError = true;
    }
  }

  /*!
   *
   */
  Socket::Socket() :
            priv(new PSocket()) {
    priv->Initialize(SOCKET_IPV4);
    priv->Create("", "");
  }

  /*!
   *
   */
  Socket::Socket(const TEbyte bFamily) :
            priv(new PSocket()) {
    priv->Initialize(bFamily);
    priv->Create("", "");
  }

  /*!
   *
   */
  Socket::Socket(const TEbyte bFamily,
                 const string szNodeName,
                 const string szServiceName) :
            priv(new PSocket()) {
    priv->Initialize(bFamily);
    priv->Create(szNodeName, szServiceName);
  }

  /*!
   *
   */
  Socket::~Socket() {
    Close();
  }

  /*!
   *
   */
  void Socket::Close() {
    freeaddrinfo(priv->m_pAddressInfo);
    WARNING_IF(CloseSocket(priv->m_iSocket) != 0, "Unable to close socket.")
  }

  /*!
   *
   */
  TEint Socket::Bind(const TEushort usPort) {
    if (priv->m_iSocket != -1 && priv->m_pAddress) {
      struct sockaddr_in *sockaddr = (struct sockaddr_in *)priv->m_pAddress->ai_addr;
      sockaddr->sin_port = ntohs(usPort);
      if (BindSocket(priv->m_iSocket, priv->m_pAddress->ai_addr, (TEint)priv->m_pAddress->ai_addrlen) != 0) {
        return -1;
      }
    }
    return 0;
  }

  /*!
   *
   */
  string Socket::GetAddress() {
    if (priv->m_pAddress) {
      TEchar *address = new TEchar[INET6_ADDRSTRLEN];
      string s;

      switch (priv->m_bFamily) {
      case AF_INET6:
        inet_ntop(priv->m_bFamily, &(((struct sockaddr_in6 *)priv->m_pAddress->ai_addr)->sin6_addr), address, INET6_ADDRSTRLEN);
        break;
      default:
        inet_ntop(priv->m_bFamily, &(((struct sockaddr_in *)priv->m_pAddress->ai_addr)->sin_addr), address, INET_ADDRSTRLEN);
        break;
      }
      return s.assign (address);
    } else {
      return "";
    }
  }

  /*!
   *
   */
  tuple<TEint, string> Socket::Receive(shared_ptr<TEbyte> pBuffer,
                        const TEuint nBufferSize) {
    struct sockaddr_storage sock_addr;
    TEchar *address = new TEchar[INET6_ADDRSTRLEN];
    socklen_t addr_len = sizeof sock_addr;
    string szAddress;
    
    TEint bytes = recvfrom(priv->m_iSocket, (TEchar *)pBuffer.get(), nBufferSize, 0, (sockaddr *)&sock_addr, &addr_len);
    switch (sock_addr.ss_family) {
    case AF_INET6:
      inet_ntop(priv->m_bFamily, &(((struct sockaddr_in6 *)&sock_addr)->sin6_addr), address, INET6_ADDRSTRLEN);
      break;
    default:
      inet_ntop(priv->m_bFamily, &(((struct sockaddr_in *)&sock_addr)->sin_addr), address, INET_ADDRSTRLEN);
      break;
    }
    szAddress.assign(address);
    return tuple<TEint, string>(bytes, szAddress);
  }

  /*!
   *
   */
  TEint Socket::Send(const shared_ptr<TEbyte> pBuffer,
                     const TEuint nBufferSize) {
    TEint bytes;
    bytes = sendto(priv->m_iSocket, (const TEchar *)pBuffer.get(), nBufferSize, 0, priv->m_pAddressInfo->ai_addr, (TEint)priv->m_pAddressInfo->ai_addrlen);
    return bytes;
  }

  /*!
   *
   */
  TEboolean Socket::HasError() {
    return priv->m_bError;
  }
}
