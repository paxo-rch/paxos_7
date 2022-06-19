#ifndef TASKS_HPP
#define TASKS_HPP

#include "vector"

#define TASKS_LOOP 1
#define TASKS_AUTODELETE 2
#define TASKS_TIME 3

#define DELETE_TASK 1

class Task
{
public:
    Task(unsigned long time, byte (*function)(void), byte type);
    ~Task();
    byte execute();

private:
    byte (*function)(void) = nullptr;
    byte type;
    unsigned long time = 1000;
    unsigned long time_counter = 0;
};

Task::Task(unsigned long time, byte (*function)(void), byte type)
{
    this->time = time;
    this->type = type;
    this->function = function;
}

Task::~Task()
{
}

byte Task::execute()
{
    if (time_counter + time < millis())
    {
        time_counter = millis();
        switch (type)
        {
        case TASKS_LOOP:
            return function();
            break;
        case TASKS_AUTODELETE:
            if (function() == 0)
                return DELETE_TASK;
            break;
        case TASKS_TIME:
            return DELETE_TASK;
            break;
        }
    }
}

//--------------------------------------------------------------

class TaskManager
{
public:
    void addTask(Task *task);
    void executeAll();

private:
    std::vector<Task *> tasks_vector;
};

void TaskManager::addTask(Task *task)
{
    tasks_vector.push_back(task);
}

void TaskManager::executeAll()
{
    for (int i = 0; i < tasks_vector.size(); i++)
    {
        if (tasks_vector[i] != nullptr)
        {
            if(tasks_vector[i]->execute()==DELETE_TASK)
            {
                delete tasks_vector[i];
            }
        }
    }
}

TaskManager taskmanager;

#endif // TASKS_HPP