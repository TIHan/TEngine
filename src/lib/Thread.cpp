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

#include "Thread.hpp"
#include "thread/tinythread.h"

typedef struct {
  function<void()> func;
} function_t;

void ThreadFunc(void *data) {
  auto *f = (function_t*)data;
  f->func();
  delete f;
}

tthread::thread* GetThreadFromPointer(void *p) {
  return ((tthread::thread *)p);
}

namespace TE {
  class PThread {
  public:
  };

  Thread::Thread(function<void()> func) {
    auto *f = new function_t();
    f->func = func;
    m_pThread = new tthread::thread(ThreadFunc, f);
  }

  Thread::~Thread() {
    Join();
    auto thread = GetThreadFromPointer(m_pThread);
    thread->detach();
    delete thread;
  }

  void Thread::Join() {
    auto thread = GetThreadFromPointer(m_pThread);
    thread->join();
  }

  TEboolean Thread::IsJoinable() {
    auto thread = GetThreadFromPointer(m_pThread);
    return thread->joinable();
  }

  void Thread::Detatch() {
    auto thread = GetThreadFromPointer(m_pThread);
    thread->detach();
  }
}
