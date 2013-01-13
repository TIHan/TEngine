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

#ifndef SERVER_MESSAGE_PROCESSOR_H_
#define SERVER_MESSAGE_PROCESSOR_H_

#include "receive_message.h"

namespace engine {
namespace network {

class ServerPacket : public ByteStream {
public:
  ServerPacket(int client_id);
  virtual ~ServerPacket();

  // Accessors / Mutators
  int client_id();

private:
  int client_id_;
};

inline ServerPacket::ServerPacket(int client_id) {
  client_id_ = client_id;
}

inline ServerPacket::~ServerPacket() {
}

inline int ServerPacket::client_id() {
  return client_id_;
}

class ServerChannel {
public:
  ServerChannel();
  virtual ~ServerChannel();

  void Push(std::shared_ptr<ByteStream> stream, int client_id);
  void Flush(std::function<void(std::shared_ptr<ServerPacket> packet)> func);

private:
  std::vector<std::shared_ptr<ServerPacket>> packet_buffer_;
  std::mutex mutex_;
};

inline ServerChannel::ServerChannel() {
}

inline ServerChannel::~ServerChannel() {
}

inline void ServerChannel::Push(std::shared_ptr<ByteStream> stream, 
                                       int client_id) {
  auto packet = std::make_shared<ServerPacket>(client_id);

  packet->SwapStreamBuffer(stream);

  mutex_.lock();
  packet_buffer_.push_back(packet);
  mutex_.unlock();
}

inline void ServerChannel::Flush(
    std::function<void(std::shared_ptr<ServerPacket> packet)> func) {
  std::vector<std::shared_ptr<ServerPacket>> packet_buffer;

  mutex_.lock();
  packet_buffer.swap(packet_buffer_);
  mutex_.unlock();

  for (auto packet : packet_buffer) {
    func(packet);
  }
}

class ServerMessageProcessor {
public:
  ServerMessageProcessor();
  virtual ~ServerMessageProcessor();

  virtual void StartReceiving(
      std::function<std::pair<int, std::shared_ptr<ByteStream>>()> func);

  virtual void Send(
      std::function<void(const ByteStream& buffer, uint8_t recipient_id)> func);

  virtual void Stop();
  virtual void Process();
  virtual void RegisterMessageCallback(int type,
      std::function<void(std::shared_ptr<ReceiveMessage>, int)> func);

  virtual void PushOnSend(std::shared_ptr<ByteStream> stream, int client_id);

private:
  std::thread receive_thread_;
  std::atomic_bool receive_close_;
  std::map<int,
           std::function<void(std::shared_ptr<ReceiveMessage>,
                              int)>> callbacks_;

  std::future<void> send_async_;

  ServerChannel receive_channel_;
  ServerChannel send_channel_;
};

} // end network namespace
} // end engine namespace

#endif // CLIENT_MESSAGE_PROCESSOR_H_
