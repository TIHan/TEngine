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

#include <string.h>
#include "Types.hpp"
#include "Messages.hpp"
#include "ByteStream.hpp"
#include "ByteIO.hpp"

namespace TE {
  namespace ByteIO {
    /*!
     *
     */
    TEchar* ReadString (IByteStream *pByteStream) {
      TEuint size = pByteStream->GetSize ();
      TEchar *val = new TEchar[size];

      for (TEuint i = 0; i < size; i++) {
        val[i] = pByteStream->ReadByte ();
        
        if (val[i] == '\0') {
          break;
        }
      }
      return val;
    }

    /*!
     *
     */
    void WriteString (IByteStream *pByteStream, const TEchar *sz) {
      TEuint size = (TEint)strlen (sz);
      TEuint stream = pByteStream->GetMaxSize () - pByteStream->GetSize ();

      stream = stream != 0 ? stream - 1 : 0; 
      size = stream < size ? stream : size;
      for (TEuint i = 0; i < size; i++) {
        pByteStream->WriteByte (sz[i]);
      }
      pByteStream->WriteByte ('\0');
    }

    /*!
     *
     */
    void WriteStream (IByteStream *pByteStream, const TEbyte *pbStream, const TEuint nSize) {
      TEuint stream = pByteStream->GetMaxSize () - pByteStream->GetSize ();
      TEuint size = stream < nSize ? stream : nSize;

      for (TEuint i = 0; i < size; i++) {
        pByteStream->WriteByte (pbStream[i]);
      }
    }
  }
}