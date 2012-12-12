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

#ifndef COMPONENT_H_
#define COMPONENT_H_

#include "common.h"

namespace engine {
namespace lib {

class ComponentInterface {
public:
  virtual ~ComponentInterface() {}

  virtual ComponentInterface* Singleton() = 0;
};

template <typename T>
class Component : public virtual ComponentInterface {
public:
  explicit Component(std::function<std::shared_ptr<T>()> func);
  virtual ~Component();

  virtual ComponentInterface* Singleton();

  std::shared_ptr<T> GetInstance();
  std::shared_ptr<T> GetInstance(
      std::function<std::shared_ptr<T>()> override_func);
  std::function<std::shared_ptr<T>()> GetCreateFunction();
  void SetCreateFunction(std::function<std::shared_ptr<T>()> func);

private:
  std::unique_ptr<ObjectFactory<T>> object_factory_;
  std::shared_ptr<T> singleton_instance_;
  bool singleton_;
};

template <typename T>
Component<T>::Component(std::function<std::shared_ptr<T>()> func)
    : object_factory_(std::make_unique<ObjectFactory<T>>(func)) {
}

template <typename T>
Component<T>::~Component() {
}

template <typename T>
ComponentInterface* Component<T>::Singleton() {
  singleton_ = true;
  return this;
}

template <typename T>
std::shared_ptr<T> Component<T>::GetInstance() {
  if (singleton_instance_) {
    return singleton_instance_;
  } else if (singleton_) {
    singleton_instance_ = object_factory_->CreateInstance();
    return singleton_instance_;
  }
  return object_factory_->CreateInstance();
}

template <typename T>
std::shared_ptr<T> Component<T>::GetInstance(
    std::function<std::shared_ptr<T>()> override_func) {
  return object_factory_->CreateInstance(override_func);
}

template <typename T>
std::function<std::shared_ptr<T>()> Component<T>::GetCreateFunction() {
  return object_factory_->create_func();
}

template <typename T>
void Component<T>::SetCreateFunction(
    std::function<std::shared_ptr<T>()> func) {
  object_factory_->set_create_func(func);
}

} // end lib namespace
} // end engine namespace

#endif /* COMPONENT_H_ */