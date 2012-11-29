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

#ifndef PROCESS_H_
#define PROCESS_H_

#include "common.h"

namespace engine {
namespace lib {

// TODO: Need to handle exceptions.
class Process {
public:
  Process();
  virtual ~Process();

  virtual void Run(const std::function<void()>& func);
  virtual void Stop();

  /* Accessors / Mutators */
  virtual bool stopped();

private:
  std::unique_ptr<std::thread> thread_;
  std::atomic_bool stopped_;

  /* Exceptions */
  std::function<void(std::exception& e)> exception_func_;
  std::exception exception_;
  bool has_exception_;
};

inline Process::Process()
    : exception_func_([] (const std::exception& e) { throw e; }) {
  stopped_ = true;
}

inline Process::~Process() {
  Stop();
}

inline void Process::Stop() {
  if (!stopped_) {
    stopped_ = true;
    thread_->join();
  }
}

inline bool Process::stopped() {
  return stopped_;
}

} // end engine namespace
} // end lib namespace

#endif /* PROCESS_H_ */