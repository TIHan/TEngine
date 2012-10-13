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
static TE::TEint CreateSocket(TE::TEint domain, TE::TEint type, TE::TEint protocol) {
  return (TE::TEint)socket (domain, type, protocol);
}

/*!
 *
 */
static TE::TEint BindSocket(TE::TEint sockfd, struct sockaddr *my_addr, TE::TEint addrlen) {
  return bind (sockfd, my_addr, addrlen);
}

/*!
 *
 */
static TE::TEint CloseSocket (TE::TEint sockfd) {
#ifdef __GNUC__
    return close (sockfd);
#elif _MSC_VER
    return closesocket (sockfd);
#endif
}

namespace TE {
  class PSocket : public ISocket {
    TEint m_iSocket;
    struct addrinfo *m_pAddressInfo;
    struct addrinfo *m_pAddress;
    TEbyte m_bFamily;

    void Initialize (const TEbyte bFamily);

  public:
    PSocket ();
    explicit PSocket (const TEbyte bFamily);
    ~PSocket ();

    void Close ();
    TEint Bind (const TEushort usPort);
    shared_ptr<TEchar> GetAddressText ();
    TEint Receive (shared_ptr<TEbyte> pBuffer, const TEuint nBufferSize, shared_ptr<TEchar> pszNodeName);
    TEint Send (const shared_ptr<TEbyte> pBuffer, const TEuint nBufferSize);
  };

  /*!
   *
   */
  void PSocket::Initialize (const TEbyte bFamily) {
    struct addrinfo hints, *p;
    m_iSocket = -1;
    m_pAddress = 0;

    switch (bFamily) {
    case IPV6:
      m_bFamily = AF_INET6;
      break;
    default:
      m_bFamily = AF_INET;
    }

#ifdef _MSC_VER
    WSADATA wsaData;
    TEint wsaResult;

    // Initialize Winsock
    wsaResult = WSAStartup (MAKEWORD(2,2), &wsaData);
    ERROR_IF_FORMAT(wsaResult != 0, "WSAStartup failed with result %i", wsaResult)
#endif
  
    memset (&hints, 0, sizeof hints);
    hints.ai_family = m_bFamily;
    hints.ai_socktype = SOCK_DGRAM;

    ERROR_IF(getaddrinfo (0, "", &hints, &m_pAddressInfo) != 0, "Unable to get address info.")
    for (p = m_pAddressInfo; p != 0; p = p->ai_next) {
      if (p->ai_family == m_bFamily) {
        m_iSocket = CreateSocket (p->ai_family, p->ai_socktype, p->ai_protocol);
        m_pAddress = p;
      }
    }
    ERROR_IF(m_iSocket == -1, "Unable to create socket.")
  }

  /*!
   *
   */
  PSocket::PSocket () {
    Initialize (IPV4);
  }

  /*!
   *
   */
  PSocket::PSocket (const TEbyte bFamily) {
    Initialize (bFamily);
  }

  /*!
   *
   */
  PSocket::~PSocket () {
    Close ();
#ifdef _MSC_VER
    WSACleanup ();
#endif
  }

  /*!
   *
   */
  void PSocket::Close () {
    freeaddrinfo (m_pAddressInfo);
    WARNING_IF(CloseSocket (m_iSocket) != 0, "Unable to close socket.")
  }

  /*!
   *
   */
  TEint PSocket::Bind (const TEushort usPort) {
    if (m_iSocket != -1 && m_pAddress) {
          struct sockaddr_in *sockaddr = (struct sockaddr_in *)m_pAddress->ai_addr;
          sockaddr->sin_port = ntohs (usPort);
      
      WARNING_IF_RETURN_VAL_FORMAT(
        BindSocket(m_iSocket, m_pAddress->ai_addr, (TEint)m_pAddress->ai_addrlen) != 0, 
        -1, 
        "Unable to associate socket with port %i.", usPort)
    }
    return 0;
  }

  /*!
   *
   */
  shared_ptr<TEchar> PSocket::GetAddressText () {
    if (m_pAddress) {
      TEchar *address = new TEchar[INET6_ADDRSTRLEN];

      switch (m_bFamily) {
      case AF_INET6:
        inet_ntop (m_bFamily, &(((struct sockaddr_in6 *)m_pAddress->ai_addr)->sin6_addr), address, INET6_ADDRSTRLEN);
        break;
      default:
        inet_ntop (m_bFamily, &(((struct sockaddr_in *)m_pAddress->ai_addr)->sin_addr), address, INET_ADDRSTRLEN);
        break;
      }
      return shared_ptr<TEchar> (address, default_delete<TEchar[]> ());
    } else {
      return 0;
    }
  }

  /*!
   *
   */
  TEint PSocket::Receive (shared_ptr<TEbyte> pBuffer, const TEuint nBufferSize, shared_ptr<TEchar> pszNodeName) {
    struct sockaddr_storage sock_addr;
    socklen_t addr_len = sizeof sock_addr;
    
    TEint bytes = recvfrom (m_iSocket, (TEchar *)pBuffer.get (), nBufferSize, 0, (sockaddr *)&sock_addr, &addr_len);
    switch (sock_addr.ss_family) {
    case AF_INET6:
      inet_ntop (m_bFamily, &(((struct sockaddr_in6 *)&sock_addr)->sin6_addr), pszNodeName.get (), INET6_ADDRSTRLEN);
      break;
    default:
      inet_ntop (m_bFamily, &(((struct sockaddr_in *)&sock_addr)->sin_addr), pszNodeName.get (), INET_ADDRSTRLEN);
      break;
    }
    return bytes;
  }

  /*!
   *
   */
  TEint PSocket::Send (const shared_ptr<TEbyte> pBuffer, const TEuint nBufferSize) {
    TEint bytes;
    bytes = sendto (m_iSocket, (const TEchar *)pBuffer.get (), nBufferSize, 0, m_pAddressInfo->ai_addr, (TEint)m_pAddressInfo->ai_addrlen);
    return bytes;
  }

  /****************************************************************************************************************************
  *****************************************************************************************************************************
  *****************************************************************************************************************************
  ****************************************************************************************************************************/

  /*!
   *
   */
  Socket::Socket () : priv (new PSocket ()) {
  }

  /*!
   *
   */
  Socket::Socket (const TEbyte bFamily) : priv (new PSocket (bFamily)) {
  }

  /*!
   *
   */
  Socket::~Socket () {
  }

  /*!
   *
   */
  void Socket::Close () {
    priv->Close ();
  }

  /*!
   *
   */
  TEint Socket::Bind (const TEushort usPort) {
    return priv->Bind (usPort);
  }

  /*!
   *
   */
  shared_ptr<TEchar> Socket::GetAddressText () {
    return priv->GetAddressText ();
  }

  /*!
   *
   */
  TEint Socket::Receive (shared_ptr<TEbyte> pBuffer, const TEuint nBufferSize, shared_ptr<TEchar> pszNodeName) {
    return priv->Receive (pBuffer, nBufferSize, pszNodeName);
  }

  /*!
   *
   */
  TEint Socket::Send (const shared_ptr<TEbyte> pBuffer, const TEuint nBufferSize) {
    return priv->Send (pBuffer, nBufferSize);
  }
 }
