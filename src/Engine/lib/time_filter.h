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

#ifndef TIME_FILTER_H_
#define TIME_FILTER_H_

#include "common.h"

namespace engine {
namespace lib {

class TimeFilter {
public:
  TimeFilter();
  explicit TimeFilter(uint64_t msec);
  virtual ~TimeFilter();

  void Clear();
  bool TryTime(uint64_t msec);

  /* A+M */
  void set_time(uint64_t msec);

private:
  uint64_t time_;
  uint64_t accum_time_;
  uint64_t captured_time_;
};

inline TimeFilter::TimeFilter() {
  time_ = 0;
  accum_time_ = captured_time_ = 0;
}

inline TimeFilter::TimeFilter(uint64_t msec) {
  time_ = msec;
  accum_time_ = captured_time_ = 0;
}

inline TimeFilter::~TimeFilter() {
}

inline void TimeFilter::Clear() {
  time_ = 0;
  accum_time_ = captured_time_ = 0;
}

inline bool TimeFilter::TryTime(uint64_t msec) {
  if (captured_time_ == 0) {
    accum_time_ = captured_time_ = msec;
  } else {
    accum_time_ = msec;
  }

  if (accum_time_ - captured_time_ >= time_) {
    accum_time_ = captured_time_ = 0;
    return true;
  }
  return false;
}

inline void TimeFilter::set_time(uint64_t msec) {
  time_ = msec;
}

} // end lib namespace
} // end engine namespace

#endif /* TIME_FILTER_H_ */