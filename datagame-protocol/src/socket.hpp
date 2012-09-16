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

namespace dgp {
  class socket {
    dgpInt m_iSocket;
    dgpInt m_iSocket6;
    struct addrinfo *m_pAddressInfo;
    struct addrinfo *m_pAddress;
    struct addrinfo *m_pAddress6;
    dgpUshort m_usPort;

  public:
    socket ();
    ~socket ();

    void close ();
    dgpInt bind (dgpUshort usPort);

  };
}

#endif // SOCKET_HPP