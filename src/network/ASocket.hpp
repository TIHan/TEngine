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

#ifndef __ASOCKET_HPP_
#define __ASOCKET_HPP_

#ifndef NETWORK_NO_TELIB
  #include <TELib.hpp>
#endif

#define SOCKET_MAX_IP_LEN INET6_ADDRSTRLEN
#define SOCKET_MAX_BUFFER 1400

namespace TE {
  enum SocketFamily {
    SOCKET_IPV4,
    SOCKET_IPV6
  };

  class PASocket;
  class ASocket {
  protected:
    unique_ptr<PASocket> priv;

    ASocket();

  public:
    virtual ~ASocket();

    virtual TEint Bind(const TEushort usPort);
    virtual string GetAddress();
    virtual TEboolean HasError();

    virtual tuple<shared_ptr<TEbyte>, TEint, string> Receive() = 0;
    virtual TEint Send(const shared_ptr<TEbyte> pBuffer, const TEuint nBufferSize) = 0;
  };
}

#endif /* __ASOCKET_HPP_ */
