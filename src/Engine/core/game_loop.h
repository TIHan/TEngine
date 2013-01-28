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

#ifndef GAME_LOOP_H_
#define GAME_LOOP_H_

#include <engine_lib.h>
#include <engine_network.h>

namespace engine {
namespace core {

class GameLoop {
public:
  GameLoop(std::shared_ptr<EventProcessor> event_processor,
           std::shared_ptr<EventAggregator> event_aggregator,
           int ticks_per_second);
  virtual ~GameLoop();

  void Run(std::function<void()> internal_loop);
  void Stop();

  /* A+M */
  bool running();

protected:
  int GetTickRate();

private:
  // Time
  std::chrono::high_resolution_clock clock_;
  std::chrono::nanoseconds rate_;
  int current_tick_;

  // Events
  std::shared_ptr<EventProcessor> event_processor_;
  std::shared_ptr<EventAggregator> event_aggregator_;

  std::atomic<bool> running_;
};

inline GameLoop::GameLoop(std::shared_ptr<EventProcessor> event_processor,
                          std::shared_ptr<EventAggregator> event_aggregator,
                          int ticks_per_second)
    : event_processor_(event_processor),
      event_aggregator_(event_aggregator),
      rate_(static_cast<int>((1.0 / ticks_per_second) * 1000000000)) {
  running_ = false;
  current_tick_ = 0;
}

inline GameLoop::~GameLoop() {
  Stop();
}

inline void GameLoop::Run(std::function<void()> internal_loop) {
  running_ = true;
  while(running_) {
    auto pre = clock_.now();

    event_processor_->Process();

    // TODO: This needs some cleaning up.
    current_tick_ = (current_tick_ == GetTickRate() ? 0 : current_tick_) + 1;
    event_aggregator_->Publish<TimeMessage>(current_tick_ / GetTickRate() * 1000);

    internal_loop();

    auto post = clock_.now();
    std::this_thread::sleep_for(rate_ - (post - pre));
  }
}

inline void GameLoop::Stop() {
  running_ = false;
}

inline bool GameLoop::running() {
  return running_;
}

inline int GameLoop::GetTickRate() {
  return static_cast<int>(1000000000 / rate_.count());
}

} // end core namespace
} // end engine namespace

#endif /* GAME_LOOP_H_ */