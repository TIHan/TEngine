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

#ifndef EVENT_H_
#define EVENT_H_

#include "common.h"

namespace engine {
namespace lib {

enum class MessageType : uint8_t {
  kUnknown,
  kCommand,
  kEvent
};

struct Message {
  size_t hash_code;
  MessageType type;

protected:
  Message() {
    type = MessageType::kUnknown;
  }
};

template <typename T>
struct MessageBase : public Message {
protected:
  MessageBase() {
    static auto hash = typeid(T).hash_code();
    hash_code = hash;
  }
};

template <typename T>
struct EventMessage : public MessageBase<T> {
protected:
  EventMessage() {
    MessageType::kEvent;
  }
};

template <typename T>
struct CommandMessage : public MessageBase<T> {
protected:
  CommandMessage() {
    MessageType::kCommand;
  }
};

template <typename T>
class EventInterface {
public:
  virtual ~EventInterface() {}

  virtual void Handle(T message) = 0;
};

} // end lib namespace
} // end engine namespace

#endif /* EVENT_H_ */