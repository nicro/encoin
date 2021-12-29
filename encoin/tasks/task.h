#ifndef TASK_H
#define TASK_H

namespace encoin {

class task {
public:
    task() = default;

    virtual void exec() = 0;
    virtual void abort() {}
};

}

#endif // TASK_H
