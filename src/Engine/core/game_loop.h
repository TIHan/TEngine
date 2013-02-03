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
  GameLoop(std::unique_ptr<EventSystem> event_system, int ticks_per_second);
  virtual ~GameLoop();

  void Run(std::function<void()> internal_loop);
  void Stop();

  /* A+M */
  bool running();

protected:
  uint64_t GetTickRate();
  uint64_t GetTickTime();

private:
  // Time
  std::chrono::high_resolution_clock clock_;
  std::chrono::nanoseconds rate_;
  uint64_t current_tick_;

  // Events
  std::unique_ptr<EventSystem> event_system_;

  std::atomic<bool> running_;
};

inline GameLoop::GameLoop(std::unique_ptr<EventSystem> event_system,
                          int ticks_per_second)
    : event_system_(std::move(event_system)),
      rate_(static_cast<int>((1.0 / ticks_per_second) * 1000000000)) {
  current_tick_ = 0;
  running_ = false;
}

inline GameLoop::~GameLoop() {
  Stop();
}

inline void GameLoop::Run(std::function<void()> internal_loop) {
  current_tick_ = 0;
  running_ = true;

  std::chrono::system_clock::time_point time;
  while(running_) {
    time = clock_.now();

    event_system_->GetAggregator()->Publish<TimeMessage>(GetTickTime());
    event_system_->GetProcessor()->Process();
    internal_loop();

    current_tick_++;
    std::cout << "TIME: " << std::chrono::duration_cast<std::chrono::milliseconds>(rate_ - (clock_.now() - time)).count() << std::endl;
    std::this_thread::sleep_for(rate_ - (clock_.now() - time));
  }
}

inline void GameLoop::Stop() {
  running_ = false;
}

inline bool GameLoop::running() {
  return running_;
}

inline uint64_t GameLoop::GetTickRate() {
  return static_cast<uint64_t>(1000000000 / rate_.count());
}

inline uint64_t GameLoop::GetTickTime() {
  return static_cast<uint64_t>(static_cast<float>(current_tick_) /
         static_cast<float>(GetTickRate()) * 1000);
}

} // end core namespace
} // end engine namespace

#endif /* GAME_LOOP_H_ */