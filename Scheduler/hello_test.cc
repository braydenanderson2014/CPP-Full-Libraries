#include <gtest/gtest.h>
#include "Scheduler.h"
#include <thread>
#include <chrono>

// Test if Job can be created and scheduled with a function


TEST(SchedulerTest, JobSchedulingTest) {
    Scheduler scheduler;
    bool jobExecuted = false;
    Job& myJob = scheduler.every(1);
    myJob.setUnit("seconds");
    myJob.setJobFunc([&]() {
        jobExecuted = true;
    });
}



// Test that multiple jobs are scheduled and run correctly

    TEST(SchedulerTest, MultipleJobTest) {
    Scheduler scheduler;
    int jobCounter1 = 0;
    int jobCounter2 = 0;

    Job& myJob1 = scheduler.every(1, "seconds");
    myJob1.setJobFunc([&]() {
        jobCounter1++;
    });

    Job& myJob2 = scheduler.every(2);
    myJob2.setUnit("seconds");
    myJob2.setJobFunc([&]() {
        jobCounter2++;
    });

    scheduler.runPending();
    std::this_thread::sleep_for(std::chrono::seconds(6));  // Increased wait time

    scheduler.stopSchedulerThread();

    ASSERT_GE(jobCounter1, 4);  // Allow for some flexibility
    ASSERT_LE(jobCounter1, 6);
    ASSERT_GE(jobCounter2, 2);
    ASSERT_LE(jobCounter2, 3);
}



// Test that a job that runs every minute runs on time

    TEST(SchedulerTest, JobExecutionTimingTest) {
    Scheduler scheduler;
    std::atomic<bool> jobExecuted{false};

    Job& myJob = scheduler.every(1);
    myJob.setUnit("seconds");
    myJob.setJobFunc([&]() {
        jobExecuted.store(true, std::memory_order_release);
    });

    scheduler.runPending();
    std::this_thread::sleep_for(std::chrono::seconds(2));  // Increased wait time

    scheduler.stopSchedulerThread();

    ASSERT_TRUE(jobExecuted.load(std::memory_order_acquire));
}

// Test that the scheduler stops correctly
TEST(SchedulerTest, StopSchedulerTest) {
    Scheduler scheduler;
    std::atomic<int> jobCounter{0};

    Job& myJob = scheduler.every(1);
    myJob.setUnit("seconds");
    myJob.setJobFunc([&]() {
        jobCounter.fetch_add(1, std::memory_order_relaxed);
    });

    scheduler.runPending();
    std::this_thread::sleep_for(std::chrono::seconds(3));

    scheduler.stopSchedulerThread();

    // Allow for some flexibility in the number of executions
    ASSERT_GE(jobCounter.load(std::memory_order_relaxed), 2);
    ASSERT_LE(jobCounter.load(std::memory_order_relaxed), 4);
}




// Test exception handling for invalid intervals
TEST(SchedulerTest, IntervalExceptionTest) {
    Scheduler scheduler;

    // Try to schedule a job with an invalid interval
    try {
        Job& myJob = scheduler.every(-1);
        myJob.setUnit("seconds");
        myJob.setJobFunc([]() {
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

    Job& jobPtr = scheduler.every(1, "seconds");
    jobPtr.setJobFunc([&]() {
        jobCounter++;
    });

    scheduler.runPending();
    std::this_thread::sleep_for(std::chrono::seconds(3));

    // Small buffer before stopping the scheduler to prevent over-execution
    //std::this_thread::sleep_for(std::chrono::milliseconds(100));
    scheduler.stopSchedulerThread();

    ASSERT_EQ(jobCounter, 2);  // The job should have run exactly 2 times
}





void externalFunction(int& counter) {
    counter++;
}

TEST(SchedulerTest, FunctionCallbackTest) {
    Scheduler scheduler;
    int functionCallCounter = 0;

    Job& myJob = scheduler.every(1, "seconds");
    myJob.setJobFunc([&]() {
        externalFunction(functionCallCounter);
    });

    scheduler.runPending();
    std::this_thread::sleep_for(std::chrono::seconds(3));

    // Small buffer before stopping the scheduler
    //std::this_thread::sleep_for(std::chrono::milliseconds(100));
    scheduler.stopSchedulerThread();

    ASSERT_EQ(functionCallCounter, 2);  // The function should have been called exactly 2 times
}



TEST(SchedulerTest, LongerIntervalJobTest) {
    Scheduler scheduler;
    int shortIntervalCounter = 0;
    int longIntervalCounter = 0;

    Job& shortJob = scheduler.every(1, "seconds");
    shortJob.setJobFunc([&]() {
        shortIntervalCounter++;
    });

    Job& longJob = scheduler.every(3, "seconds");
    longJob.setJobFunc([&]() {
        longIntervalCounter++;
    });

    scheduler.runPending();
    std::this_thread::sleep_for(std::chrono::seconds(7));

    // Small delay before stopping the scheduler
    //std::this_thread::sleep_for(std::chrono::milliseconds(100));
    scheduler.stopSchedulerThread();

    ASSERT_EQ(shortIntervalCounter, 6);
    ASSERT_EQ(longIntervalCounter, 2);  // Should run exactly 2 times, not 3
}



TEST(SchedulerTest, JobCancellationTest) {
    Scheduler scheduler;
    int jobCounter = 0;

    Job& myJob = scheduler.every(1, "seconds");
    myJob.setJobFunc([&]() {
        jobCounter++;
    });

    scheduler.runPending();
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));

    // Add buffer before stopping to prevent over-execution
    //std::this_thread::sleep_for(std::chrono::milliseconds(50));
    scheduler.stopSchedulerThread();

    ASSERT_EQ(jobCounter, 1);  // Job should run only once
}




    TEST(SchedulerTest, SchedulerWithoutJobsTest) {
    Scheduler scheduler;

    // Start the scheduler without adding any jobs
    scheduler.runPending();

    // Wait for 2 seconds
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Stop the scheduler
    scheduler.stopSchedulerThread();

    // If the scheduler handles this gracefully, it should not crash or hang.
    SUCCEED();  // If no crashes or hangs occur, the test passes.
}


TEST(SchedulerTest, HighFrequencyJobTest) {
    Scheduler scheduler;
    std::atomic<int> jobCounter{0};

    Job& myJob = scheduler.every(100, "milliseconds");  // Run every 100 milliseconds
    myJob.setJobFunc([&]() {
        jobCounter.fetch_add(1, std::memory_order_relaxed);
    });

    scheduler.runPending();
    
    std::this_thread::sleep_for(std::chrono::milliseconds(1200));  // Run for 1.2 seconds

    scheduler.stopSchedulerThread();

    int finalCount = jobCounter.load(std::memory_order_relaxed);

    // Job should run about 10-12 times in 1.2 seconds
    ASSERT_GE(finalCount, 10);
    ASSERT_LE(finalCount, 12);
}




TEST(SchedulerTest, DelayedJobExecutionTest) {
    Scheduler scheduler;
    bool jobExecuted = false;

    Job& myJob = scheduler.every(2, "seconds");
    myJob.setJobFunc([&]() {
        jobExecuted = true;
    });

    scheduler.runPending();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // The job should not have run yet
    ASSERT_FALSE(jobExecuted);

    // Wait for another second, allowing the job to execute
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));  // Add buffer

    scheduler.stopSchedulerThread();

    ASSERT_TRUE(jobExecuted);  // The job should now have run
}


TEST(SchedulerTest, EvenLongerIntervalJobTest) {
    Scheduler scheduler;
    int shortIntervalCounter = 0;
    int longIntervalCounter = 0;

    Job& shortJob = scheduler.every(1, "minutes");
    shortJob.setJobFunc([&]() {
        shortIntervalCounter++;
    });

    Job& longJob = scheduler.every(3, "minutes");
    longJob.setJobFunc([&]() {
        longIntervalCounter++;
    });

    scheduler.runPending();
    std::this_thread::sleep_for(std::chrono::minutes(7));

    // Small delay before stopping the scheduler
    //std::this_thread::sleep_for(std::chrono::milliseconds(100));
    scheduler.stopSchedulerThread();

    ASSERT_EQ(shortIntervalCounter, 6);
    ASSERT_EQ(longIntervalCounter, 2);  // Should run exactly 2 times, not 3
}
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
