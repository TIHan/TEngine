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

#include "Server.hpp"
#include "Socket.hpp"

namespace TE {
  class PServer : public Network, IServer {
  public:
    explicit PServer (TEuint nMaxTransUnit, TEushort usPort);
    PServer (TEuint nMaxTransUnit, string szIp, string port);
    ~PServer ();

    bool ServerSend (shared_ptr<IPacket> pPacket);
    shared_ptr<Packet> ServerReceive ();
  };

  PServer::PServer (TEuint nMaxTransUnit, TEushort usPort) : Network () {
    Setup (nMaxTransUnit, IPV4, "", "");
    Host (usPort);
    PrintAddresses ();
  }

  /*!
   *
   */
  PServer::PServer (TEuint nMaxTransUnit, string szIp, string szPort) : Network () {
    Setup (nMaxTransUnit, IPV4, szIp, szPort);
    PrintAddresses ();
  }

  /*!
   *
   */
  PServer::~PServer () {
  }

  /*!
   *
   */
  bool PServer::ServerSend (shared_ptr<IPacket> pPacket) {
    return Send (pPacket);
  }

  /*!
   *
   */
  shared_ptr<Packet> PServer::ServerReceive () {
    string ip;
    return Receive (ip);
  }

  /****************************************************************************************************************************
  *****************************************************************************************************************************
  *****************************************************************************************************************************
  ****************************************************************************************************************************/

  /*!
   *
   */
  Server::Server (TEuint nMaxTransUnit, TEushort usPort) :
    priv (new PServer (nMaxTransUnit, usPort)) {
  }

  /*!
   *
   */
  Server::Server (TEuint nMaxTransUnit, string szIp, string szPort) :
    priv (new PServer (nMaxTransUnit, szIp, szPort)) {
  }

  /*!
   *
   */
  Server::~Server () {
    priv->~PServer ();
  }

  /*!
   *
   */
  bool Server::Send (shared_ptr<IPacket> pPacket) {
    return priv->ServerSend (pPacket);
  }

  /*!
   *
   */
  shared_ptr<Packet> Server::Receive () {
    return priv->ServerReceive ();
  }
}