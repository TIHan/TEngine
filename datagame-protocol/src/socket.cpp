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
#include "types.hpp"
#include "socket.hpp"
#include "assert.hpp"
#include "error.hpp"

// createSocket
static dgp::dgpInt createSocket(dgp::dgpInt domain, dgp::dgpInt type, dgp::dgpInt protocol) {
  return socket (domain, type, protocol);
}

// bindSocket
static dgp::dgpInt bindSocket(dgp::dgpInt sockfd, struct sockaddr *my_addr, dgp::dgpInt addrlen) {
  return bind (sockfd, my_addr, addrlen);
}

// closeSocket
static dgp::dgpInt closeSocket (dgp::dgpInt sockfd) {
#ifdef __GNUC__
    return close (sockfd);
#elif _MSC_VER
    return closesocket (sockfd);
#endif
}

namespace dgp {
  void socket::initialize (dgpByte bFamily) {
    struct addrinfo hints, *p;
    m_iSocket = -1;
    m_pAddress = 0;
    m_bFamily = bFamily;

#ifdef _MSC_VER
    WSADATA wsaData;
    dgpInt wsaResult;

    // Initialize Winsock
    wsaResult = WSAStartup (MAKEWORD(2,2), &wsaData);
    if (wsaResult != 0) {
      ERROR_MESSAGE_FORMAT("WSAStartup failed with result %i", wsaResult)
      return;
    }
#endif
  
    memset (&hints, 0, sizeof hints);
    hints.ai_family = m_bFamily;
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo (NULL, "", &hints, &m_pAddressInfo) != 0) {
      ERROR_MESSAGE("Unable to get address info")
      return;
    }

    for (p = m_pAddressInfo; p != NULL; p = p->ai_next) {
      if (p->ai_family == m_bFamily) {
        m_iSocket = createSocket (p->ai_family, p->ai_socktype, p->ai_protocol);
        m_pAddress = p;
      }
    }

    if (m_iSocket == -1) {
      ERROR_MESSAGE("Unable to create a socket")
      return;
    }
  }
  //**************************************************
  // socket
  socket::socket () {
    initialize (IPV4);
  }

  socket::socket (dgpByte bFamily) {
    initialize (bFamily);
  }

  //**************************************************
  // ~socket
  socket::~socket () {
    close ();
#ifdef _MSC_VER
    WSACleanup ();
#endif
  }

  //**************************************************
  // close
  void socket::close () {
    assertReturn(m_iSocket != -1 && m_pAddressInfo)

    freeaddrinfo (m_pAddressInfo);
    if (closeSocket (m_iSocket) != 0) {
      ERROR_MESSAGE("Unable to close socket")
      return;
    }
  }

  //**************************************************
  // bind
  dgpInt socket::bind (dgpUshort usPort) {
    assertReturnVal(m_iSocket != -1 && m_pAddress != 0, -1)

    if (m_iSocket != -1 && m_pAddress) {
          struct sockaddr_in *sockaddr = (struct sockaddr_in *)m_pAddress->ai_addr;
          sockaddr->sin_port = ntohs (usPort);

      if (bindSocket (m_iSocket, m_pAddress->ai_addr, m_pAddress->ai_addrlen) != 0) {
        ERROR_MESSAGE_FORMAT("Unable to associated socket with port %i", usPort)
        return -1;
      }
    }
    return 0;
  }

  //**************************************************
  // getAddressText
  void socket::getAddressText (dgpChar *pszAddress) {
    assertReturn(m_pAddress)

    if (m_pAddress) {
      switch (m_bFamily) {
      case IPV6:
        inet_ntop (m_bFamily, &(((struct sockaddr_in6 *)m_pAddress->ai_addr)->sin6_addr), pszAddress, IP_STRLEN);
        break;
      default:
        inet_ntop (m_bFamily, &(((struct sockaddr_in *)m_pAddress->ai_addr)->sin_addr), pszAddress, IP_STRLEN);
        break;
      }
    }
    else {
      pszAddress[0] = 0;
    }
  }

  //**************************************************
  // receive
  dgpInt socket::receive (dgpByte *pBuffer, const size_t bufferSize) {
    assertReturnVal(m_iSocket != -1, -1)

    struct sockaddr_storage sock_addr;
    socklen_t addr_len = sizeof sock_addr;
    
    dgpInt bytes = recvfrom (m_iSocket, (dgpChar *)pBuffer, bufferSize, 0, (sockaddr *)&sock_addr, &addr_len);
    if (bytes == -1) {
      ERROR_MESSAGE("Failed to receive packet")
      return -1;
    }

    printf ("Message received. Got %i bytes.\n", bytes);
    return 0;
  }

  //**************************************************
  // send
  dgpInt socket::send (dgpByte *pBuffer, const size_t bufferSize, const dgpChar *szNodeName, const dgpChar *szServiceName) {
    assertReturnVal(m_iSocket != -1, -1)
    
    int sendfd, bytes;
    struct addrinfo hints, *addrinfo;

    memset (&hints, 0, sizeof hints);
    hints.ai_family = m_bFamily;
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo (szNodeName, szServiceName, &hints, &addrinfo) != 0) {
      ERROR_MESSAGE("Can't get address info")
      return -1;
    }
    sendfd = createSocket (addrinfo->ai_family, addrinfo->ai_socktype, addrinfo->ai_protocol);

    bytes = sendto (sendfd, (const dgpChar *)pBuffer, bufferSize, 0, addrinfo->ai_addr, addrinfo->ai_addrlen);
    if (bytes == -1) {
      ERROR_MESSAGE("Failed to send packet")
      return -1;
    }
    printf ("Message sent. Got %i bytes.\n", bytes);

    // [WS] Close socket and free address info.
    freeaddrinfo (addrinfo);
    closeSocket (sendfd);
    return 0;
  }
 }
