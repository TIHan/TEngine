#ifndef SOCKET_HPP
#define SOCKET_HPP

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

#include "types.hpp"

#define IP_STRLEN INET_ADDRSTRLEN
#define IP6_STRLEN INET6_ADDRSTRLEN

namespace dgp {
  class socket {
    dgpInt m_iSocket;
    dgpInt m_iSocket6;
    struct addrinfo *m_pAddressInfo;
    struct addrinfo *m_pAddress;
    struct addrinfo *m_pAddress6;

  public:
    socket ();
    ~socket ();

    void close ();
    dgpInt bind (dgpUshort usPort);
    void getAddressText (dgpChar *pszAddress, dgpChar *pszAddress6);

  };
}

#endif // SOCKET_HPP