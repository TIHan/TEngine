#include <stdio.h>
#include "network.hpp"
#include "socket.hpp"
#include "assert.hpp"

namespace dgp {
  network::network (dgpUshort usPort) {
    m_pSocket = new socket ();
    m_pSocket->bind (usPort);
    m_pSocket->send ("LOLOL", "localhost", "4767");
    m_pSocket->receive ((dgpChar *)m_bStream);
  }

  network::~network () {
    delete m_pSocket;
  }

  void network::printAddresses () {
    dgpChar ip[IP_STRLEN];
    dgpChar ip6[IP6_STRLEN];

    assertReturn(m_pSocket)

    m_pSocket->getAddressText (ip, ip6);

    if (ip[0]) {
      printf ("IPv4: %s\n", ip);
    }

    if (ip6[0]) {
      printf ("IPv6: %s\n", ip6);
    }
  }
}
