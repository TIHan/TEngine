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

#define SOCKET_MAX_BUFFER 1400

namespace TE {
  enum SocketFamily {
    SOCKET_UNSPECIFIED,
    SOCKET_IPV4,
    SOCKET_IPV6
  };

  typedef struct address_s address_t;

  namespace Socket {
    string GetAddress(const shared_ptr<const address_t>& address);
  }

  class ISocket {
  public:
    virtual ~ISocket() {};

    virtual TEint Bind(const TEushort& usPort) = 0;
    virtual string GetAddress() = 0;
    virtual SocketFamily GetFamily() = 0;
    virtual void Close() = 0;
  };

  class PASocket;
  class ASocket : public virtual ISocket {
  protected:
    unique_ptr<PASocket> priv;

    ASocket(); // Abstract class

  public:
    virtual ~ASocket();

    virtual TEint Bind(const TEushort& usPort);
    virtual string GetAddress();
    virtual SocketFamily GetFamily();
    virtual void Close();
  };
}

#endif /* __ASOCKET_HPP_ */
