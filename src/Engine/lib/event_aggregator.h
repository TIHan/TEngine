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

#ifndef EVENT_AGGREGATOR_H_
#define EVENT_AGGREGATOR_H_

#include "event_processor.h"
#include "event.h"

namespace engine {
namespace lib {

class EventAggregator {
public:
  EventAggregator(std::shared_ptr<EventProcessor> event_processor);
  virtual ~EventAggregator();

  template <typename T>
  void Subscribe(EventInterface<T>* event);

  template <typename T, typename... Args>
  void Publish(Args&&... args);

private:
  std::unordered_multimap<size_t, void*> events_;
  std::shared_ptr<EventProcessor> event_processor_;
};

inline EventAggregator::EventAggregator(
    std::shared_ptr<EventProcessor> event_processor)
    : event_processor_(event_processor) {
}

inline EventAggregator::~EventAggregator() {
}

template <typename T>
inline void EventAggregator::Subscribe(EventInterface<T>* event) {
  events_.emplace(typeid(T).hash_code(), event);
}

template <typename T, typename... Args>
inline void EventAggregator::Publish(Args&&... args) {
  auto events = events_.equal_range(typeid(T).hash_code());
  for (auto iter = events.first; iter != events.second; ++iter) {
    if (auto void_event = iter->second) {
      T message(std::forward<Args>(args)...);
      event_processor_->Push([=] {
        auto event = reinterpret_cast<EventInterface<T>*>(void_event);
        if (event) event->Handle(message);
      });
    } else {
      events_.erase(iter);
    }
  }
}

} // end lib namespace
} // end engine namespace

#endif /* EVENT_AGGREGATOR_H_ */