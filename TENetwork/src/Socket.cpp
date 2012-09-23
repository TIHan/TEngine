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
#include "Socket.hpp"
#include "Messages.hpp"

/*!
 *
 */
static TE::TEint CreateSocket(TE::TEint domain, TE::TEint type, TE::TEint protocol) {
  return socket (domain, type, protocol);
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
  /*!
   *
   */
  void Socket::Initialize (const TEbyte bFamily) {
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
  Socket::Socket () {
    Initialize (IPV4);
  }

  /*!
   *
   */
  Socket::Socket (const TEbyte bFamily) {
    Initialize (bFamily);
  }

  /*!
   *
   */
  Socket::~Socket () {
    Close ();
#ifdef _MSC_VER
    WSACleanup ();
#endif
  }

  /*!
   *
   */
  void Socket::Close () {
    freeaddrinfo (m_pAddressInfo);
    WARNING_IF(CloseSocket (m_iSocket) != 0, "Unable to close socket.")
  }

  /*!
   *
   */
  TEint Socket::Bind (const TEushort usPort) {
    if (m_iSocket != -1 && m_pAddress) {
          struct sockaddr_in *sockaddr = (struct sockaddr_in *)m_pAddress->ai_addr;
          sockaddr->sin_port = ntohs (usPort);
      
      WARNING_IF_RETURN_VAL_FORMAT(
        BindSocket(m_iSocket, m_pAddress->ai_addr, m_pAddress->ai_addrlen) != 0, 
        -1, 
        "Unable to associate socket with port %i.", usPort)
    }
    return 0;
  }

  /*!
   *
   */
  TEchar* Socket::GetAddressText () {
    TEchar *address = new TEchar[IP_STRLEN];

    if (m_pAddress) {
      switch (m_bFamily) {
      case AF_INET6:
        inet_ntop (m_bFamily, &(((struct sockaddr_in6 *)m_pAddress->ai_addr)->sin6_addr), address, IP_STRLEN);
        break;
      default:
        inet_ntop (m_bFamily, &(((struct sockaddr_in *)m_pAddress->ai_addr)->sin_addr), address, IP_STRLEN);
        break;
      }
      return address;
    } else {
      return 0;
    }
  }

  /*!
   *
   */
  TEint Socket::Receive (TEbyte *pBuffer, const TEuint nBufferSize) {
    struct sockaddr_storage sock_addr;
    socklen_t addr_len = sizeof sock_addr;
    
    TEint bytes = recvfrom (m_iSocket, (TEchar *)pBuffer, nBufferSize, 0, (sockaddr *)&sock_addr, &addr_len);
    WARNING_IF_RETURN_VAL(bytes == -1, -1, "Failed to receive packet.")
    MESSAGE_FORMAT ("Got %i bytes.\n", bytes);
    return bytes;
  }

  /*!
   *
   */
  TEint Socket::Send (const TEbyte *pBuffer, const TEuint nBufferSize, const TEchar *szNodeName, const TEchar *szServiceName) {
    int sendfd, bytes;
    struct addrinfo hints, *addrinfo;

    memset (&hints, 0, sizeof hints);
    hints.ai_family = m_bFamily;
    hints.ai_socktype = SOCK_DGRAM;

    WARNING_IF_RETURN_VAL(getaddrinfo (szNodeName, szServiceName, &hints, &addrinfo) != 0,
      -1,
      "Can't get address info.")

    sendfd = CreateSocket (addrinfo->ai_family, addrinfo->ai_socktype, addrinfo->ai_protocol);
    bytes = sendto (sendfd, (const TEchar *)pBuffer, nBufferSize, 0, addrinfo->ai_addr, addrinfo->ai_addrlen);

    // [WS] Close Socket and free address info.
    freeaddrinfo (addrinfo);
    CloseSocket (sendfd);

    WARNING_IF(bytes == -1, "Failed to send packet.")
    MESSAGE_FORMAT ("Sent %i bytes.\n", bytes);
    return bytes;
  }
 }
