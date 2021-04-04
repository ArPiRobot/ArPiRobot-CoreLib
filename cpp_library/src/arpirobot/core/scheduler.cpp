#include <arpirobot/core/scheduler.hpp>
#include <arpirobot/core/log/Logger.hpp>
#include <algorithm>

using namespace arpirobot;


////////////////////////////////////////////////////////////////////////////////
/// Task
////////////////////////////////////////////////////////////////////////////////
Task::Task(const std::function<void()> &&f) : targetFunction(f){

}

sched_clk::time_point Task::nextRunTime(){
    return sched_clk::time_point(sched_clk::duration(0)); // Dummy time point, not used
}

bool Task::doesRepeat(){
    return false;
}

////////////////////////////////////////////////////////////////////////////////
/// RepeatedTask
////////////////////////////////////////////////////////////////////////////////
RepeatedTask::RepeatedTask(const std::function<void()> &&f, sched_clk::duration rate) : Task(std::move(f)), rate(rate){

}

sched_clk::time_point RepeatedTask::nextRunTime(){
    return sched_clk::now() + rate;
}

bool RepeatedTask::doesRepeat(){
    return true;
}


////////////////////////////////////////////////////////////////////////////////
/// SchedSleeper
////////////////////////////////////////////////////////////////////////////////
SchedSleeper::SchedSleeper() : interrupted(false){

}
        
void SchedSleeper::sleep_for(sched_clk::duration duration){
    std::unique_lock<std::mutex> l(m);
    cv.wait_for(l, duration, [this]{return interrupted;});
    interrupted = false;
}

void SchedSleeper::sleep_until(sched_clk::time_point time){
    std::unique_lock<std::mutex> l(m);
    cv.wait_until(l, time, [this]{return interrupted;});
    interrupted = false;
}

void SchedSleeper::sleep(){
    std::unique_lock<std::mutex> l(m);
    cv.wait(l, [this]{return interrupted;});
    interrupted = false;
}

void SchedSleeper::interrupt(){
    std::lock_guard<std::mutex> l(m);
    interrupted = true;
    cv.notify_one();
}


////////////////////////////////////////////////////////////////////////////////
/// Scheduler
////////////////////////////////////////////////////////////////////////////////
Scheduler::Scheduler(unsigned int maxThreads) : done(false), threads(maxThreads + 1){
    // Use an extra thread in this pool to run the scheduler itself (so scheduler is not blocking)
    threads.push([this](int) {
        while (!done) {
            if (tasks.empty()) {
                sleeper.sleep();
            } else {
                auto time_of_first_task = (*tasks.begin()).first;
                sleeper.sleep_until(time_of_first_task);
            }
            serviceTasks();
        }
    });
}

Scheduler::~Scheduler(){
    done = true;
    sleeper.interrupt();
}

std::shared_ptr<Task> Scheduler::addTask(const std::function<void()> &&targetFunc, 
        sched_clk::time_point::duration delay){
    std::shared_ptr<Task> task = std::make_shared<Task>(std::move(targetFunc));
    sched_clk::time_point time = sched_clk::now() + delay;

    {
        std::lock_guard<std::mutex> l(lock);
        tasks.emplace(time, task);
        sleeper.interrupt();
    }
    
    return task;
}

std::shared_ptr<Task> Scheduler::addRepeatedTask(const std::function<void()> &&targetFunc, 
        sched_clk::time_point::duration delay, sched_clk::time_point::duration rate){
    std::shared_ptr<Task> task = std::make_shared<RepeatedTask>(std::move(targetFunc), rate);
    sched_clk::time_point time = sched_clk::now() + delay;

    {
        std::lock_guard<std::mutex> l(lock);
        tasks.emplace(time, task);
        sleeper.interrupt();
    }
    
    return task;
}

void Scheduler::removeTask(std::shared_ptr<Task> task){
    std::lock_guard<std::mutex> l(lock);
    for(auto i = tasks.begin(); i != tasks.end(); ++i){
        if(i->second.get() == task.get()){
            tasks.erase(i);
            break;
        }
    }
}

void Scheduler::serviceTasks(){
    std::lock_guard<std::mutex> l(lock);

    const auto lastTaskToRun = tasks.upper_bound(sched_clk::now());

    // If there are any tasks to run
    if(lastTaskToRun != tasks.begin()){
        // Keep a list of tasks that will need to run again (repeated tasks)
        decltype(tasks) repeatedTasks;

        // Run each task that needs to run
        for(auto i = tasks.begin(); i != lastTaskToRun; ++i){
            auto &task = i->second;
            // Start the task (in the threadpool)
            threads.push([task](int) {
                task->targetFunction();
            });
            // Calculate the next run time for the task
            if(task->doesRepeat()){
                repeatedTasks.emplace(task->nextRunTime(), std::move(task));
            }
        }

        // Remove completed tasks
        tasks.erase(tasks.begin(), lastTaskToRun);

        // Re-add tasks that are repeating
        for(auto &task : repeatedTasks){
            tasks.emplace(task.first, std::move(task.second));
        }
    }
}