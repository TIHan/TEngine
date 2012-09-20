#include <stdio.h>
#include "network.hpp"
#include "socket.hpp"
#include "assert.hpp"
#include "bytestream.hpp"

namespace dgp {
  network::network (dgpUshort usPort) {
    dgpUint byteSize;

    m_pSocket = new socket (IPV4);
    m_pByteStream = new byteStream ();

    m_pSocket->bind (usPort);
   // m_pByteStream->write<dgpInt> (-18);
    m_pByteStream->writeString ("HELLO");
    m_pSocket->send (m_pByteStream->getStream (), m_pByteStream->getSize (), "localhost", "4767");
    m_pByteStream->unrefStream ();
    m_pByteStream->clear ();
    byteSize = m_pSocket->receive (m_pByteStream->getStream (), MAX_BUFFER);
    m_pByteStream->setSize (byteSize);
    dgpChar *HEY = m_pByteStream->readString ();
    //dgpInt MY_BYTE = m_pByteStream->read<dgpInt> ();
    //printf ("my byte: %i\n", MY_BYTE);
    printf ("SAY: %s\n", HEY);
    delete [] HEY;
    m_pByteStream->unrefStream ();
    delete m_pByteStream;
  }

  network::~network () {
    delete m_pSocket;
  }

  void network::printAddresses () {
    dgpChar ip[IP_STRLEN];

    ASSERT_RETURN(m_pSocket)

    m_pSocket->getAddressText (ip);

    if (ip[0]) {
      printf ("IP: %s\n", ip);
    }
  }

  void network::connect (const dgpChar *szAddress, const dgpChar *szPort) {
   // m_pSocket->send ("connect", szAddress, szPort);
  }
}
