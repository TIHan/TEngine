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

#include "server_reactor.h"

namespace engine {
namespace network {

/********************************************//**
 * The constructor for the ServerReactor.
 ***********************************************/
ServerReactor::ServerReactor()
    : send_channel_(std::make_shared<ServerChannel>()) {
  receive_close_ = false;
}

/********************************************//**
 * The destructor for the ServerReactor.
 * Stop all threads and asynchronous operations.
 ***********************************************/
ServerReactor::~ServerReactor() {
  Stop();
}

/********************************************//**
 * Takes an anonymous function as its parameter which 
 * will contain the functionality on receiving a 
 * client_id and a byte_stream in a looping thread.
 * 
 * Implementation of this function is recommended to contain
 * a "synchronous event demultiplexer", such as the method
 * "select()" for I/O. This is to prevent the thread from
 * running too quickly and using unnecessary CPU time.
 ***********************************************/
void ServerReactor::StartReceiving(
    std::function<std::pair<int, std::shared_ptr<ByteStream>>()> func) {
  receive_thread_ = std::thread([=] () {
    while (!receive_close_) {
        receive_channel_.Push(func());
    }
  });
}

/********************************************//**
 * Takes an anonymous function as its parameter which
 * will contain the functionality on sending data
 * to the specified client_id.
 * 
 * The send_channel will flush out all of the packets
 * while exposing iteration of the packets in an
 * anonymous function. This is used to execute
 * sending each flushed packet with the corresponding
 * client_id.
 ***********************************************/
void ServerReactor::Send(
    std::function<void(const ByteStream& buffer, uint8_t client_id)> func) {
  send_async_ = std::async(std::launch::async, [=] {
    send_channel_->Flush([=] (std::shared_ptr<ServerPacket> packet) {
      func(*packet, packet->client_id());
    });
  });
}

/********************************************//**
 * Stops the receiving thread and the sending
 * asynchronous operation.
 ***********************************************/
void ServerReactor::Stop() {
  receive_close_ = true;
  if (receive_thread_.joinable()) receive_thread_.join();
  if (send_async_.valid()) send_async_.wait();
}

/********************************************//**
 * Essentially dispatches each message synchronously
 * in the receive channel to its corresponding
 * function (message handler).
 ***********************************************/
void ServerReactor::Process() {
  receive_channel_.Flush([=] (std::shared_ptr<ServerPacket> packet) {
    while (packet->CanRead()) {
      try {
        message_handler_.Handle(packet);
      } catch (const std::runtime_error& e) {
        throw e;
      }
    }
  });
}

/********************************************//**
 * Allows the registration of an message type,
 * described by an "int", to a function (handler).
 ***********************************************/
void ServerReactor::RegisterMessageCallback(int type,
    std::function<void(std::shared_ptr<ReceiveMessage>, int)> func) {
  message_handler_.RegisterHandler(type, func);
}

/********************************************//**
 * Creates a message to be sent to one or more
 * clients on a server.
 ***********************************************/
std::unique_ptr<ServerMessage> ServerReactor::CreateMessage(int type,
    std::shared_ptr<std::list<int>> client_ids) {
  return std::make_unique<ServerMessage>(type, client_ids, send_channel_);
}

} // end network namespace
} // end engine namespace