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

#ifndef IOC_H_
#define IOC_H_

#include "common.h"

namespace engine {
namespace lib {

template <typename T>
class IocObject {
public:
  IocObject();
  virtual ~IocObject();

  std::shared_ptr<T> GetInstance();
};

template <typename T>
IocObject<T>::IocObject() {
}

template <typename T>
IocObject<T>::~IocObject() {
}

template <typename T>
std::shared_ptr<T> IocObject<T>::GetInstance() {
  return std::make_shared<T>();
}

class Ioc {
public:
  template <typename T, typename U>
  static void Register() {
    container_[typeid(T).] = std::make_unique<IocObject<U>>();
  }

  template <typename T>
  static std::shared_ptr<T> Resolve() {
    return std::static_pointer_cast<T>(container_[typeid(T).name()]->GetInstance());
  }

private:
  static std::map<std::string, std::unique_ptr<IocObject<void>>> container_;
};

} // end engine namespace
} // end lib namespace

#endif /* IOC_H_ */