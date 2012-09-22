#include <stdio.h>
#include "ByteStream.hpp"
#include "Socket.hpp"
#include "Network.hpp"
#include "Messages.hpp"

namespace TE {
  Network::Network (TEushort usPort) {
    TEint byteSize;

    m_pSocket = new Socket (IPV4);
    m_pByteStream = new ByteStream (512);

    m_pSocket->Bind (usPort);
    m_pByteStream->WriteString ("LOHELLHELLOHELLOH");

    TEbyte *sendBuffer = m_pByteStream->GetCopyOfStream ();
    m_pSocket->Send (sendBuffer, m_pByteStream->GetSize (), "localhost", "4767");
    delete sendBuffer;

    m_pByteStream->Clear ();

    TEuint maxSize = m_pByteStream->GetMaxSize ();
    TEbyte *receiveBuffer = new TEbyte[maxSize];
    byteSize = m_pSocket->Receive (receiveBuffer, maxSize);

    m_pByteStream->WriteStream (receiveBuffer, byteSize);
    delete receiveBuffer;
    TEchar *HEY = m_pByteStream->ReadString ();
    printf ("SAY: %s\n", HEY);
    delete [] HEY;
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
