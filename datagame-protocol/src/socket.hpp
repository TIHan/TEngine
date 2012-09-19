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

#define IP_STRLEN INET6_ADDRSTRLEN
#define IPV4 AF_INET
#define IPV6 AF_INET6

namespace dgp {
  class socket {
    dgpInt m_iSocket;
    struct addrinfo *m_pAddressInfo;
    struct addrinfo *m_pAddress;
    dgpByte m_bFamily;

    void initialize (dgpByte bFamily);
  public:
    socket ();
    explicit socket (dgpByte bFamily);
    ~socket ();

    void close ();
    dgpInt bind (dgpUshort usPort);
    void getAddressText (dgpChar *pszAddress);
    dgpInt receive (dgpByte *pBuffer, const size_t bufferSize);
    dgpInt send (dgpByte *pBuffer, const size_t bufferSize, const dgpChar *szNodeName, const dgpChar *szServiceName);
  };
}

#endif // SOCKET_HPP
