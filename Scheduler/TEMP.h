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
            std::cout << "Job running at: " << std::chrono::steady_clock::now().time_since_epoch().count() << std::endl;
            jobFunc();
            scheduleNextRun();
        }
    }

    bool shouldRun() const {
        auto now = std::chrono::steady_clock::now();
        bool isDue = now >= nextRun;
        std::cout << "Checking if job should run: " << isDue 
                  << " (now: " << now.time_since_epoch().count() 
                  << ", nextRun: " << nextRun.time_since_epoch().count() << ")" << std::endl;
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
        } else {
            throw SchedulerIntervalException("Unsupported time unit");
        }
        std::cout << "Next run scheduled at: " << nextRun.time_since_epoch().count() << std::endl;
    }

    void setUnit(const std::string& u) {
        unit = u;
        std::cout << "Unit set to: " << unit << std::endl;

    }

    void setInterval(int newInterval) {
        interval = newInterval;
        std::cout << "Interval set to: " << interval << std::endl;
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

    Job& every(int interval) {
    if (interval <= 0) {
        throw SchedulerIntervalException("Invalid interval error");
    }

    std::unique_lock<std::shared_mutex> lock(jobsMutex);
    
    jobs.emplace_back(interval);
    Job& job = jobs.back();
    job.scheduleNextRun();  // Schedule the first run as soon as the job is added
    
    jobCount.fetch_add(1, std::memory_order_relaxed);
    std::cout << "Job added. Total jobs: " << jobCount.load() << std::endl;

    return job;
}


/*

void run() {
        while (running) {
            std::unique_lock<std::mutex> lock(mutex);
            if (jobs.isEmpty()) {
                cv.wait(lock, [this] { return !running || !jobs.isEmpty(); });
            } else {
                auto nextRun = getNextRunTime();
                if (cv.wait_until(lock, nextRun, [this] { return !running || !jobs.isEmpty(); })) {
                    if (!running) break;
                }

                auto now = std::chrono::system_clock::now();
                for (std::size_t i = 0; i < jobs.elements(); ++i) {
                    if (jobs[i].shouldRun() && now >= jobs[i].getNextRun()) {
                        // Release the lock before running the job
                        lock.unlock();
                        jobs[i].run();
                        lock.lock();
                    }
                }
            }
        }
    }


*/
    
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
        std::cout << "Scheduler run method started" << std::endl;
        while (running.load(std::memory_order_acquire)) {
            auto now = std::chrono::steady_clock::now();
            auto nextRun = now + std::chrono::milliseconds(1);  // Check every 1ms

            {
                std::shared_lock<std::shared_mutex> lock(jobsMutex);
                unsigned int count = jobCount.load(std::memory_order_relaxed);
                std::cout << "Checking " << count << " jobs" << std::endl;
                for (unsigned int i = 0; i < count; ++i) {
                    if (jobs[i].shouldRun()) {
                        std::cout << "Running job " << i << " at time: " << now.time_since_epoch().count() << std::endl;
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
        std::cout << "Scheduler run method ended" << std::endl;
    }



    SimpleVector<Job> jobs;
    std::thread schedulerThread;
    std::atomic<bool> running{false};
    std::condition_variable_any cv;  // Changed this line
    mutable std::shared_mutex jobsMutex;
    std::atomic<size_t> jobCount{0};
};  //std::mutex mutex;


#endif // SCHEDULER_H
