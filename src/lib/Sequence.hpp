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

#ifndef __SEQUENCE_HPP_
#define __SEQUENCE_HPP_

#include "ICollection.hpp"
#include "IRawData.hpp"

namespace TE {
  template <class T>
  class ISequence : public ICollection<T, ISequence<T>>, public IRawData<T> {
  public:
    virtual ~ISequence() {};
  };

  template <class T>
  class Sequence : public ISequence<T> {
  protected:
    unique_ptr<vector<T>> m_pVector;

  public:
    Sequence();
    virtual ~Sequence(); 

    virtual void Add(const T& item);
    virtual void Remove(const T& item);

    virtual shared_ptr<ISequence<T>> Where(function<bool(T)> func);

    virtual T* GetRawData();
  };

  /*!
   *
   */
  template <class T>
  Sequence<T>::Sequence() :
      m_pVector(make_unique<vector<T>>()) {
  }

  /*!
   *
   */
  template <class T>
  Sequence<T>::~Sequence() {
  }

  /*!
   *
   */
  template <class T>
  void Sequence<T>::Add(const T& item) {
    m_pVector->push_back(item);
  }

  /*!
   *
   */
  template <class T>
  void Sequence<T>::Remove(const T& item) {
    for (auto i = m_pVector->cbegin(); i != m_pVector->cend(); i++) {
      if (*i == item) {
        i = m_pVector->erase(i);
        return;
      }
    }
  }

  /*!
   *
   */
  template <class T>
  shared_ptr<ISequence<T>> Sequence<T>::Where(function<bool(T)> func) {
    auto seq = make_shared<Sequence<T>>();
    for_each(m_pVector->begin(), m_pVector->end(), [&seq, &func] (T item) {
      if (func(item)) {
        seq->Add(item);
      }
    });
    return seq;
  }

  /*!
   *
   */
  template <class T>
  T* Sequence<T>::GetRawData() {
    if (m_pVector->size() != 0) {
      return m_pVector->data();
    } else {
      return nullptr;
    }
  }
}

#endif /* __SEQUENCE_HPP_ */