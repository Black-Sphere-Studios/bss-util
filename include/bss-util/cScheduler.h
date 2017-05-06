// Copyright �2017 Black Sphere Studios
// For conditions of distribution and use, see copyright notice in "bss_util.h"

#ifndef __C_SCHEDULER_H__BSS__
#define __C_SCHEDULER_H__BSS__

#include "cHighPrecisionTimer.h"
#include "cBinaryHeap.h"

namespace bss_util {
  // Scheduler object that lets you schedule events that happen x milliseconds into the future. If the event returns a number greater than 0,it will be rescheduled. 
  template<typename F, typename ST = uint32_t> //std::function<double(void)>
  class BSS_COMPILER_DLLEXPORT cScheduler : protected cHighPrecisionTimer, protected cBinaryHeap<std::pair<double, F>, ST, CompTFirst<std::pair<double, F>, CompTInv<double>>, CARRAY_SAFE>
  {
    typedef cBinaryHeap<std::pair<double, F>, ST, CompTFirst<std::pair<double, F>, CompTInv<double>>, CARRAY_SAFE> BASE;
  public:
    // Constructor
    inline cScheduler() {}
    inline cScheduler(const cScheduler& copy) : BASE(copy), cHighPrecisionTimer(copy) {}
    inline cScheduler(cScheduler&& mov) : BASE(std::move(mov)), cHighPrecisionTimer(mov) {}
    inline cScheduler(double t, const F& f) { Add(t, f); }
    inline cScheduler(double t, F&& f) { Add(t, std::move(f)); }
    inline ~cScheduler() {}
    // Gets number of events
    BSS_FORCEINLINE ST Length() const { return BASE::_length; }
    // Adds an event that will happen t milliseconds in the future, starting from the current time
    BSS_FORCEINLINE void Add(double t, const F& f) { BASE::Insert(std::pair<double, F>(t + _time, f)); }
    BSS_FORCEINLINE void Add(double t, F&& f) { BASE::Insert(std::pair<double, F>(t + _time, std::move(f))); }
    // Updates the scheduler, setting off any events that need to be set off
    inline void Update()
    {
      cHighPrecisionTimer::Update();
      while(BASE::Peek().first <= _time)
      {
        double r = BASE::Peek().second();
        if(r == 0.0)
          BASE::Remove(0);
        else
          BASE::Set(0, std::pair<double, F>(r + _time, BASE::Peek().second)); // This is why we don't use the actual priority queue data structure
      }
    }
  };
}

#endif