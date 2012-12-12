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

template <typename T>
class ObjectFactory {
public:
  ObjectFactory(std::function<std::shared_ptr<T>()> func);
  virtual ~ObjectFactory();

  std::shared_ptr<T> CreateInstance();
  std::shared_ptr<T> CreateInstance(
      std::function<std::shared_ptr<T>()> func);

  std::function<std::shared_ptr<T>()> create_func();
  void set_create_func(std::function<std::shared_ptr<T>()> func);

private:
  std::function<std::shared_ptr<T>()> create_func_;
};

template <typename T>
ObjectFactory<T>::ObjectFactory(std::function<std::shared_ptr<T>()> func) {
  create_func_ = func;
}

template <typename T>
ObjectFactory<T>::~ObjectFactory() {
}

template <typename T>
std::shared_ptr<T> ObjectFactory<T>::CreateInstance() {
  return create_func_();
}

template <typename T>
std::shared_ptr<T> ObjectFactory<T>::CreateInstance(
    std::function<std::shared_ptr<T>()> func) {
  return func();
}

template <typename T>
std::function<std::shared_ptr<T>()> ObjectFactory<T>::create_func() {
  return create_func_;
}

template <typename T>
void ObjectFactory<T>::set_create_func(
    std::function<std::shared_ptr<T>()> func) {
  create_func_ = func;
}

} // end lib namespace
} // end engine namespace

#endif /* OBJECT_FACTORY_H_ */