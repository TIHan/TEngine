#include <stdio.h>
#include "ByteStream.hpp"
#include "Socket.hpp"
#include "Network.hpp"
#include "Messages.hpp"

namespace TE {
  Network::Network (TEushort usPort) {
    TEuint byteSize;

    m_pSocket = new Socket (IPV4);
    m_pByteStream = new ByteStream ();

    m_pSocket->Bind (usPort);
    m_pByteStream->writeString ("LOHELLOHELLOHELLOHELLOHELLLOELLOHELLLOHELLOHELLOHELLOHELLOHELLOHELLLOELLOHELLLOHELLOHELLOHELLOHELLOHELLOHELLLOHELLOHELLOHELLOHELLOHELLOHELLLOHELLOHELLOHELLOHELLOHELLOHELLLOHELLOHELLOHELLOHELLOHELLOHELLLOHELLOHELLOHELLOHELLOHELLOHELLLOHELLOHELLOHELLOHELLOHELLOHELLLOHELLOHELLOHELLOHELLOHELLOHELLLOHELLOHELLOHELLOHELLOHELLOHELLLOHELLOHELLOHELLOHELLOHELLOHELLLOHELLOHELLOHELLOHELLOHELLOHELLLOHELLOHELLOHELLOHELLOHELLOHELLLOHELLOHELLOHELLOHELLOHELLOHELLLOHELLOHELLOHELLOHELLOHELLOHELLOHELLOHELLOHELLOHELLOHELLOHELLO");
    m_pSocket->Send (m_pByteStream->getStream (), m_pByteStream->getSize (), "localhost", "4767");
    m_pByteStream->unrefStream ();
    m_pByteStream->clear ();
    byteSize = m_pSocket->Receive (m_pByteStream->getStream (), MAX_BUFFER);
    m_pByteStream->setSize (byteSize);
    TEchar *HEY = m_pByteStream->readString ();
    printf ("SAY: %s\n", HEY);
    delete [] HEY;
    m_pByteStream->unrefStream ();
    delete m_pByteStream;
  }

  Network::~Network () {
    delete m_pSocket;
  }

  void Network::PrintAddresses () {
    TEchar ip[IP_STRLEN];
    m_pSocket->GetAddressText (ip);

    if (ip[0]) {
      printf ("IP: %s\n", ip);
    }
  }

  void Network::Connect (const TEchar *szAddress, const TEchar *szPort) {
   // m_pSocket->send ("connect", szAddress, szPort);
  }
}
