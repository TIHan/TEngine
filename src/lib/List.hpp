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

#include "ICollection.hpp"

namespace TE {
  template <typename T>
  class IList : public ICollection<T, IList<T>> {
  public:
    virtual ~IList() {};
  };

  template <typename T>
  class List : public IList<T> {
  protected:
    unique_ptr<list<T>> m_pList;

  public:
    List();
    virtual ~List();

    virtual void Add(const T& item);
    virtual void Remove(const T& item);

    virtual shared_ptr<IList<T>> Where(function<bool(T)> func);
  };

  /*!
   *
   */
  template <typename T>
  List<T>::List() :
      m_pList(make_unique<list<T>>()) {
  }

  /*!
   *
   */
  template <typename T>
  List<T>::~List() {
  }

  /*!
   *
   */
  template <typename T>
  void List<T>::Add(const T& item) {
    m_pList->push_back(item);
  }

  /*!
   *
   */
  template <typename T>
  void List<T>::Remove(const T& item) {
    for (auto i = m_pList->begin(); i != m_pList->end(); i++) {
      if (*i == item) {
        i = m_pList->erase(i);
        return;
      }
    }
  }

  /*!
   *
   */
  template <typename T>
  shared_ptr<IList<T>> List<T>::Where(function<bool(T)> func) {
    auto l = make_shared<List<T>>();
    for_each(m_pList->cbegin(), m_pList->cend(), [&func, &l] (T t) {
      if (func(t)) {
        l->Add(t);
      }
    });
    return l;
  }
}

#endif /* __LIST_HPP_ */