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
dgp::dgpInt createSocket(dgp::dgpInt domain, dgp::dgpInt type, dgp::dgpInt protocol) {
  return socket (domain, type, protocol);
}

// bindSocket
dgp::dgpInt bindSocket(dgp::dgpInt sockfd, struct sockaddr *my_addr, dgp::dgpInt addrlen) {
  return bind (sockfd, my_addr, addrlen);
}

// closeSocket
dgp::dgpInt closeSocket (dgp::dgpInt sockfd) {
#ifdef __GNUC__
    return close (sockfd);
#elif _MSC_VER
    return closesocket (sockfd);
#endif
}

namespace dgp {

  // socket
  socket::socket () {
    struct addrinfo hints, *p;
    m_iSocket = -1;
    m_iSocket6 = -1;
    m_pAddress = 0;
    m_pAddress6 = 0;

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
    hints.ai_family = AF_UNSPEC;          /* IPv4 and/or IPv6 */
    hints.ai_socktype = SOCK_DGRAM;       /* User Datagram Protocol */
  
    if (getaddrinfo (NULL, "1", &hints, &m_pAddressInfo) != 0) {
      ERROR_MESSAGE("Unable to get address info")
      return;
    }
  
    for (p = m_pAddressInfo; p != NULL; p = p->ai_next) {
      if (p->ai_family == AF_INET) {
        m_iSocket = createSocket (p->ai_family, p->ai_socktype, p->ai_protocol);
        m_pAddress = p;
      } else if (p->ai_family == AF_INET6) {
        m_iSocket6 = createSocket (p->ai_family, p->ai_socktype, p->ai_protocol);
        m_pAddress6 = p;
      }
    }
  
    if (m_iSocket == -1 && m_iSocket6 == -1) {
      ERROR_MESSAGE("Unable to create a socket")
      exit (1);
    }
  }

  // ~socket
  socket::~socket () {
    close ();
#ifdef _MSC_VER
    WSACleanup ();
#endif
  }

  // close
  void socket::close () {
    assertReturn(m_pAddress)
    assertReturn(m_iSocket != -1 || m_iSocket6 != -1)

    if (m_pAddressInfo) {
      freeaddrinfo (m_pAddressInfo);
    }

    if (m_iSocket != -1) {
      if (closeSocket (m_iSocket) != 0)
        ERROR_MESSAGE("Unable to close IPv4 socket")
    }

    if (m_iSocket6 != -1) {
      if (closeSocket (m_iSocket6) != 0)
        ERROR_MESSAGE("Unable to close IPv6 socket")
    }
  }

  // bind
  dgpInt socket::bind (dgpUshort usPort) {
    dgpInt success = 0;

    assertReturnVal((m_iSocket != -1 && m_pAddress) ||
      (m_iSocket6 != -1 && m_pAddress6), -1)

    if (m_iSocket != -1 && m_pAddress) {
          struct sockaddr_in *sockaddr = (struct sockaddr_in *)m_pAddress->ai_addr;
          sockaddr->sin_port = usPort;

      if (bindSocket (m_iSocket, m_pAddress->ai_addr, m_pAddress->ai_addrlen) != 0) {
        ERROR_MESSAGE_FORMAT("Unable to associated IPv4 socket with port %i", usPort)
        success = -1;
      }
    }

    if (m_iSocket6 != -1 && m_pAddress6) {
          struct sockaddr_in6 *sockaddr = (struct sockaddr_in6 *)m_pAddress6->ai_addr;
          sockaddr->sin6_port = usPort;

      if (bindSocket (m_iSocket6, m_pAddress6->ai_addr, m_pAddress6->ai_addrlen) != 0) {
        ERROR_MESSAGE_FORMAT("Unable to associated IPv4 socket with port %i", usPort)
        success = -1;
      }
    }
    return success;
  }

  // getAddressText
  void socket::getAddressText (dgpChar *pszAddress, dgpChar *pszAddress6) {
    assertReturn(m_pAddress || m_pAddress6)

    if (m_pAddress) {
      inet_ntop (AF_INET, &(((struct sockaddr_in *)m_pAddress->ai_addr)->sin_addr), pszAddress, IP_STRLEN);
    }
    else {
      pszAddress[0] = 0;
    }

    if (m_pAddress6) {
      inet_ntop (AF_INET6, &(((struct sockaddr_in6 *)m_pAddress6->ai_addr)->sin6_addr), pszAddress6, IP6_STRLEN);
    }
    else {
      pszAddress6[0] = 0;
    }
  }
 }