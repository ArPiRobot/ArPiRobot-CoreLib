#pragma once

#include <functional>
#include <chrono>
#include <memory>
#include <atomic>
#include <mutex>
#include <future>
#include <map>
#include <ctpl_stl.h>

/**
 * Based loosely on Bosma's Scheduler library (https://github.com/Bosma/Scheduler)
 */

namespace arpirobot{

    using sched_clk = std::chrono::system_clock;

    /**
     * Class to represent a task for the scheduler. Holds the function to be run for this task
     * 
     * Also acts as a common base class for all tasks, including repeated ones.
     */
    class Task{
    public:
        Task(const std::function<void()> &&f);

        virtual sched_clk::time_point nextRunTime();
    
        virtual bool doesRepeat();

        std::function<void()> targetFunction;
    };

    /**
     * Task that runs repeatedly at a fixed rate
     */
    class RepeatedTask : public Task{
    public:
        RepeatedTask(const std::function<void()> &&f, sched_clk::duration rate);

        sched_clk::time_point nextRunTime();

        bool doesRepeat();
    
        sched_clk::duration rate;
    };

    /**
     * Interruptable sleeper (thread safe)
     */
    class SchedSleeper{
    public:
        SchedSleeper();
        
        void sleep_for(sched_clk::duration duration);

        void sleep_until(sched_clk::time_point time);

        void sleep();

        void interrupt();
    
    private:
        bool interrupted;
        std::mutex m;
        std::condition_variable cv;
    };

    /**
     * Scheduler to run Tasks. Allows removing tasks
     */
    class Scheduler{
    public:
        Scheduler(unsigned int maxThreads = 4);
        ~Scheduler();

        // Add a task to be run once
        std::shared_ptr<Task> addTask(const std::function<void()> &&targetFunc, sched_clk::time_point::duration delay);

        // Add a task to be run periodically (at given rate)
        std::shared_ptr<Task> addRepeatedTask(const std::function<void()> &&targetFunc, sched_clk::time_point::duration delay, 
            sched_clk::time_point::duration rate);
        
        // Remove a task (only really useful for repeated tasks)
        void removeTask(std::shared_ptr<Task> task);
    
    private:
        void serviceTasks();

        std::atomic<bool> done;
        SchedSleeper sleeper;
        std::multimap<sched_clk::time_point, std::shared_ptr<Task>> tasks;
        std::mutex lock;
        ctpl::thread_pool threads;
    };

}