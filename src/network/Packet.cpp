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
    unique_ptr<IByteStream> m_pByteStream;

  public:
    explicit PPacket (unique_ptr<IByteStream> pByteStream);
    ~PPacket ();

    bool HasError ();
    TEuint GetSize ();
    shared_ptr<TEbyte> GetStream ();
  };

  /*!
   *
   */
  PPacket::PPacket (unique_ptr<IByteStream> pByteStream) {
    m_pByteStream.swap (pByteStream);
  }

  /*!
   *
   */
  PPacket::~PPacket () { 
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
  shared_ptr<TEbyte> PPacket::GetStream () {
    return m_pByteStream->GetStream ();
  }

  /****************************************************************************************************************************
  *****************************************************************************************************************************
  *****************************************************************************************************************************
  ****************************************************************************************************************************/

  Packet::Packet (unique_ptr<IByteStream> pByteStream) :
    priv (new PPacket (move (pByteStream))) {
  }

  /*!
   *
   */
  Packet::~Packet () {
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
  shared_ptr<TEbyte> Packet::GetStream () {
    return priv->GetStream ();
  }
}
