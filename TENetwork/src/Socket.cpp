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

#include <stdio.h>
#include "Types.hpp"
#include "Socket.hpp"
#include "Messages.hpp"

// createSocket
static TE::TEint CreateSocket(TE::TEint domain, TE::TEint type, TE::TEint protocol) {
  return socket (domain, type, protocol);
}

// bindSocket
static TE::TEint BindSocket(TE::TEint sockfd, struct sockaddr *my_addr, TE::TEint addrlen) {
  return bind (sockfd, my_addr, addrlen);
}

// closeSocket
static TE::TEint CloseSocket (TE::TEint sockfd) {
#ifdef __GNUC__
    return close (sockfd);
#elif _MSC_VER
    return closesocket (sockfd);
#endif
}

namespace TE {
  void Socket::Initialize (TEbyte bFamily) {
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
    if (wsaResult != 0) {
      ERROR_MESSAGE_FORMAT("WSAStartup failed with result %i", wsaResult)
    }
#endif
  
    memset (&hints, 0, sizeof hints);
    hints.ai_family = m_bFamily;
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo (NULL, "", &hints, &m_pAddressInfo) != 0) {
      ERROR_MESSAGE("Unable to get address info")
    }

    for (p = m_pAddressInfo; p != NULL; p = p->ai_next) {
      if (p->ai_family == m_bFamily) {
        m_iSocket = CreateSocket (p->ai_family, p->ai_socktype, p->ai_protocol);
        m_pAddress = p;
      }
    }

    if (m_iSocket == -1) {
      ERROR_MESSAGE("Unable to create a Socket")
    }
  }
  //**************************************************
  // Socket
  Socket::Socket () {
    Initialize (IPV4);
  }

  Socket::Socket (TEbyte bFamily) {
    Initialize (bFamily);
  }

  //**************************************************
  // ~Socket
  Socket::~Socket () {
    Close ();
#ifdef _MSC_VER
    WSACleanup ();
#endif
  }

  //**************************************************
  // close
  void Socket::Close () {
    freeaddrinfo (m_pAddressInfo);
    if (CloseSocket (m_iSocket) != 0) {
      WARNING_MESSAGE("Unable to close Socket")
      return;
    }
  }

  //**************************************************
  // bind
  TEint Socket::Bind (TEushort usPort) {
    if (m_iSocket != -1 && m_pAddress) {
          struct sockaddr_in *sockaddr = (struct sockaddr_in *)m_pAddress->ai_addr;
          sockaddr->sin_port = ntohs (usPort);

      if (BindSocket (m_iSocket, m_pAddress->ai_addr, m_pAddress->ai_addrlen) != 0) {
        WARNING_MESSAGE_FORMAT("Unable to associated Socket with port %i", usPort)
        return -1;
      }
    }
    return 0;
  }

  //**************************************************
  // getAddressText
  void Socket::GetAddressText (TEchar *pszAddress) {
    if (m_pAddress) {
      switch (m_bFamily) {
      case AF_INET6:
        inet_ntop (m_bFamily, &(((struct sockaddr_in6 *)m_pAddress->ai_addr)->sin6_addr), pszAddress, IP_STRLEN);
        break;
      default:
        inet_ntop (m_bFamily, &(((struct sockaddr_in *)m_pAddress->ai_addr)->sin_addr), pszAddress, IP_STRLEN);
        break;
      }
    } else {
      pszAddress[0] = 0;
    }
  }

  //**************************************************
  // receive
  TEint Socket::Receive (TEbyte *pBuffer, const TEuint nBufferSize) {
    struct sockaddr_storage sock_addr;
    socklen_t addr_len = sizeof sock_addr;
    
    TEint bytes = recvfrom (m_iSocket, (TEchar *)pBuffer, nBufferSize, 0, (sockaddr *)&sock_addr, &addr_len);
    if (bytes == -1) {
      WARNING_MESSAGE("Failed to receive packet.")
      return bytes;
    }

    printf ("Got %i bytes.\n", bytes);
    return bytes;
  }

  //**************************************************
  // send
  TEint Socket::Send (TEbyte *pBuffer, const TEuint nBufferSize, const TEchar *szNodeName, const TEchar *szServiceName) {    
    int sendfd, bytes;
    struct addrinfo hints, *addrinfo;

    memset (&hints, 0, sizeof hints);
    hints.ai_family = m_bFamily;
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo (szNodeName, szServiceName, &hints, &addrinfo) != 0) {
      WARNING_MESSAGE("Can't get address info")
      return -1;
    }
    sendfd = CreateSocket (addrinfo->ai_family, addrinfo->ai_socktype, addrinfo->ai_protocol);
    bytes = sendto (sendfd, (const TEchar *)pBuffer, nBufferSize, 0, addrinfo->ai_addr, addrinfo->ai_addrlen);

    // [WS] Close Socket and free address info.
    freeaddrinfo (addrinfo);
    CloseSocket (sendfd);

    if (bytes == -1) {
      WARNING_MESSAGE("Failed to send packet.")
      return bytes;
    }

    printf ("Sent %i bytes.\n", bytes);
    return bytes;
  }
 }
