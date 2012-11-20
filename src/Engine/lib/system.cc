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

#include "system.h"
#ifdef __GNUC__
# include <sys/time.h>
#elif _MSC_VER
# include <Windows.h>
# include <sys/timeb.h>
#endif

namespace engine {
namespace lib {

unsigned long long GetTicks() {
#ifdef _MSC_VER
  __timeb64 tb;
  _ftime64_s(&tb);
  return ((unsigned long long)tb.time * 1000) + tb.millitm;
#elif __GNUC__
  struct timeval tv;
  gettimeofday(&tv, nullptr);
  return ((unsigned long long)tv.tv_sec * 1000) + (tv.tv_usec / 1000);
#endif
}

void Delay(unsigned int ms) {
#ifdef __GNUC__
  struct timespec ts;
  ts.tv_sec = ms / 1000;
  ts.tv_nsec = (ms % 1000) * (1000 * 1000);
  nanosleep(&ts, nullptr);
#elif _MSC_VER
  Sleep(ms);
#endif
}

} // end lib namespace
} // end engine namespace