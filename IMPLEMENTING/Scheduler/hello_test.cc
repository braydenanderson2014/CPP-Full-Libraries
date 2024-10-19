#include <gtest/gtest.h>
#include "Scheduler.h"
#include <thread>
#include <chrono>

// Test if Job can be created and scheduled with a function
TEST(SchedulerTest, JobSchedulingTest) {
    Scheduler scheduler;
    bool jobExecuted = false;

    // Create a job to set jobExecuted to true
    scheduler.every(1).setUnit("seconds").setJobFunc([&]() {
        jobExecuted = true;
    });

    // Start the scheduler
    scheduler.runPending();

    // Wait for the job to run
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Check if the job was executed
    ASSERT_TRUE(jobExecuted);
}

// Test that multiple jobs are scheduled and run correctly
TEST(SchedulerTest, MultipleJobTest) {
    Scheduler scheduler;
    int jobCounter1 = 0;
    int jobCounter2 = 0;

    // Create two jobs that increment counters
    scheduler.every(1).setUnit("seconds").setJobFunc([&]() {
        jobCounter1++;
    });
    scheduler.every(2).setUnit("seconds").setJobFunc([&]() {
        jobCounter2++;
    });

    // Start the scheduler
    scheduler.runPending();

    // Wait for 5 seconds
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // Check that the first job has run 5 times and the second job 2-3 times
    ASSERT_EQ(jobCounter1, 5);
    ASSERT_GE(jobCounter2, 2);
    ASSERT_LE(jobCounter2, 3);  // Depending on timing, it may run 2 or 3 times
}

// Test that a job that runs every minute runs on time
TEST(SchedulerTest, JobExecutionTimingTest) {
    Scheduler scheduler;
    auto start = std::chrono::system_clock::now();
    bool jobExecuted = false;

    // Create a job that sets jobExecuted to true
    scheduler.every(1).setUnit("seconds").setJobFunc([&]() {
        jobExecuted = true;
    });

    // Start the scheduler
    scheduler.runPending();

    // Wait for job to execute
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end - start;

    // Check if the job executed around 1 second mark
    ASSERT_TRUE(jobExecuted);
    ASSERT_GE(diff.count(), 1.0);
    ASSERT_LE(diff.count(), 1.5);  // Allow some timing flexibility
}

// Test that the scheduler stops correctly
TEST(SchedulerTest, StopSchedulerTest) {
    Scheduler scheduler;
    int jobCounter = 0;

    // Create a job that increments a counter
    scheduler.every(1).setUnit("seconds").setJobFunc([&]() {
        jobCounter++;
    });

    // Start the scheduler
    scheduler.runPending();

    // Wait for 3 seconds
    std::this_thread::sleep_for(std::chrono::seconds(3));

    // Stop the scheduler
    scheduler.stopSchedulerThread();

    // Wait 2 more seconds (scheduler should be stopped)
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Check that the job ran 3 times and not more
    ASSERT_EQ(jobCounter, 3);
}

// Test exception handling for invalid intervals
TEST(SchedulerTest, IntervalExceptionTest) {
    Scheduler scheduler;

    // Try to schedule a job with an invalid interval
    try {
        scheduler.every(-1).setUnit("seconds").setJobFunc([]() {
            std::cout << "This should not run!" << std::endl;
        });
        FAIL() << "Expected SchedulerIntervalException";
    } catch (const SchedulerIntervalException& e) {
        EXPECT_STREQ(e.what(), "Invalid interval error");
    } catch (...) {
        FAIL() << "Expected SchedulerIntervalException";
    }
}

// Test that job cancellation works
TEST(SchedulerTest, CancelJobTest) {
    Scheduler scheduler;
    int jobCounter = 0;
    Job* jobPtr = nullptr;

    // Create a job that increments a counter
    jobPtr = &scheduler.every(1).setUnit("seconds").setJobFunc([&]() {
        jobCounter++;
    });

    // Start the scheduler
    scheduler.runPending();

    // Wait for the job to run twice
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Cancel the job
    scheduler.stopSchedulerThread();

    // Wait for 2 more seconds (the job should not run anymore)
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Check that the job ran exactly 2 times
    ASSERT_EQ(jobCounter, 2);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
