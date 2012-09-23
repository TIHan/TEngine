/*
  Copyright (c) 2012, William F. Smith
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation 
  and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
  THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include "ByteStream.hpp"
#include "Socket.hpp"
#include "Network.hpp"
#include "Messages.hpp"

namespace TE {
  Network::Network (const TEushort usPort) {
    TEint byteSize;

    m_pSocket = new Socket (IPV4);
    m_pByteStream = new ByteStream (512);

    m_pSocket->Bind (usPort);
    m_pByteStream->WriteString ("LOHELLHELLOHELLOH");
    m_pByteStream->Write<TEint> (5);

    TEbyte *sendBuffer = m_pByteStream->GetCopyOfStream ();
    m_pSocket->Send (sendBuffer, m_pByteStream->GetSize (), "localhost", "4767");
    delete [] sendBuffer;

    m_pByteStream->Clear ();

    TEuint maxSize = m_pByteStream->GetMaxSize ();
    TEbyte *receiveBuffer = new TEbyte[maxSize];
    byteSize = m_pSocket->Receive (receiveBuffer, maxSize);

    m_pByteStream->WriteStream (receiveBuffer, byteSize);
    delete [] receiveBuffer;
    TEchar *HEY = m_pByteStream->ReadString ();
    MESSAGE_FORMAT ("SAY: %s\n", HEY);
    delete [] HEY;
    delete m_pByteStream;
  }

  Network::~Network () {
    delete m_pSocket;
  }

  void Network::PrintAddresses () {
    TEchar *ip = m_pSocket->GetAddressText ();

    if (ip[0]) {
      printf ("IP: %s\n", ip);
      delete [] ip;
    }
  }

  void Network::Connect (const TEchar *szAddress, const TEchar *szPort) {
   // m_pSocket->send ("connect", szAddress, szPort);
  }
}
