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

#ifndef MESSAGE_ADAPTER_H_
#define MESSAGE_ADAPTER_H_

#include "event.h"

namespace engine {
namespace lib {

class MessageAdapterInterface {
public:
  virtual ~MessageAdapterInterface() {}

  virtual void FlushMessage() = 0;
};

template <typename T>
class MessageAdapter : public virtual MessageAdapterInterface {
public:
  MessageAdapter(std::multimap<size_t, void*>* events, T&& message);
  virtual ~MessageAdapter();

  virtual void FlushMessage();

private:
  std::multimap<size_t, void*>* events_;
  T message_;
};

template <typename T>
inline MessageAdapter<T>::MessageAdapter(std::multimap<size_t, void*>* events,
                                         T&& message)
    : events_(events), message_(std::move(message)) {
}

template <typename T>
inline MessageAdapter<T>::~MessageAdapter() {
}

template <typename T>
inline void MessageAdapter<T>::FlushMessage() {
  //auto events = events_->equal_range(typeid(T).hash_code());
  //for (auto iter = events.first; iter != events.second; ++iter) {
  //  if (auto void_event = iter->second) {
  //    reinterpret_cast<EventInterface<T>*>(void_event)->Handle(message_);
  //  } else {
  //    events_->erase(iter);
  //  }
  //}
}

} // end lib namespace
} // end engine namespace

#endif /* MESSAGE_ADAPTER_H_ */