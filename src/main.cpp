#include <blockchain.h>

using namespace Encoin;

int main()
{
    Transaction tx1 = { "user1", "user2", 0.61 };
    Transaction tx2 = { "user2", "user1", 0.3 };
    Transaction tx3 = { "user1", "amazon", 0.134 };
    Transaction tx4 = { "user2", "starbucks", 0.15 };

    Block b1(1);
    b1.add(tx1);
    b1.add(tx2);

    Block b2(2);
    b2.add(tx3);
    b2.add(tx4);

    Blockchain chain;
    chain.add(b1);
    chain.add(b2);
    chain.print();

    return 0;
}
