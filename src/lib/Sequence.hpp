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

#include "Types.hpp"
#include "Output.hpp"
#include "IQueryable.hpp"

namespace TE {
  template <class T>
  class ISequence : public IQueryable<T, ISequence<T>> {
  public:
    virtual ~ISequence() {};

    virtual void Add(T type) = 0;
  };

  template <class T>
  class Sequence : public ISequence<T> {
    unique_ptr<vector<T>> m_pVector;

  public:
    Sequence();
    virtual ~Sequence(); 

    virtual void Add(T type);

    virtual shared_ptr<ISequence<T>> Where(function<bool(T)> func);

    virtual const T* GetRawData();
  };

  template <class T>
  Sequence<T>::Sequence() {
    m_pVector = make_unique<vector<T>>();
  }

  template <class T>
  Sequence<T>::~Sequence() {
  }

  template <class T>
  void Sequence<T>::Add(T type) {
  }

  template <class T>
  shared_ptr<ISequence<T>> Sequence<T>::Where(function<bool(T)> func) {
    return nullptr; // do this for now TODO: need impl
  }

  template <class T>
  const T* Sequence<T>::GetRawData() {
    if (m_pVector->size() != 0) {
      return m_pVector->data();
    } else {
      return nullptr;
    }
  }

}

#endif /* __SEQUENCE_HPP_ */