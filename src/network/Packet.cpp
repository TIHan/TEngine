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

#include "Packet.hpp"

namespace TE {
  class PPacket : public IPacket {
    IByteStream *m_pByteStream;
    string m_strAddress;
    string m_strPort;

  public:
    explicit PPacket (IByteStream *pByteStream, string strAddress, string strPort);
    explicit PPacket (TEbyte *pBuffer, TEuint nMaxSize, TEuint nSize, string strAddress, string strPort);
    ~PPacket ();

    bool HasError ();
    TEuint GetSize ();
    TEbyte* GetCopyOfStream ();
    string GetAddress ();
    string GetPort ();

    void SetAddress (string strAddress);
    void SetPort (string strPort);
  };

  /*!
   *
   */
  PPacket::PPacket (IByteStream *pByteStream, string strAddress, string strPort) {
    m_pByteStream = pByteStream;
    m_strAddress = strAddress;
    m_strPort = strPort;
  }

  /*!
   *
   */
  PPacket::PPacket (TEbyte *pBuffer, TEuint nMaxSize, TEuint nSize, string strAddress, string strPort) {
    m_pByteStream = new ByteStream (nMaxSize);
    m_pByteStream->WriteStream (pBuffer, nSize);
    m_strAddress = strAddress;
    m_strPort = strPort;
  }

  /*!
   *
   */
  PPacket::~PPacket () {
    delete m_pByteStream; 
  }

  /*!
   *
   */
  bool PPacket::HasError () {
    return m_pByteStream->HasError ();
  }

  /*!
   *
   */
  TEuint PPacket::GetSize () {
    return m_pByteStream->GetSize ();
  }

  /*!
   *
   */
  TEbyte* PPacket::GetCopyOfStream () {
    return m_pByteStream->GetCopyOfStream ();
  }

  /*!
   *
   */
  string PPacket::GetAddress () {
    return m_strAddress;
  }

  /*!
   *
   */
  string PPacket::GetPort () {
    return m_strPort;
  }

  /*!
   *
   */
  void PPacket::SetAddress (string strAddress) {
    m_strAddress = strAddress;
  }

  /*!
   *
   */
  void PPacket::SetPort (string strPort) {
    m_strPort = strPort;
  }

  /****************************************************************************************************************************
  *****************************************************************************************************************************
  *****************************************************************************************************************************
  ****************************************************************************************************************************/

  /*!
   *
   */
  Packet::Packet (IByteStream *pByteStream, string strAddress, string strPort) : 
    priv (new PPacket (pByteStream, strAddress, strPort)) {
  }

  Packet::Packet (TEbyte *pBuffer, TEuint nMaxSize, TEuint nSize, string strAddress, string strPort) :
    priv (new PPacket (pBuffer, nMaxSize, nSize, strAddress, strPort)) {
  }

  /*!
   *
   */
  Packet::~Packet () {
    delete priv;
  }

  /*!
   *
   */
  bool Packet::HasError () {
    return priv->HasError ();
  }

  /*!
   *
   */
  TEuint Packet::GetSize () {
    return priv->GetSize ();
  }

  /*!
   *
   */
  TEbyte* Packet::GetCopyOfStream () {
    return priv->GetCopyOfStream ();
  }

  /*!
   *
   */
  string Packet::GetAddress () {
    return priv->GetAddress ();
  }

  /*!
   *
   */
  string Packet::GetPort () {
    return priv->GetPort ();
  }

  /*!
   *
   */
  void Packet::SetAddress (string strAddress) {
    priv->SetAddress (strAddress);
  }

  /*!
   *
   */
  void Packet::SetPort (string strPort) {
    priv->SetPort (strPort);
  }
}