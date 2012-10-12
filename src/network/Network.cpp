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
    unique_ptr<ISocket> m_pSocket;
    TEuint m_nMaxTransUnit;

  public:
    explicit PNetwork (TEuint nMaxTransUnit);
    ~PNetwork ();

    void PrintAddresses ();
    void Host (TEushort usPort);
    void Send (shared_ptr<IPacket> pPacket);
    shared_ptr<Packet> Receive ();
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
  void PNetwork::Send (shared_ptr<IPacket> pPacket) {
    m_pSocket->Send (pPacket->GetByteStream ()->GetStream (), pPacket->GetSize ());
  }

  /*!
   *
   */
  shared_ptr<Packet> PNetwork::Receive () {
    shared_ptr<ByteStream> byteStream;
    shared_ptr<Packet> packet;
    TEuint bytes;
    TEchar *ip = new TEchar[256];
    TEchar *port = new TEchar[256];
    TEbyte *receiveBuffer = new TEbyte[m_nMaxTransUnit];

    bytes = m_pSocket->Receive (receiveBuffer, m_nMaxTransUnit, ip, port);
    byteStream.reset (new ByteStream (bytes));
    byteStream->WriteStream (receiveBuffer, bytes);
    packet.reset (new Packet (byteStream));

    delete [] ip;
    delete [] port;
    delete [] receiveBuffer;
    if (packet->HasError ()) {
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
  void Network::Send (shared_ptr<IPacket> pPacket) {
    priv->Send (pPacket);
  }

  /*!
   *
   */
  shared_ptr<Packet> Network::Receive () {
    return priv->Receive ();
  }
}
