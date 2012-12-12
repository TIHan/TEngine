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

#include "object_factory.h"
#include "component.h"

namespace engine {
namespace lib {

class Ioc {
public:
  template <typename T>
  static std::shared_ptr<ComponentInterface> Register(
      std::function<std::shared_ptr<T>()> func) {
    auto component = std::make_shared<Component<T>>(func);
#ifdef _MSC_VER
    container_.emplace(typeid(T).hash_code(), component);
#else
    container_[typeid(T).hash_code()] = component;
#endif
    return component;
  }

  template <typename T>
  static std::shared_ptr<T> Resolve() {
    auto iter = container_.find(typeid(T).hash_code());
    auto component = std::dynamic_pointer_cast<Component<T>>(iter->second);
    if (!component)
      throw std::runtime_error("Type not registered.");
    return component->GetInstance();
  }

  template <typename T>
  static std::shared_ptr<T> Resolve(
      std::function<std::shared_ptr<T>()> func) {
    auto iter = container_.find(typeid(T).hash_code());
    auto component = std::dynamic_pointer_cast<Component<T>>(iter->second);
    if (!component)
      throw std::runtime_error("Type not registered.");
    return component->GetInstance(func);
  }

private:
  static std::map<size_t, std::shared_ptr<ComponentInterface>> container_;
};

} // end lib namespace
} // end engine namespace

#endif /* IOC_H_ */
