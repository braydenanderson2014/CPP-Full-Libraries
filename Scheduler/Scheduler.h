#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <iostream>
#include <functional>
#include "SimpleVector.h"
#include <chrono>
#include <string>
#include "Map.h"
#include <thread>
#include <memory>
#include <atomic>
#include <condition_variable>
#include <ctime>
#include <mutex>
#include <shared_mutex>

// Exception classes
#ifndef SCHEDULEREXCEPTION
    #define SCHEDULEREXCEPTION
        class SchedulerException : public std::exception {
        public:
            explicit SchedulerException(const std::string& message) : msg_(message) {}
            virtual const char* what() const noexcept {
                return msg_.c_str();
            }
        private:
            std::string msg_;
        };
    #endif // SCHEDULEREXCEPTION

#ifndef SCHEDULERVALUEEXCEPTION
    #define SCHEDULERVALUEEXCEPTION
        class SchedulerValueException : public std::exception {
        public:
            explicit SchedulerValueException(const std::string& message) : msg_(message) {}
            virtual const char* what() const noexcept {
                return msg_.c_str();
            }
        private:
            std::string msg_;
        };
    #endif // SCHEDULERVALUEEXCEPTION

#ifndef SCHEDULERINTERVALEXCEPTION
    #define SCHEDULERINTERVALEXCEPTION
        class SchedulerIntervalException : public std::exception {
        public:
            explicit SchedulerIntervalException(const std::string& message) : msg_(message) {}
            virtual const char* what() const noexcept {
                return msg_.c_str();
            }
        private:
            std::string msg_;
        };
    #endif // SCHEDULERINTERVALEXCEPTION

// Job class definition
class Job {
public:
    Job() : interval(0), unit("seconds"), jobFunc(nullptr), cancelled(false) {}

    Job(int interval) : interval(interval), unit("seconds"), jobFunc(nullptr), cancelled(false) {}

    // Delete copy constructor and copy assignment operator
    Job(const Job&) = delete;
    Job& operator=(const Job&) = delete;

    // Add move constructor and move assignment operator
    Job(Job&& other) noexcept
        : interval(other.interval),
          unit(std::move(other.unit)),
          jobFunc(std::move(other.jobFunc)),
          nextRun(other.nextRun),
          cancelled(other.cancelled.load()) {}

    Job& operator=(Job&& other) noexcept {
        if (this != &other) {
            interval = other.interval;
            unit = std::move(other.unit);
            jobFunc = std::move(other.jobFunc);
            nextRun = other.nextRun;
            cancelled = other.cancelled.load();
        }
        return *this;
    }

    bool operator<(const Job& other) const {
        return nextRun < other.nextRun;
    }

    void setJobFunc(std::function<void()> func) {
        jobFunc = std::move(func);
    }

    void run() {
        if (!cancelled && jobFunc) {
            jobFunc();
            scheduleNextRun();
        }
    }

    bool shouldRun() const {
        auto now = std::chrono::steady_clock::now();
        bool isDue = now >= nextRun;
        return isDue && !cancelled;
    }

    void scheduleNextRun() {
        auto now = std::chrono::steady_clock::now();  // Use steady_clock for accurate intervals    
        if (unit == "milliseconds") {
            nextRun = now + std::chrono::milliseconds(interval);
        } else if (unit == "seconds") {
            nextRun = now + std::chrono::seconds(interval);
        } else if (unit == "minutes") {
            nextRun = now + std::chrono::minutes(interval);
        } else if (unit == "hours") {
            nextRun = now + std::chrono::hours(interval);
        } else if (unit == "days"){
            nextRun = now + std::chrono::hours(interval * 24);
        } else if (unit == "weeks") {
            nextRun = now + std::chrono::hours(interval * 24 * 7);
        } else if (unit == "months") {
            nextRun = now + std::chrono::hours(interval * 24 * 30);
        } else if (unit == "years") {
            nextRun = now + std::chrono::hours(interval * 24 * 365);
        } else if (unit == "gameTicks") {
            nextRun = now + std::chrono::nanoseconds(interval * 16666666);
        } else if (unit == "gameFrames") {
            nextRun = now + std::chrono::nanoseconds(interval * 16666666 * 60);
        } else if (unit == "gameSeconds") {
            nextRun = now + std::chrono::nanoseconds(interval * 16666666 * 60 * 60);
        } else if (unit == "gameMinutes") {
            nextRun = now + std::chrono::nanoseconds(interval * 16666666 * 60 * 60 * 60);
        } else if (unit == "gameHours") {
            nextRun = now + std::chrono::nanoseconds(interval * 16666666 * 60 * 60 * 60 * 60);
        } else if (unit == "gameDays") {
            nextRun = now + std::chrono::nanoseconds(interval * 16666666 * 60 * 60 * 60 * 60 * 24);
        } else if (unit == "gameWeeks") {
            nextRun = now + std::chrono::nanoseconds(interval * 16666666 * 60 * 60 * 60 * 60 * 24 * 7);
        } else if (unit == "gameMonths") {
            nextRun = now + std::chrono::nanoseconds(interval * 16666666 * 60 * 60 * 60 * 60 * 24 * 30);
        } else if (unit == "gameYears") {
            nextRun = now + std::chrono::nanoseconds(interval * 16666666 * 60 * 60 * 60 * 60 * 24 * 365);
        } else {
            throw SchedulerIntervalException("Unsupported time unit");
        } 
    }

    void setUnit(const std::string& u) {
        unit = u;
    }

    void setInterval(int newInterval) {
        interval = newInterval;
    }

    std::chrono::steady_clock::time_point getNextRun() const {
        return nextRun;
    }

    void cancel() {
        cancelled = true;
    }

    bool isCancelled() const {
        return cancelled;
    }

private:
    int interval;
    std::string unit;
    std::function<void()> jobFunc;
    std::chrono::steady_clock::time_point nextRun;
    std::atomic<bool> cancelled;
};

// Scheduler class definition
class Scheduler {
public:
    Scheduler() : running(false) {}

    void startSchedulerThread() {
        bool expected = false;
        if (running.compare_exchange_strong(expected, true)) {
            schedulerThread = std::thread(&Scheduler::run, this);
        }
    }

    void stopSchedulerThread() {
        running.store(false);
        cv.notify_all();
        if (schedulerThread.joinable()) {
            schedulerThread.join();
        }
    }

    Job& every(int interval, const std::string& unit = "milliseconds") {
        if (interval <= 0) {
            throw SchedulerIntervalException("Invalid interval error");
        }
        std::unique_lock<std::shared_mutex> lock(jobsMutex);
        jobs.emplace_back(interval);
        Job& job = jobs.back();
        job.setUnit(unit);  // Set the unit before scheduling the first run
        job.scheduleNextRun();  // Schedule the first run as soon as the job is added
        jobCount.fetch_add(1, std::memory_order_relaxed);
        return job;
    }

    std::chrono::steady_clock::time_point getNextRunTime() {
        if (jobs.isEmpty()) return std::chrono::steady_clock::now() + std::chrono::hours(24);
        std::chrono::steady_clock::time_point soonest = jobs[0].getNextRun();
        for (std::size_t i = 1; i < jobs.elements(); ++i) {
            if (jobs[i].getNextRun() < soonest) {
                soonest = jobs[i].getNextRun();
            }
        }
        return soonest;
    }

    void runPending() {
        startSchedulerThread();
    }

    ~Scheduler() {
        stopSchedulerThread();
    }

private:
    void run() {
        while (running.load(std::memory_order_acquire)) {
            auto now = std::chrono::steady_clock::now();
            auto nextRun = now + std::chrono::nanoseconds(1);  // Check every 1ns
            {
                std::shared_lock<std::shared_mutex> lock(jobsMutex);
                unsigned int count = jobCount.load(std::memory_order_relaxed);
                for (unsigned int i = 0; i < count; ++i) {
                    if (jobs[i].shouldRun()) {
                        Job* job = &jobs[i];
                        lock.unlock();
                        job->run();  // Run the job
                        job->scheduleNextRun();  // Schedule its next run
                        lock.lock();
                    }
                    nextRun = std::min(nextRun, jobs[i].getNextRun());
                }
            }
            std::this_thread::sleep_until(nextRun);
        }
    }
    SimpleVector<Job> jobs;
    std::thread schedulerThread;
    std::atomic<bool> running{false};
    std::condition_variable_any cv;  // Changed this line
    mutable std::shared_mutex jobsMutex;
    std::atomic<size_t> jobCount{0};
};

#endif // SCHEDULER_H