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

#ifndef __SOCKET_HPP_
#define __SOCKET_HPP_

#ifndef NETWORK_NO_TELIB
  #include <TELib.hpp>
#endif

#define MAX_IP_LEN INET6_ADDRSTRLEN

namespace TE {
  enum SocketFamily {
    IPV4,
    IPV6
  };

  class ISocket {
  public:
    virtual ~ISocket () {};
    virtual void Close () = 0;
    virtual TEint Bind (const TEushort usPort) = 0;
    virtual shared_ptr<TEchar> GetAddressText () = 0;
    virtual TEint Receive (shared_ptr<TEbyte> pBuffer, const TEuint nBufferSize, shared_ptr<TEchar> pszNodeName) = 0;
    virtual TEint Send (const shared_ptr<TEbyte> pBuffer, const TEuint nBufferSize) = 0;
  };

  class PSocket;
  class Socket : public ISocket {
    unique_ptr<PSocket> priv;

  public:
    Socket ();
    explicit Socket (const TEbyte bFamily);
    Socket (const TEbyte bFamily, const string szNodeName, const string szServiceName);
    ~Socket ();

    void Close ();
    TEint Bind (const TEushort usPort);
    shared_ptr<TEchar> GetAddressText ();
    TEint Receive (shared_ptr<TEbyte> pBuffer, const TEuint nBufferSize, shared_ptr<TEchar> pszNodeName);
    TEint Send (const shared_ptr<TEbyte> pBuffer, const TEuint nBufferSize);
  };
}

#endif /* __SOCKET_HPP_ */
