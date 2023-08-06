#include "timer.h"



Timer::Timer(int _max_ticks)
    : max_ticks(_max_ticks), running(false), ticks(0)
{
}



Timer::~Timer()
{
}



void Timer::start()
{
    running = true;
}



void Timer::reset()
{
    running = false;
    ticks = 0;
}



void Timer::set_to_new_value(int _new_max_ticks)
{
    ticks = _new_max_ticks;
}



void Timer::update()
{
    if (running)
        ticks++;
}



bool Timer::check()
{
    return ticks >= max_ticks;
}
