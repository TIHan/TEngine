/*
  Copyright (c) 2012, William F. Smith
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

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

#ifndef COMMON_H_
#define COMMON_H_

#include <cstdint>
#include <string>
#include <memory>
#include <functional>
#include <tuple>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <list>
#include <thread>
#include <queue>
#include <iostream>
#include <map>

// http://en.cppreference.com/w/cpp/utility/forward
// Note: This should be in the C++11 standard eventually.
namespace std {

template <typename T>
unique_ptr<T> make_unique() {
    return unique_ptr<T>(new T());
}

template <typename T, typename V1>
unique_ptr<T> make_unique(V1&& v1) {
    return unique_ptr<T>(new T(forward<V1>(v1)));
}

template <typename T, typename V1, typename V2>
unique_ptr<T> make_unique(V1&& v1, V2&& v2) {
    return unique_ptr<T>(new T(forward<V1>(v1), forward<V2>(v2)));
}

} // end std namespace

namespace engine {
namespace lib {

inline std::string EmptyString() { return ""; }
inline void Print(std::string string) { std::cout << string << "\n"; }

} // end lib namespace
} // end engine namespace

#endif // COMMON_H_