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

#include "event_channel.h"

namespace engine {
namespace lib {

class EventAggregator {
public:
  explicit EventAggregator(EventChannel* channel);

  template <typename T>
  void Subscribe(EventInterface<T>* event);

  template <typename T>
  void Unsubscribe(EventInterface<T>* event);

  template <typename T, typename... Args>
  void Publish(Args&&... args);

private:
  EventChannel* channel_;
};

inline EventAggregator::EventAggregator(EventChannel* channel)
    : channel_(channel) {
}

template <typename T>
inline void EventAggregator::Subscribe(EventInterface<T>* event) {
  channel_->RegisterEvent(event);
}

template <typename T>
inline void EventAggregator::Unsubscribe(EventInterface<T>* event) {
  channel_->UnregisterEvent(event);
}

template <typename T, typename... Args>
inline void EventAggregator::Publish(Args&&... args) {
  channel_->PushMessage<T>(args...);
}

} // end lib namespace
} // end engine namespace

#endif /* EVENT_AGGREGATOR_H_ */
