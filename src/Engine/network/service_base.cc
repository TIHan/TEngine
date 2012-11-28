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

#include "service_base.h"
#include "udp_socket.h"

namespace engine {
namespace network {

ServiceBase::ServiceBase() : send_stream_(std::make_shared<lib::ByteStream>()),
    receive_stream_(std::make_shared<lib::ByteStream>()) {
}

ServiceBase::~ServiceBase() {
}

std::shared_ptr<SendMessage> ServiceBase::CreateMessage(const int& type) {
  return std::make_shared<SendMessage>(send_stream_, type);
}

void ServiceBase::RegisterMessageCallback(const int& type,
    std::function<void(std::shared_ptr<ReceiveMessage>)> func) {
  callbacks_[type] = func;
}

void ServiceBase::ProcessMessages() {
  receive_mutex_.lock(); // LOCK

  auto stream = receive_stream_;
  while (stream->read_position() < stream->GetSize()) {
    uint8_t first_byte = stream->Read<uint8_t>();

    auto iter = callbacks_.find(first_byte);

    if (iter != callbacks_.end()) {
      auto message = iter->second;
      try {
        message(std::make_shared<ReceiveMessage>(stream, first_byte));
      } catch (std::exception& e) {
        receive_mutex_.unlock();
        throw e;
      }
    } else {
      receive_mutex_.unlock();
      throw std::logic_error("Invalid message.");
    }
  }
  receive_stream_->Reset();

  receive_mutex_.unlock(); // UNLOCK
}

} // end network namespace
} // end engine namespace