#include <stdio.h>
#include "network.hpp"
#include "socket.hpp"

namespace dgp {
  network::network (dgpUshort usPort) {
    m_pSocket = new socket ();
    m_pSocket->bind (usPort);
  }

  network::~network () {
    delete m_pSocket;
  }


  void network::printAddresses () {
    /*dgpChar ip[INET6_ADDRSTRLEN];

    if (m_pAddress) {
      inet_ntop (AF_INET, &m_pAddress->sin_addr, ip, sizeof ip);
      printf ("IPv4: %s\n", ip);
    }

    if (m_pAddress6) {
      inet_ntop (AF_INET6, &m_pAddress6->sin6_addr, ip, sizeof ip);
      printf ("IPv6: %s\n", ip);
    }*/
  }
}
