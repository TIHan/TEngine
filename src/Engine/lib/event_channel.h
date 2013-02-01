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

#ifndef EVENT_CHANNEL_H_
#define EVENT_CHANNEL_H_

#include "event_processor.h"
#include "message_adapter.h"

namespace engine {
namespace lib {

class EventChannel {
public:
  explicit EventChannel(EventProcessor* processor);
  virtual ~EventChannel();

  template <typename T>
  void RegisterEvent(EventInterface<T>* event);

  template <typename T>
  void UnregisterEvent(EventInterface<T>* event);

  template <typename T>
  void PushMessage(T message);

private:
  EventProcessor* processor_;
  std::multimap<size_t, void*> events_;
};

inline EventChannel::EventChannel(EventProcessor* processor) {
  processor_ = processor;
}

inline EventChannel::~EventChannel() {
}

template <typename T>
inline void EventChannel::RegisterEvent(EventInterface<T>* event) {
  events_.emplace(typeid(T).hash_code(), event);
}

template <typename T>
inline void EventChannel::UnregisterEvent(EventInterface<T>* event) {
  events_.erase(typeid(T).hash_code(), event);
}

template <typename T>
inline void EventChannel::PushMessage(T message) {
  MessageAdapter<T> adapter(&events_, std::move(message));
  processor_->PullMessage(&adapter);
}

} // end lib namespace
} // end engine namespace

#endif /* EVENT_CHANNEL_H_ */