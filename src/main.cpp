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
    auto &&address = wallet.get_active_address();
    std::cout << address << std::endl;

    block block1;
    block1.add(wallet.send("receiver1", 700));
    block1.add(wallet.send("receiver2", 120));
    chain.push(block1);

    std::cout << chain.get_balance(address) << std::endl;

    return 0;
}
