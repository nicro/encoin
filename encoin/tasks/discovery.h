#ifndef DISCOVERY_H
#define DISCOVERY_H

#include "task.h"

namespace encoin {

class discovery_task : public task {
public:
    discovery_task();

    virtual void exec() override;
};

}

#endif // DISCOVERY_H
