#include <stdio.h>
#include "network.hpp"
#include "socket.hpp"
#include "assert.hpp"
#include "bytestream.hpp"

namespace dgp {
  network::network (dgpUshort usPort) {
    m_pSocket = new socket (IPV4);
    m_pByteStream = new byteStream ();

    m_pSocket->bind (usPort);
    m_pByteStream->writeByte (18);
    m_pByteStream->writeByte (80);
    m_pSocket->send (m_pByteStream->getStream (), m_pByteStream->getSize (), "localhost", "4767");
    m_pByteStream->unrefStream ();
    m_pByteStream->clear ();
    m_pSocket->receive (m_pByteStream->getStream (), MAX_BUFFER);
    dgpByte MY_BYTE = m_pByteStream->readByte ();
    dgpByte MY_BYTE2 = m_pByteStream->readByte ();
    printf ("my byte: %i\n", MY_BYTE);
    printf ("my byte2: %i\n", MY_BYTE2);
    m_pByteStream->unrefStream ();
    delete m_pByteStream;
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
   // m_pSocket->send ("connect", szAddress, szPort);
  }
}
