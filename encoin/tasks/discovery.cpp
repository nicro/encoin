#include "discovery.h"
#include <iostream>

namespace encoin {

discovery_task::discovery_task()
{
}

void discovery_task::exec()
{
    std::cout << "task execution" << std::endl;
}

}
