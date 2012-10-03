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

  public:
    explicit PPacket (TEuint nMaxSize);
    ~PPacket ();

    bool HasError ();
    TEuint GetSize ();
    TEbyte* GetCopyOfStream ();

    IByteStream* GetStream ();
  };

  /*!
   *
   */
  IByteStream* PPacket::GetStream () {
    return m_pByteStream;
  }

  /*!
   *
   */
  PPacket::PPacket (TEuint nMaxSize) {
    m_pByteStream = new ByteStream (nMaxSize);
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

  /****************************************************************************************************************************
  *****************************************************************************************************************************
  *****************************************************************************************************************************
  ****************************************************************************************************************************/

  /*!
   *
   */
  IByteStream* Packet::GetStream () {
    return priv->GetStream ();
  }

  Packet::Packet (TEuint nMaxSize) :
    priv (new PPacket (nMaxSize)) {
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
}
