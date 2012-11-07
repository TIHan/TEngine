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

namespace TE {
  template <typename T>
  class ISequence : public ICollection<T, ISequence<T>> {
  public:
    virtual ~ISequence() {};
  };

  template <typename T>
  class Sequence : public ISequence<T> {
  protected:
    unique_ptr<vector<T>> m_pVector;

  public:
    Sequence();
    explicit Sequence(const TEint nAllocateSize);
    virtual ~Sequence(); 

    virtual void Add(const T& item);
    virtual void Remove(const T& item);
    virtual TEint GetSize() const;
    virtual void Clear();

    virtual shared_ptr<ISequence<T>> Where(const function<bool(T)> func);

    virtual void SetCapacity(const TEint nAmount);
    virtual TEint GetCapacity();
    virtual void Resize(const TEint nSize);
    virtual void ShrinkToSize(const TEint nSize);
    virtual const T* GetRawData() const;
  };

  /*!
   *
   */
  template <typename T>
  Sequence<T>::Sequence() :
      m_pVector(make_unique<vector<T>>()) {
  }

  /*!
   *
   */
  template <typename T>
  Sequence<T>::Sequence(const TEint nAllocateSize) :
      m_pVector(make_unique<vector<T>>(nAllocateSize)) {
  }

  /*!
   *
   */
  template <typename T>
  Sequence<T>::~Sequence() {
  }

  /*!
   *
   */
  template <typename T>
  void Sequence<T>::Add(const T& item) {
    m_pVector->push_back(item);
  }

  /*!
   *
   */
  template <typename T>
  void Sequence<T>::Remove(const T& item) {
    for (auto i = m_pVector->begin(); i != m_pVector->end(); i++) {
      if (*i == item) {
        i = m_pVector->erase(i);
        return;
      }
    }
  }

  /*!
   *
   */
  template <typename T>
  TEint Sequence<T>::GetSize() const {
    return m_pVector->size();
  }

  /*!
   *
   */
  template <typename T>
  void Sequence<T>::Clear() {
    return m_pVector->clear();
  }

  /*!
   *
   */
  template <typename T>
  shared_ptr<ISequence<T>> Sequence<T>::Where(const function<bool(T)> func) {
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
  template <typename T>
  void Sequence<T>::SetCapacity(const TEint nAmount) {
    m_pVector->reserve(nAmount);
  }

  /*!
   *
   */
  template <typename T>
  TEint Sequence<T>::GetCapacity() {
    return m_pVector->capacity();
  }

  /*!
   *
   */
  template <typename T>
  void Sequence<T>::Resize(const TEint nSize) {
    m_pVector->resize(nSize);
  }

  /*!
   *
   */
  template <typename T>
  void Sequence<T>::ShrinkToSize(const TEint nSize) {
    m_pVector->resize(nSize);
    m_pVector->shrink_to_fit();
  }

  /*!
   *
   */
  template <typename T>
  const T* Sequence<T>::GetRawData() const {
    return m_pVector->data();
  }
}

#endif /* __SEQUENCE_HPP_ */