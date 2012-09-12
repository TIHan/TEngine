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
#include "network.hpp"

namespace dgp {
  network::network (dgpUshort usPort) {
    dgpAddressInfo hints, *res, *p;
    dgpChar portbuf[sizeof usPort];
    m_iFileDescriptorSocket = -1;
    m_iFileDescriptorSocket6 = -1;
    m_pAddress = NULL;
    m_pAddress6 = NULL;

#ifdef _MSC_VER
    WSADATA wsaData;
    dgpInt wsaResult;

    // Initialize Winsock
    wsaResult = WSAStartup (MAKEWORD(2,2), &wsaData);
    if (wsaResult != 0) {
      printf("(network) Error: WSAStartup failed: %d\n", wsaResult);
      return;
    }
#endif
  
    memset (&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;          /* IPv4 and/or IPv6 */
    hints.ai_socktype = SOCK_DGRAM;       /* User Datagram Protocol */
  
    if (getaddrinfo (NULL, "46767", &hints, &res) != 0) {
      fprintf (stderr, "(network) Error: Unable to get address info.\n");
      return;
    }
  
    for (p = res; p != NULL; p = p->ai_next) {
      if (p->ai_family == AF_INET) {
        m_iFileDescriptorSocket = socket (p->ai_family, p->ai_socktype, p->ai_protocol);
        m_pAddress = (struct sockaddr_in *)p->ai_addr;
      } else if (p->ai_family == AF_INET6) {
        m_iFileDescriptorSocket6 = socket (p->ai_family, p->ai_socktype, p->ai_protocol);
        m_pAddress6 = (struct sockaddr_in6 *)p->ai_addr;
      }
    }
  
    if (m_iFileDescriptorSocket == -1 && m_iFileDescriptorSocket6 == -1) {
      fprintf (stderr, "(network) Error: Unable to create a socket.\n");
      return;
    }
  }

  network::~network () {
    if (m_iFileDescriptorSocket != -1) {
      if (this->close (m_iFileDescriptorSocket) != 0)
        fprintf (stderr, "(~network) Error: Unable to close IPv4 socket.\n");
    }

    if (m_iFileDescriptorSocket6 != -1) {
      if (this->close (m_iFileDescriptorSocket6) != 0)
        fprintf (stderr, "(~network) Error: Unable to close IPv6 socket.\n");
    }
  }

  dgpInt network::close (dgpInt sockfd) {
#ifdef __GNUC__
    return close (sockfd);
#elif _MSC_VER
    return closesocket (sockfd);
#endif
  }

  void network::printAddresses () {
    dgpChar ip[INET6_ADDRSTRLEN];

    if (m_pAddress) {
      inet_ntop (AF_INET, &m_pAddress->sin_addr, ip, sizeof ip);
      printf ("IPv4: %s\n", ip);
    }

    if (m_pAddress6) {
      inet_ntop (AF_INET6, &m_pAddress6->sin6_addr, ip, sizeof ip);
      printf ("IPv6: %s\n", ip);
    }
  }
}
