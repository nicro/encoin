#include <blockchain.h>

using namespace Encoin;

int main()
{
    transaction tx1 = { "user1", "user2", 0.61 };
    transaction tx2 = { "user2", "user1", 0.3 };
    transaction tx3 = { "user1", "amazon", 0.134 };
    transaction tx4 = { "user2", "starbucks", 0.15 };

    block b1;
    b1.add(tx1);
    b1.add(tx2);

    block b2;
    b2.add(tx3);
    b2.add(tx4);

    blockchain chain;
    chain.add(b1);
    chain.add(b2);

    return 0;
}
