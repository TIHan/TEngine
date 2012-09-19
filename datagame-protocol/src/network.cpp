#include <stdio.h>
#include "network.hpp"
#include "socket.hpp"
#include "assert.hpp"

namespace dgp {
  network::network (dgpUshort usPort) {
    m_pSocket = new socket (IPV4);
    m_pSocket->bind (usPort);
    m_pSocket->send ("hello", "localhost", "4767");
    m_pSocket->receive ((dgpChar *)m_bStream);
  }

  network::~network () {
    delete m_pSocket;
  }

  void network::printAddresses () {
    dgpChar ip[IP_STRLEN];

    assertReturn(m_pSocket)

    m_pSocket->getAddressText (ip);

    if (ip[0]) {
      printf ("IP: %s\n", ip);
    }
  }

  void network::connect (const dgpChar *szAddress, const dgpChar *szPort) {
    m_pSocket->send ("connect", szAddress, szPort);
  }
}
