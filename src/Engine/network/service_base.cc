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

void ServiceBase::RegisterMessageCallback(const int& type,
    std::function<void(std::shared_ptr<ReceiveMessage>)> func) {
  callbacks_[type] = func;
}

void ServiceBase::ProcessMessages() {
  receive_mutex_.lock(); // LOCK

  while (receive_stream_->read_position() < receive_stream_->GetSize()) {
    try {
      uint8_t first_byte = receive_stream_->ReadByte();

      auto iter = callbacks_.find(first_byte);

      if (iter != callbacks_.end()) {
        auto message = iter->second;
        message(std::make_shared<ReceiveMessage>(receive_stream_, first_byte));
      } else {
        throw std::logic_error("Invalid message.");
      }
    } catch (const std::exception& e) {
      receive_mutex_.unlock(); // UNLOCK
      throw e;
    }
  }

  receive_mutex_.unlock(); // UNLOCK
}

} // end network namespace
} // end engine namespace