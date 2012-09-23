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

#include "Types.hpp"
#include "Messages.hpp"
#include "ByteStream.hpp"
#include "ByteIO.hpp"

namespace TE {
  namespace ByteIO {
    /*
     *
     */
    template <class T>
    T Read (IByteStream *pByteStream) {
      TEuint size = sizeof (T);

      union unpack_t {
        TEbyte byte[sizeof (T)];
        T val;
      } unpack;

      for (TEuint i = 0; i < size; i++) {
        unpack.byte[i] = pByteStream->ReadByte ();
      }

      T value = unpack.val;
      return value;
    }

    /*
     *
     */
    template <class T>
    void Write (IByteStream *pByteStream, T val) {
      TEuint size = sizeof (T);

      union pack_t {
        TEbyte byte[sizeof (T)];
        T val;
      } pack;

      pack.val = val;
      for (TEuint i = 0; i < size; i++) {
        pByteStream->WriteByte (pack.byte[i]);
      }
    }
  }
}