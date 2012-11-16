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

#ifndef __PRECOMPILED_HPP_
#define __PRECOMPILED_HPP_

#include <cstdint>
#include <string>
#include <memory>
#include <functional>
#include <tuple>
#include <algorithm>
#include <stdexcept>

// http://en.cppreference.com/w/cpp/utility/forward
// Note: This should be in the C++11 standard eventually.
template <typename T>
std::unique_ptr<T> make_unique()
{
    return std::unique_ptr<T>(new T());
}

template <typename T, typename U>
std::unique_ptr<T> make_unique(U&& u)
{
    return std::unique_ptr<T>(new T(std::forward<U>(u)));
}

namespace TE {
  using namespace std;

  // [WS] TODO: Need to put this somewhere else.
  namespace String {
    inline string Empty() { return ""; }
  }
}

#endif /* __PRECOMPILED_HPP_ */