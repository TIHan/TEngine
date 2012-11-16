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

#ifndef __UDPSOCKET_HPP_
#define __UDPSOCKET_HPP_

#include "ASocket.hpp"

#ifndef NETWORK_NO_TELIB
  #include <TELib.hpp>
#endif

// Microsoft gives a warning about virtual inheritance. Turn it off.
#ifdef _MSC_VER
  #pragma warning( disable : 4250 ) // C4250 - 'class1' : inherits 'class2::member' via dominance
#endif

namespace TE {
  class IUdpSocket : public virtual ISocket {
  public:
    virtual ~IUdpSocket() {};

    virtual void Open() = 0;
    virtual void Open(const string& szAddress, const string& szPort) = 0;
    virtual tuple<shared_ptr<ByteSequence>, shared_ptr<address_t>> ReceiveFrom() = 0;
    virtual int Send(const shared_ptr<const IByteData>& pByteData) = 0;
    virtual int SendTo(const shared_ptr<const IByteData>& pByteData, const shared_ptr<const address_t>& address) = 0;
  };

  class UdpSocket : public ASocket, public IUdpSocket {
  public:
    explicit UdpSocket();
    explicit UdpSocket(const SocketFamily& family);
    virtual ~UdpSocket();

    virtual void Open();
    virtual void Open(const string& szAddress, const string& szPort);
    virtual tuple<shared_ptr<ByteSequence>, shared_ptr<address_t>> ReceiveFrom();
    virtual int Send(const shared_ptr<const IByteData>& pByteData);
    virtual int SendTo(const shared_ptr<const IByteData>& pByteData, const shared_ptr<const address_t>& address);
  };
}

#endif /* __UDPSOCKET_HPP_ */