/*
  Copyright (c) 2012-2013, William F. Smith
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

#ifndef EVENT_PROCESSOR_H_
#define EVENT_PROCESSOR_H_

#include "message_adapter.h"

namespace engine {
namespace lib {

class EventProcessor {
public:
  EventProcessor();
  virtual ~EventProcessor();

  void PullMessage(MessageAdapterInterface* adapter);
  void Process();

private:
  std::vector<MessageAdapterInterface*> vector_;
  std::array<MessageAdapterInterface*, 1000000> asdf_;
};

inline EventProcessor::EventProcessor() {

}

inline EventProcessor::~EventProcessor() {
}

inline void EventProcessor::PullMessage(MessageAdapterInterface* adapter) {
  //adapter_queue_.push(adapter);
  //vector_.push_back(adapter);
  asdf_[asdf_.size() - 1] = adapter;
}

inline void EventProcessor::Process() {
  for (auto v : vector_) {
    v->FlushMessage();
  }
  vector_.clear();
}

} // end lib namespace
} // end engine namespace

#endif /* EVENT_PROCESSOR_H_ */