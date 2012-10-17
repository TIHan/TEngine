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
    PNetwork ();
    ~PNetwork ();

    void PrintAddresses ();
    void Host (TEushort usPort);
    bool Send (shared_ptr<IPacket> pPacket);
    shared_ptr<Packet> Receive (string &szIp);
    void Setup (TEuint nMaxTransUnit, TEbyte bFamily, string szNodeName, string szServiceName);
  };

  /*!
   *
   */
  PNetwork::PNetwork () {
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
    shared_ptr<TEchar> ip (m_pSocket->GetAddressText ());
    cout << ip << endl;
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
  bool PNetwork::Send (shared_ptr<IPacket> pPacket) {
    return m_pSocket->Send (pPacket->GetByteStream ()->GetStream (), pPacket->GetSize ()) == -1 ? false : true;
  }

  /*!
   *
   */
  shared_ptr<Packet> PNetwork::Receive (string &szIp) {
    shared_ptr<ByteStream> byteStream;
    shared_ptr<Packet> packet;
    TEint bytes;
    shared_ptr<TEchar> ip (new TEchar[256], default_delete<TEchar[]> ());
    shared_ptr<TEbyte> receiveBuffer (new TEbyte[m_nMaxTransUnit], default_delete<TEbyte[]> ());

    bytes = m_pSocket->Receive (receiveBuffer, m_nMaxTransUnit, ip);
    if (bytes == -1) {
      return 0;
    }

    byteStream.reset (new ByteStream (bytes));
    byteStream->WriteStream (receiveBuffer, bytes);
    packet.reset (new Packet (byteStream));
    if (packet->HasError ()) {
      return 0;
    }
    szIp.assign (ip.get ());
    return packet;
  }

  /*!
   *
   */
  void PNetwork::Setup (TEuint nMaxTransUnit, TEbyte bFamily, string szNodeName, string szServiceName) {
    m_nMaxTransUnit = nMaxTransUnit;
    m_pSocket.reset (new Socket (bFamily, szNodeName, szServiceName));
  }

  /****************************************************************************************************************************
  *****************************************************************************************************************************
  *****************************************************************************************************************************
  ****************************************************************************************************************************/

  /*!
   *
   */
  Network::Network () :
    priv (new PNetwork ()) {
  }

  /*!
   *
   */
  Network::~Network () {
    priv->~PNetwork ();
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
  bool Network::Send (shared_ptr<IPacket> pPacket) {
    return priv->Send (pPacket);
  }

  /*!
   *
   */
  shared_ptr<Packet> Network::Receive (string &szIp) {
    return priv->Receive (szIp);
  }

  /*!
   *
   */
  void Network::Setup (TEuint nMaxTransUnit, TEbyte bFamily, string szNodeName, string szServiceName) {
    priv->Setup (nMaxTransUnit, bFamily, szNodeName, szServiceName);
  }
}
