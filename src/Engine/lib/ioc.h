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

class ObjectFactoryMarker {
public:
  virtual ~ObjectFactoryMarker() {}
};

template <typename T>
class ObjectFactoryInterface : public ObjectFactoryMarker {
public:
  virtual ~ObjectFactoryInterface() {};

  virtual std::shared_ptr<T> CreateInstance() = 0;
};

// Object Factory
template <typename T, typename U>
class ObjectFactory : public virtual ObjectFactoryInterface<T> {
public:
  explicit ObjectFactory(std::function<std::shared_ptr<U>()> create_func);
  virtual ~ObjectFactory();

  virtual std::shared_ptr<T> CreateInstance();

  bool singleton();
  void set_singleton(bool value);

private:
  std::shared_ptr<U> singleton_instance_;
  std::function<std::shared_ptr<U>()> create_func_;
  bool singleton_;
};

template <typename T, typename U>
ObjectFactory<T, U>::ObjectFactory(
    std::function<std::shared_ptr<U>()> create_func) {
  create_func_ = create_func;
  singleton_ = false;
}

template <typename T, typename U>
ObjectFactory<T, U>::~ObjectFactory() {
}

template <typename T, typename U>
std::shared_ptr<T> ObjectFactory<T, U>::CreateInstance() {
  if (singleton_) { 
    if (!singleton_instance_) {
      singleton_instance_ = create_func_();
      return singleton_instance_;
    } else {
      return singleton_instance_;
    }
  }
  return create_func_();
}

template <typename T, typename U>
bool ObjectFactory<T, U>::singleton() {
  return singleton_;
}

template <typename T, typename U>
void ObjectFactory<T, U>::set_singleton(bool value) {
  singleton_ = value;
}

// Component
template <typename T, typename U>
class Component {
public:
  Component(std::shared_ptr<ObjectFactory<T, U>> object_factory) {
    object_factory_ = object_factory;
  }

  Component<T, U>* Singleton() {
    object_factory_->set_singleton(true);
    return this;
  }

private:
  std::shared_ptr<ObjectFactory<T, U>> object_factory_;
};

static std::map<size_t, std::shared_ptr<ObjectFactoryMarker>> g_container;

class Ioc {
public:
  template <typename T, typename U>
  static std::unique_ptr<Component<T, U>> Register() {
    auto factory = std::make_shared<ObjectFactory<T, U>>([] {
      return std::make_shared<U>();
    });
    g_container[typeid(T).hash_code()] = factory;
    return std::make_unique<Component<T, U>>(factory);
  }

  template <typename T, typename U, typename... Args>
  static std::unique_ptr<Component<T, U>> RegisterSpecial(Args&&... args) {
    // Make a tuple to include in our arguments, then unpack them in the
    // function.
    auto tuple_args = std::make_tuple(args...);
    auto factory = std::make_shared<ObjectFactory<T, U>>([=] {
      return std::make_shared<U>(std::get<(sizeof... (Args)) - 1>(tuple_args));
    });
#if 0
    // This is perfectly acceptable code according to the standard, but not
    // implemented in all compilers.
    auto factory = std::make_shared<ObjectFactory<T, U>>([&] {
      return std::make_shared<U>(args...);
    });
#endif
    g_container[typeid(T).hash_code()] = factory;
    return std::make_unique<Component<T, U>>(factory);
  }

  template <typename T>
  static std::shared_ptr<T> Resolve() {
    auto factory = std::static_pointer_cast<ObjectFactoryInterface<T>>(
      g_container[typeid(T).hash_code()]);
    return factory->CreateInstance();
  }
};

} // end lib namespace
} // end engine namespace

#endif /* IOC_H_ */