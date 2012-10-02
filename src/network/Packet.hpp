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

#include <TELib.hpp>

namespace TE {
  class IPacket {
  public:
    virtual ~IPacket () {};

    virtual bool HasError () = 0;
    virtual TEuint GetSize () = 0;
    virtual TEbyte* GetCopyOfStream () = 0;
    virtual string GetAddress () = 0;
    virtual string GetPort () = 0;

    virtual void SetAddress (string strAddress) = 0;
    virtual void SetPort (string strPort) = 0;
  };

  class PPacket;
  class Packet : public IPacket {
   PPacket *priv;

  public:
    explicit Packet (TEbyte *pBuffer, TEuint nMaxSize, TEuint nSize, string strAddress, string strPort);
    ~Packet ();

    bool HasError ();
    TEuint GetSize ();
    TEbyte* GetCopyOfStream ();
    string GetAddress ();
    string GetPort ();

    void SetAddress (string strAddress);
    void SetPort (string strPort);
  };
}
