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

#include "event.h"

namespace engine {
namespace lib {

class EventAdapterInterface {
public:
  virtual ~EventAdapterInterface() {}

  virtual void Execute(void* message) = 0;
};

template <typename T>
class EventAdapter : public virtual EventAdapterInterface {
public:
  explicit EventAdapter(EventInterface<T>* event) {
    event_ = event;
  }
  
  virtual void Execute(void* message) {
    event_->Handle(*reinterpret_cast<T*>(message));
  }

private:
  EventInterface<T>* event_;
};

class EventChannel {
public:
  EventChannel();
  virtual ~EventChannel();

  template <typename T>
  void RegisterEvent(EventInterface<T>* event);

  template <typename T>
  void UnregisterEvent(EventInterface<T>* event);

  template <typename T, typename... Args>
  void PushMessage(Args&&... args);

  void Flush();

private:
  std::unordered_multimap<size_t, void*> events_;
  std::vector<MessageType*> message_buffer_;
};

inline EventChannel::EventChannel() {
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

template <typename T, typename... Args>
inline void EventChannel::PushMessage(Args&&... args) {
  message_buffer_.emplace_back(new T(args...));
}

inline void EventChannel::Flush() {
  for (auto citer = message_buffer_.crbegin();
            citer != message_buffer_.crend(); ++citer) {
    auto events = events_.equal_range((*citer)->type_hash_code);
    for (auto iter = events.first; iter != events.second; ++iter) {
    }
  }
  message_buffer_.clear();
}

} // end lib namespace
} // end engine namespace

#endif /* EVENT_CHANNEL_H_ */