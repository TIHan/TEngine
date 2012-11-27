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

#include "client.h"
#include "udp_socket.h"

namespace engine {
namespace network {

class ClientImpl {
public:
  UdpSocket socket_;
  std::string server_address_;
  std::string server_port_;
};

Client::Client() : impl_(std::make_unique<ClientImpl>()),
      send_stream_(std::make_shared<lib::ByteStream>()),
      receive_stream_(std::make_shared<lib::ByteStream>()) {
  impl_->socket_.set_blocking(false);
}

Client::~Client() {
  Disconnect();
}

void Client::Connect(const std::string& address, const std::string& port) {
  impl_->socket_.Open(address, port);

  close_receive_thread_ = false;
  receive_thread_ = std::make_unique<std::thread>([&] () {
    while (!close_receive_thread_) {
      auto receive = impl_->socket_.ReceiveFrom();
      receive_stream_->WriteBuffer(*std::get<0>(receive));
      // Sleep for 1 microsecond.
      std::chrono::microseconds sleep(1);
      std::this_thread::sleep_for(sleep);
    }
  });
}

void Client::Disconnect() {
  if (!close_receive_thread_) {
    close_receive_thread_ = true;
    receive_thread_->join();
  }
  impl_->socket_.Close();
}

std::shared_ptr<SendMessage> Client::CreateMessage(const int& type) {
  return std::make_shared<SendMessage>(send_stream_, type);
}

void Client::RegisterMessageCallback(const int& type,
    std::function<void(std::unique_ptr<ReceiveMessage>)>& func) {
  callbacks_[type] = func;
}

void Client::ProcessMessages() {
  auto stream = receive_stream_;
  while (stream->read_position() < stream->GetSize()) {
    uint8_t first_byte = stream->Read<uint8_t>();

    std::map<int,
        std::function<void(std::unique_ptr<ReceiveMessage>)>>
        ::iterator iter = callbacks_.find(first_byte);

    if (iter != callbacks_.end()) {
      auto message = iter->second;
      message(std::make_unique<ReceiveMessage>(stream, first_byte));
    } else {
      throw std::logic_error("Invalid message.");
    }
  }
}

void Client::SendMessages() {
  impl_->socket_.Send(*send_stream_);
}

} // end network namespace
} // end engine namespace