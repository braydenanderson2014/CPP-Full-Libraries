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
    Job(int interval) : interval(interval) {}

    bool operator<(const Job& other) const {
        return nextRun < other.nextRun;
    }

    void setJobFunc(std::function<void()> func) {
        jobFunc = func;
    }

    void run() {
        jobFunc();
        scheduleNextRun();
    }

    bool shouldRun() const {
        return std::chrono::system_clock::now() >= nextRun;
    }

    void scheduleNextRun() {
        auto now = std::chrono::system_clock::now();
        if (unit == "seconds") {
            nextRun = now + std::chrono::seconds(interval);
        } else if (unit == "minutes") {
            nextRun = now + std::chrono::minutes(interval);
        } else if (unit == "hours") {
            nextRun = now + std::chrono::hours(interval);
        }
    }

    void setUnit(const std::string& u) {
        unit = u;
    }

    std::chrono::system_clock::time_point getNextRun() const {
        return nextRun;
    }

private:
    int interval;
    std::string unit;
    std::function<void()> jobFunc;
    std::chrono::system_clock::time_point nextRun;
};

// Scheduler class definition
class Scheduler {
public:
    Scheduler() : running(false) {}

    void startSchedulerThread() {
        if (!running) {
            running = true;
            schedulerThread = std::thread(&Scheduler::run, this);
        }
    }

    void stopSchedulerThread() {
        running = false;
        if (schedulerThread.joinable()) {
            schedulerThread.join();
        }
    }

    Job& every(int interval) {
        jobs.push_back(Job(interval));  // Add the job directly to the jobs vector
        cv.notify_one();  // Notify the thread to check the new job
        return jobs.back();  // Return a reference to the last job in the vector
    }

    void run() {
        while (running) {
            std::unique_lock<std::mutex> lk(cv_m);

            if (jobs.isEmpty()) {
                cv.wait(lk);  // Wait if there are no jobs
            } else {
                auto nextRun = getNextRunTime();
                cv.wait_until(lk, nextRun);  // Wait until the next job is due

                // Run pending jobs
                for (std::size_t i = 0; i < jobs.size(); ++i) {
                    if (jobs[i].shouldRun()) {
                        jobs[i].run();
                    }
                }
            }
        }
    }

    std::chrono::system_clock::time_point getNextRunTime() {
        std::chrono::system_clock::time_point soonest = jobs[0].getNextRun();
        for (std::size_t i = 1; i < jobs.size(); ++i) {
            if (jobs[i].getNextRun() < soonest) {
                soonest = jobs[i].getNextRun();
            }
        }
        return soonest;
    }

    void runPending() {
        startSchedulerThread();  // Start the background thread
    }

    ~Scheduler() {
        stopSchedulerThread();  // Ensure the thread stops on exit
    }

private:
    SimpleVector<Job> jobs;
    std::thread schedulerThread;
    std::atomic<bool> running;
    std::condition_variable cv;
    std::mutex cv_m;
};

#endif // SCHEDULER_H
