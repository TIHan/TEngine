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

#include "ASocket.hpp"
#include "PASocket.hpp"

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
  /*!
   *
   */
  void PASocket::Initialize(const SocketFamily family) {
    m_iSocket = -1;
    m_pAddress = 0;

    switch (family) {
    case SOCKET_IPV4:
      m_bFamily = AF_INET;
      break;
    case SOCKET_IPV6:
      m_bFamily = AF_INET6;
      break;
    default:
      m_bFamily = AF_UNSPEC;
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
  void PASocket::Create(const TEint iSocketType, const string szNodeName, const string szServiceName) {
    struct addrinfo hints, *p;
    TEchar *nodeName = 0;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = m_bFamily;
    hints.ai_socktype = iSocketType;

    if (szNodeName.compare("") != 0) {
      nodeName = (TEchar *)szNodeName.c_str();
    } else {
      hints.ai_flags = AI_PASSIVE;
    }

    if (getaddrinfo(nodeName, szServiceName.c_str(), &hints, &m_pAddressInfo) != 0) {
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
  ASocket::ASocket() :
      priv(new PASocket()) {
  }

  /*!
   *
   */
  ASocket::~ASocket() {
    if (priv->m_pAddressInfo) {
      freeaddrinfo(priv->m_pAddressInfo);
    }
    CloseSocket(priv->m_iSocket);
  }

  /*!
   *
   */
  TEint ASocket::Bind(const TEushort usPort) {
    if (priv->m_iSocket != -1 && priv->m_pAddress) {
      struct sockaddr_in *sockaddr = (struct sockaddr_in *)priv->m_pAddress->ai_addr;

      sockaddr->sin_port = ntohs(usPort);
      if (BindSocket(priv->m_iSocket, priv->m_pAddress->ai_addr, (TEint)priv->m_pAddress->ai_addrlen) != 0) {
        priv->m_bError = true;
        return -1;
      }
      return 0;
    }
    return -1;
  }

  /*!
   *
   */
  string ASocket::GetAddress() {
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
  TEboolean ASocket::HasErrors() {
    return priv->m_bError;
  }
}
