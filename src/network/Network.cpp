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

#include "Network.hpp"
#include "Socket.hpp"

namespace TE {
  class PNetwork : public INetwork {
    ISocket *m_pSocket;
    TEuint m_nMaxTransUnit;

  public:
    explicit PNetwork (TEuint nMaxTransUnit);
    ~PNetwork ();

    void PrintAddresses ();
    void Host (TEushort usPort);
    void Send (IPacket *pPacket);
    IPacket* Receive ();
  };

  /*!
   *
   */
  PNetwork::PNetwork (TEuint nMaxTransUnit) : m_pSocket (new Socket()) {
    m_nMaxTransUnit = nMaxTransUnit;
  }

  /*!
   *
   */
  PNetwork::~PNetwork () {
    delete m_pSocket;
  }

  /*!
   *
   */
  void PNetwork::PrintAddresses () {
    TEchar *ip = m_pSocket->GetAddressText ();

    if (ip[0]) {
      printf ("IP: %s\n", ip);
      delete [] ip;
    }
  }

  /*!
   *
   */
  void PNetwork::Host (TEushort usPort) {
    m_pSocket->Bind (usPort);
  }

  /*!
   *
   */
  void PNetwork::Send (IPacket *pPacket) {
    TEbyte *send = pPacket->GetCopyOfStream ();
    m_pSocket->Send (send, pPacket->GetSize (), pPacket->GetAddress ().c_str (), pPacket->GetPort ().c_str ());
    delete send;
  }

  /*!
   *
   */
  IPacket* PNetwork::Receive () {
    Packet *packet;
    TEuint bytes;
    TEchar *ip = new TEchar[256];
    TEchar *port = new TEchar[256];
    TEbyte *receiveBuffer = new TEbyte[m_nMaxTransUnit];

    bytes = m_pSocket->Receive (receiveBuffer, m_nMaxTransUnit, ip, port);
    packet = new Packet (receiveBuffer, m_nMaxTransUnit, bytes, ip, port);
    delete [] receiveBuffer;
    delete [] port;
    delete [] ip;

    if (packet->HasError ()) {
      delete packet;
      return 0;
    }
    return packet;
  }

  /****************************************************************************************************************************
  *****************************************************************************************************************************
  *****************************************************************************************************************************
  ****************************************************************************************************************************/

  /*!
   *
   */
  Network::Network (TEuint nMaxTransUnit) : priv (new PNetwork (nMaxTransUnit)) {
  }

  /*!
   *
   */
  Network::~Network () {
    delete priv;
  }

  /*!
   *
   */
  void Network::PrintAddresses () {
    priv->PrintAddresses ();
  }

  /*!
   *
   */
  void Network::Host (TEushort usPort) {
    priv->Host (usPort);
  }

  /*!
   *
   */
  void Network::Send (IPacket *pPacket) {
    priv->Send (pPacket);
  }

  /*!
   *
   */
  IPacket* Network::Receive () {
    return priv->Receive ();
  }
}