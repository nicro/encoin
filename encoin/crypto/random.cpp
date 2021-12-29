#include "random.h"

#include <crypto/sha256.h>
#include <chrono>
#include <random>

namespace encoin {

std::string generate_random_private_key()
{
    unsigned now = std::chrono::system_clock::now().time_since_epoch().count();
    std::seed_seq seed{now};
    std::mt19937_64 eng(seed);
    std::string randString;
    for (int i = 0; i < 10; i++)
        randString += eng();

    return encoin::sha256(randString).substr(0, 32);
}

}
