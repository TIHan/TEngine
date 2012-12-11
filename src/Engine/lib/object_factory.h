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

#ifndef OBJECT_FACTORY_H_
#define OBJECT_FACTORY_H_

#include "common.h"

namespace engine {
namespace lib {

class ObjectFactoryMarker {
public:
  virtual ~ObjectFactoryMarker() {}
};

template <typename T, typename Derived>
class ObjectFactoryBase {
public:
  template <typename... Args>
  std::shared_ptr<T> CreateInstanceWithArgs(Args&&... args) {
    return static_cast<Derived*>(this)->CreateInstanceWithArgs(args...);
  }
};

template <typename T>
class ObjectFactoryInterface : public ObjectFactoryMarker {
public:
  virtual ~ObjectFactoryInterface() {};

  virtual std::shared_ptr<T> CreateInstance() = 0;
};

template <typename T>
class ObjectFactory
    : public ObjectFactoryBase<T, ObjectFactory<T>>,
      public ObjectFactoryInterface<T> {
public:
  explicit ObjectFactory(std::function<std::shared_ptr<T>()> create_func);
  virtual ~ObjectFactory();

  virtual std::shared_ptr<T> CreateInstance();

  template <typename... Args>
  std::shared_ptr<T> CreateInstanceWithArgs(Args&&... args);

  bool singleton();
  void set_singleton(bool value);

private:
  std::shared_ptr<T> singleton_instance_;
  std::function<std::shared_ptr<T>()> create_func_;
  bool singleton_;
};

template <typename T>
ObjectFactory<T>::ObjectFactory(
    std::function<std::shared_ptr<T>()> create_func) {
  create_func_ = create_func;
  singleton_ = false;
}

template <typename T>
ObjectFactory<T>::~ObjectFactory() {
}

template <typename T>
std::shared_ptr<T> ObjectFactory<T>::CreateInstance() {
  if (singleton_instance_) {
    return singleton_instance_;
  } else if (singleton_) {
      singleton_instance_ = create_func_();
      return singleton_instance_;
  }
  return create_func_();
}

template <typename T>
template <typename... Args>
std::shared_ptr<T> ObjectFactory<T>::CreateInstanceWithArgs(Args&&... args) {
  if (singleton_instance_) {
    return singleton_instance_;
  } else if (singleton_) {
      singleton_instance_ = std::make_shared<T>(args...);
      return singleton_instance_;
  }
  return std::make_shared<T>(args...);
}

template <typename T>
bool ObjectFactory<T>::singleton() {
  return singleton_;
}

template <typename T>
void ObjectFactory<T>::set_singleton(bool value) {
  singleton_ = value;
}

} // end lib namespace
} // end engine namespace

#endif /* OBJECT_FACTORY_H_ */