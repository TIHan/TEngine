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

#ifndef SERVER_CHANNEL_H_
#define SERVER_CHANNEL_H_

#include "server_packet.h"

namespace engine {
namespace network {

enum TransferMethod {
  kUnreliable,
  kUnreliableSequenced,
  kReliable,
  kReliableSequenced,
  kReliableOrdered
};

class Packet : public ByteStream {
private: 
  int recipient_id_;
  int sequence_number_;
  TransferMethod transfer_method_;
};

class Channel {
public:
  Channel();
  virtual ~Channel();

  void Push(std::shared_ptr<ByteStream> stream);
  void Flush(std::function<void(std::shared_ptr<ByteStream> stream)> iterator);

private:
  std::vector<std::shared_ptr<ByteStream>> stream_buffer_;
  std::mutex mutex_;
};

inline void Channel::Push(std::shared_ptr<ByteStream> stream) {
    if (stream->GetSize() == 0)
      return;

    mutex_.lock();
    stream_buffer_.push_back(stream);
    mutex_.unlock();
}

inline void Channel::Flush(
  std::function<void(std::shared_ptr<ByteStream> stream)> func) {
    if (stream_buffer_.empty()) {
      return;
    }

    std::vector<std::shared_ptr<ByteStream>> stream_buffer;

    mutex_.lock();
    stream_buffer.swap(stream_buffer_);
    mutex_.unlock();

    for (auto packet : stream_buffer) {
      func(packet);
    }
}

class Router {
public:
  Router();
  virtual ~Router();

  void Push(std::shared_ptr<ByteStream> stream);
  void Flush(std::function<void(std::shared_ptr<ByteStream> stream)> iterator);

private:
  Channel unreliable_channel_;
  Channel unreliable_sequenced_channel_;
  Channel reliable_channel_;
  Channel reliable_sequenced_channel_;
  Channel reliable_ordered_channel_;
};

inline Router::Router() {

}

inline Router::~Router() {

}

inline void Router::Push(std::shared_ptr<ByteStream> stream) {
  switch (stream->ReadByte()) {
    case TransferMethod::kUnreliable: {
      unreliable_channel_.Push(stream);
      break;
    }
    case TransferMethod::kUnreliableSequenced: {
      unreliable_sequenced_channel_.Push(stream);
      break;
    }
    case TransferMethod::kReliable: {
      reliable_channel_.Push(stream);
      break;
    }
    case TransferMethod::kReliableSequenced: {
      reliable_sequenced_channel_.Push(stream);
      break;
    }
    case TransferMethod::kReliableOrdered: {
      reliable_ordered_channel_.Push(stream);
      break;
    }
    default:
      throw std::domain_error("Invalid transfer method on stream.");
  }
}

inline void Router::Flush(
    std::function<void(std::shared_ptr<ByteStream> stream)> iterator) {
  unreliable_channel_.Flush(iterator);
  unreliable_sequenced_channel_.Flush(iterator);
  reliable_channel_.Flush(iterator);
  reliable_sequenced_channel_.Flush(iterator);
  reliable_ordered_channel_.Flush(iterator);
}


class ServerChannel {
public:
  ServerChannel();
  virtual ~ServerChannel();

  void Push(std::pair<int, std::shared_ptr<ByteStream>> key_stream);
  void Flush(std::function<void(std::shared_ptr<ServerPacket> packet)> func);

private:
  std::vector<std::shared_ptr<ServerPacket>> packet_buffer_;
  std::mutex mutex_;
};

inline ServerChannel::ServerChannel() {
}

inline ServerChannel::~ServerChannel() {
}

inline void ServerChannel::Push(
    std::pair<int, std::shared_ptr<ByteStream>> key_stream) {
  auto packet = std::make_shared<ServerPacket>(key_stream);

  if (packet->GetSize() == 0)
    return;

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

} // end network namespace
} // end engine namespace

#endif // SERVER_CHANNEL_H_