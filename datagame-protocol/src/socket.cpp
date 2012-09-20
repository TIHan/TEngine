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
#include "warning.hpp"

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

    ASSERT(bFamily == IPV4 || bFamily == IPV6)

    switch (bFamily) {
    case IPV6:
      m_bFamily = AF_INET6;
      break;
    default:
      m_bFamily = AF_INET;
    }

#ifdef _MSC_VER
    WSADATA wsaData;
    dgpInt wsaResult;

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
        m_iSocket = createSocket (p->ai_family, p->ai_socktype, p->ai_protocol);
        m_pAddress = p;
      }
    }

    if (m_iSocket == -1) {
      ERROR_MESSAGE("Unable to create a socket")
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
    ASSERT_RETURN(m_iSocket != -1 && m_pAddressInfo)

    freeaddrinfo (m_pAddressInfo);
    if (closeSocket (m_iSocket) != 0) {
      WARNING_MESSAGE("Unable to close socket")
      return;
    }
  }

  //**************************************************
  // bind
  dgpInt socket::bind (dgpUshort usPort) {
    ASSERT_RETURN_VAL(m_iSocket != -1 && m_pAddress != 0, -1)

    if (m_iSocket != -1 && m_pAddress) {
          struct sockaddr_in *sockaddr = (struct sockaddr_in *)m_pAddress->ai_addr;
          sockaddr->sin_port = ntohs (usPort);

      if (bindSocket (m_iSocket, m_pAddress->ai_addr, m_pAddress->ai_addrlen) != 0) {
        WARNING_MESSAGE_FORMAT("Unable to associated socket with port %i", usPort)
        return -1;
      }
    }
    return 0;
  }

  //**************************************************
  // getAddressText
  void socket::getAddressText (dgpChar *pszAddress) {
    ASSERT_RETURN(m_pAddress)

    if (m_pAddress) {
      switch (m_bFamily) {
      case AF_INET6:
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
  dgpInt socket::receive (dgpByte *pBuffer, const dgpUint nBufferSize) {
    ASSERT_RETURN_VAL(m_iSocket != -1, -1)

    struct sockaddr_storage sock_addr;
    socklen_t addr_len = sizeof sock_addr;
    
    dgpInt bytes = recvfrom (m_iSocket, (dgpChar *)pBuffer, nBufferSize, 0, (sockaddr *)&sock_addr, &addr_len);
    if (bytes == -1) {
      WARNING_MESSAGE("Failed to receive packet")
      return bytes;
    }

    printf ("Message received. Got %i bytes.\n", bytes);
    return bytes;
  }

  //**************************************************
  // send
  dgpInt socket::send (dgpByte *pBuffer, const dgpUint nBufferSize, const dgpChar *szNodeName, const dgpChar *szServiceName) {
    ASSERT_RETURN_VAL(m_iSocket != -1, -1)
    
    int sendfd, bytes;
    struct addrinfo hints, *addrinfo;

    memset (&hints, 0, sizeof hints);
    hints.ai_family = m_bFamily;
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo (szNodeName, szServiceName, &hints, &addrinfo) != 0) {
      WARNING_MESSAGE("Can't get address info")
      return -1;
    }
    sendfd = createSocket (addrinfo->ai_family, addrinfo->ai_socktype, addrinfo->ai_protocol);
    bytes = sendto (sendfd, (const dgpChar *)pBuffer, nBufferSize, 0, addrinfo->ai_addr, addrinfo->ai_addrlen);

    // [WS] Close socket and free address info.
    freeaddrinfo (addrinfo);
    closeSocket (sendfd);

    if (bytes == -1) {
      WARNING_MESSAGE("Failed to send packet")
      return bytes;
    }

    printf ("Message sent. Got %i bytes.\n", bytes);
    return bytes;
  }
 }
