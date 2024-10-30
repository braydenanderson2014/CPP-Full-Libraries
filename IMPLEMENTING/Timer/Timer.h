#ifndef TIMER_H
#define TIMER_H

#include <SDL2/SDL.h>
#include <iostream>


class Timer {
private:
    Uint32 mStartTicks; // The clock time when the timer started
    Uint32 mPausedTicks; // The ticks stored when the timer was paused

    bool mPaused; // The timer status
    bool mStarted; // The timer status

public:
    Timer();
    ~Timer();

    void start();
    void stop();
    void pause();
    void unpause();

    Uint32 getTicks();

    bool isStarted();
    bool isPaused();

    void printStatus();






};

#endif // TIMER_H