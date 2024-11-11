#ifndef TIMER_H
#define TIMER_H

#include <cstdint>
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <functional>
#include <string>
#include <mutex>

class Timer {
private:
    uint32_t mInterval; // Interval for timer in milliseconds
    uint32_t mStartTicks; // When the timer was started
    uint32_t mPausedTicks; // Paused time

    bool mPaused; // Pause status
    bool mStarted; // Start status
    bool mLooping; // Looping status

    std::string TimerName;
    std::string TimerType;

    std::thread timerThread;
    std::atomic<bool> running;
    std::function<void()> callback;
    bool runInSeparateThread;

    // To check if the timer is running on time
    std::chrono::steady_clock::time_point lastRunTime;

    void run() {
        while (running.load()) {
            auto currentTime = std::chrono::steady_clock::now();
            std::this_thread::sleep_until(lastRunTime + std::chrono::milliseconds(mInterval));

            if (running.load() && callback) {
                callback();
                
                // Calculate if the timer is behind schedule
                auto timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - lastRunTime).count();
                if (timeElapsed > mInterval) {
                    std::cout << "[WARNING]: '" << TimerName << "' is behind by " << (timeElapsed - mInterval) << " ms" << std::endl;
                }
                
                lastRunTime = std::chrono::steady_clock::now(); // Update last run time
            }

            if (!mLooping) {
                running.store(false); // Stop if not looping
            }
        }
    }

public:
    Timer() : mInterval(1000), mStartTicks(0), mPausedTicks(0), mPaused(false), mStarted(false),
              mLooping(false), running(false), runInSeparateThread(false) {}

    Timer(uint32_t interval, const std::string& timeType, const std::string& timerName, bool isLooping = false)
        : mInterval(interval), TimerType(timeType), TimerName(timerName), mLooping(isLooping),
          mPaused(false), mStarted(false), running(false), runInSeparateThread(false) {}

    ~Timer() {
        stop();
    }

    void start() {
        mStarted = true;
        mPaused = false;
        mStartTicks = getTicks();
        lastRunTime = std::chrono::steady_clock::now();

        if (runInSeparateThread) {
            running.store(true);
            timerThread = std::thread(&Timer::run, this);
        } else {
            running.store(true);
            run(); // Runs in the same thread if `runInSeparateThread` is false
        }
    }

    void stop() {
        running.store(false);
        mStarted = false;
        mPaused = false;

        if (timerThread.joinable()) {
            timerThread.join();
        }
    }

    void pause() {
        if (mStarted && !mPaused) {
            mPaused = true;
            mPausedTicks = getTicks() - mStartTicks;
        }
    }

    void unpause() {
        if (mStarted && mPaused) {
            mPaused = false;
            mStartTicks = getTicks() - mPausedTicks;
            mPausedTicks = 0;
        }
    }

    uint32_t getTicks() const {
        if (mStarted) {
            return mPaused ? mPausedTicks : getCurrentTime() - mStartTicks;
        }
        return 0;
    }

    bool isStarted() const {
        return mStarted;
    }

    bool isPaused() const {
        return mPaused;
    }

    void printStatus() const {
        std::cout << "Timer '" << TimerName << "' [" << TimerType << "] - Started: " << mStarted
                  << ", Paused: " << mPaused << ", Interval: " << mInterval << " ms" << std::endl;
    }

    void setCallback(const std::function<void()>& func) {
        callback = func;
    }

    void setRunInSeparateThread(bool value) {
        runInSeparateThread = value;
    }

    uint32_t getCurrentTime() const {
        return static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count());
    }

    void setTimerName(const std::string& name) {
        TimerName = name;
    }

    void setTimerType(const std::string& type) {
        TimerType = type;
    }

    std::string getTimerName() const {
        return TimerName;
    }

    std::string getTimerType() const {
        return TimerType;
    }

    void setInterval(uint32_t interval) {
        mInterval = interval;
    }

    uint32_t getInterval() const {
        return mInterval;
    }

    void setLooping(bool isLooping) {
        mLooping = isLooping;
    }

    bool isLooping() const {
        return mLooping;
    }
};

#endif // TIMER_H
