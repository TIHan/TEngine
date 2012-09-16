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
    dgpChar ip[IP_STRLEN];
    dgpChar ip6[IP6_STRLEN];

    m_pSocket->getAddressText (ip, ip6);

    if (ip)
      printf ("IPv4: %s\n", ip);

    if (ip6)
      printf ("IPv6: %s\n", ip6);
  }
}
