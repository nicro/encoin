#include <iostream>
#include <wallet.h>
#include <blockchain.h>
#include <block.h>
#include <miner.h>

using namespace encoin;

int main()
{
    blockchain chain;
    wallet wallet;
    auto reward_addr = wallet.addresses().front();

    block block1;
    block1.add(transaction::create_test("from", "to", reward_addr, 105, 100));
    block1.add(transaction::create_test("from", "to", reward_addr, 105, 100));
    block1.add(transaction::create_test("from", "to", reward_addr, 105, 100));
    chain.push(block1);

    miner miner{chain, reward_addr};
    miner.start();

    return 0;
}
