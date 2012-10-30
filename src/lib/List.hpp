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

#ifndef __LIST_HPP_
#define __LIST_HPP_

#include "Types.hpp"
#include "Output.hpp"

namespace TE {
  template<class T>
  class IList {
  public:
    virtual ~IList() {};

    virtual void Add(T type) = 0;
    virtual T Find(T type) = 0;
  };

  template<class T>
  class List : IList<T> {
    shared_ptr<list<T>> m_list;

  public:
    List();
    virtual ~List();

    virtual void Add(T type);
    virtual T Find(T type);
    shared_ptr<list<T>> Get();
  };

  template <class T>
  List<T>::List() {
    m_list = make_shared<list<T>>();
  }

  template <class T>
  List<T>::~List() {
  }

  template <class T>
  void List<T>::Add(T type) {
    m_list->push_back(type);
  }

  template <class T>
  T List<T>::Find(T type) {
    T found = (T)nullptr;
    for_each(m_list->begin(), m_list->end(), [&type, &found] (T t) {
      if (type == t) {
        found = t;
      }
    });
    return found;
  }

  template <class T>
  shared_ptr<list<T>> List<T>::Get() {
    return m_list;
  }
}

#endif /* __LIST_HPP_ */