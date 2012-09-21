#ifndef __SOCKET_HPP_
#define __SOCKET_HPP_

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

#define IP_STRLEN INET6_ADDRSTRLEN

namespace TE {
  enum SocketFamily {
    IPV4,
    IPV6
  };

  class Socket {
    TEint m_iSocket;
    struct addrinfo *m_pAddressInfo;
    struct addrinfo *m_pAddress;
    TEbyte m_bFamily;

    void Initialize (TEbyte bFamily);
  public:
    Socket ();
    explicit Socket (TEbyte bFamily);
    ~Socket ();

    void Close ();
    TEint Bind (TEushort usPort);
    void GetAddressText (TEchar *pszAddress);
    TEint Receive (TEbyte *pBuffer, const TEuint nBufferSize);
    TEint Send (TEbyte *pBuffer, const TEuint nBufferSize, const TEchar *szNodeName, const TEchar *szServiceName);
  };
}

#endif /* __SOCKET_HPP_ */
