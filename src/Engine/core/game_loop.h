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

namespace engine {
namespace core {

class GameLoop {
public:
  GameLoop(int ticks_per_second);
  virtual ~GameLoop();

  void Run(std::function<void()> internal_loop);
  void Stop();

  int GetPerformanceRate();
  uint64_t GetTickTime();
  int GetTickRate();

  /* A+M */
  bool running();

private:
  std::chrono::high_resolution_clock::time_point current_time_;
  std::chrono::duration<uint64_t, std::nano> sleep_time_;
  std::chrono::high_resolution_clock clock_;
  std::chrono::nanoseconds rate_;
  std::atomic<bool> running_;
  uint64_t current_tick_;
};

inline GameLoop::GameLoop(int ticks_per_second)
    : rate_(static_cast<int>((1.0 / ticks_per_second) * 1000000000)) {
  running_ = false;
  current_tick_ = 0;
}

inline GameLoop::~GameLoop() {
  Stop();
}

inline void GameLoop::Run(std::function<void()> internal_loop) {
  if (running_)
    throw std::logic_error("Already running.");

  running_ = true;
  do {
    current_time_ = clock_.now();

    internal_loop();

    current_tick_++;
    sleep_time_ = rate_ - (clock_.now() - current_time_);
    std::this_thread::sleep_for(sleep_time_);
  } while(running_);
}

inline void GameLoop::Stop() {
  running_ = false;
  current_tick_ = 0;
}

inline int GameLoop::GetPerformanceRate() {
  return static_cast<int>(
      std::chrono::duration_cast<std::chrono::milliseconds>(
      sleep_time_).count());
}

inline uint64_t GameLoop::GetTickTime() {
  return static_cast<uint64_t>(static_cast<float>(current_tick_) /
         static_cast<float>(GetTickRate()) * 1000);
}

inline int GameLoop::GetTickRate() {
  return static_cast<int>(1000000000 / rate_.count());
}

inline bool GameLoop::running() {
  return running_;
}

} // end core namespace
} // end engine namespace

#endif /* GAME_LOOP_H_ */