#include <stdio.h>
#include "ByteStream.hpp"
#include "Socket.hpp"
#include "Network.hpp"
#include "Messages.hpp"

namespace TE {
  Network::Network (TEushort usPort) {
    TEint byteSize;

    m_pSocket = new Socket (IPV4);
    m_pByteStream = new ByteStream ();

    m_pSocket->Bind (usPort);
    m_pByteStream->WriteString ("LOHELLHELLOHELLOH");
    m_pSocket->Send (m_pByteStream->GetStream (), m_pByteStream->GetSize (), "localhost", "4767");
    m_pByteStream->UnrefStream ();
    m_pByteStream->Clear ();
    byteSize = m_pSocket->Receive (m_pByteStream->GetStream (), MAX_BUFFER);
    m_pByteStream->SetSize (byteSize);
    TEchar *HEY = m_pByteStream->ReadString ();
    printf ("SAY: %s\n", HEY);
    delete [] HEY;
    m_pByteStream->UnrefStream ();
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
