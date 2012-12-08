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
  ClientImpl();

  std::unique_ptr<UdpSocket> socket_;
};

ClientImpl::ClientImpl() {
  UdpSocketOptions options;
  options.max_receive_buffer = 1024;
  options.blocking = false;
  socket_ = std::make_unique<UdpSocket>(options);
}

Client::Client()
    : impl_(std::make_unique<ClientImpl>()),
      send_buffer_(
        std::make_shared<std::queue<std::shared_ptr<lib::ByteStream>>>()) {
}

Client::~Client() {
  Disconnect();
}

void Client::Connect(const std::string& address, const std::string& port) {
  impl_->socket_->Open(address, port);
  server_address_ = address;
  server_port_ = port;

  receive_thread_ = std::thread([=] () {
    while (!receive_close_) {
      if (impl_->socket_->WaitToRead(2500)) {
        auto receive = impl_->socket_->ReceiveFrom();
        auto buffer = std::get<0>(receive);

        if (buffer->GetSize() != 0) {
          receive_mutex_.lock(); // LOCK
          receive_buffer_.push(buffer);
          receive_mutex_.unlock(); // UNLOCK
        }
      }
    }
  });
}

void Client::Disconnect() {
  receive_close_ = true;
  if (receive_thread_.joinable()) receive_thread_.join();
  if (send_async_.valid()) send_async_.wait();
  impl_->socket_->Close();
}

std::shared_ptr<ClientMessage> Client::CreateMessage(const int& type) {
  return std::make_shared<ClientMessage>(send_buffer_, type);
}

void Client::ProcessMessages() {
  receive_mutex_.lock(); // LOCK
  receive_queue_.swap(receive_buffer_);
  receive_mutex_.unlock(); // UNLOCK

  while (!receive_queue_.empty()) {
    auto stream = receive_queue_.front();
    while (stream->read_position() < stream->GetSize()) {
      try {
        uint8_t first_byte = stream->ReadByte();

        auto iter = callbacks_.find(first_byte);

        if (iter != callbacks_.end()) {
          auto message = iter->second;
          message(std::make_shared<ReceiveMessage>(stream, first_byte));
        } else {
          throw std::logic_error("Invalid message.");
        }
      } catch (const std::exception& e) {
        throw e;
      }
    }
    receive_queue_.pop();
  }
}

void Client::SendMessages() {
  send_mutex_.lock(); // LOCK
  send_queue_.swap(*send_buffer_);
  send_mutex_.unlock(); // UNLOCK

  send_async_ = std::async(std::launch::async, [=] {
    send_mutex_.lock(); // LOCK
    while (!send_queue_.empty()) {
      impl_->socket_->Send(*send_queue_.front());
      send_queue_.pop();
    }
    send_mutex_.unlock(); // LOCK
  });
}

} // end network namespace
} // end engine namespace