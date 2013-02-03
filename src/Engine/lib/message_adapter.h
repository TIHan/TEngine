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

  virtual void ExecuteEvent(void* event) = 0;
  virtual size_t GetTypeHashCode() = 0;
};

template <typename T, typename... Args>
class MessageAdapter : public virtual MessageAdapterInterface {
public:
  explicit MessageAdapter(Args&&... args) : message_(args...) {};

  virtual void ExecuteEvent(void* event);
  virtual size_t GetTypeHashCode();

private:
  T message_;
};

template <typename T, typename... Args>
inline void MessageAdapter<T, Args...>::ExecuteEvent(void* event) {
  reinterpret_cast<EventInterface<T>*>(event)->Handle(message_);
}

template <typename T, typename... Args>
inline size_t MessageAdapter<T, Args...>::GetTypeHashCode() {
  return message_.type_hash_code;
}

} // end lib namespace
} // end engine namespace

#endif /* MESSAGE_ADAPTER_H_ */