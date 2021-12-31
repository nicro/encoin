#include <iostream>
#include <wallet.h>
#include <blockchain.h>
#include <block.h>
#include <miner.h>
#include <cxxopts.hpp>
#include <crypto/base16.h>
#include <settings.h>
#include <net/node.h>

//#include <thread>
//#include <chrono>

using namespace encoin;

inline cxxopts::ParseResult cmdopts;

template <class T >
T getopt (const std::string &opt)
{
    if (cmdopts.count(opt))
    {
        return cmdopts[opt].as<T>();
    }
    std::cerr << opt << " was not provided" << std::endl;
    exit(-1);
}

int main(int argc, char **argv)
{
    cxxopts::Options parser("encoin", "A simple cryptocurrency");
    parser.add_options()
        ("c,opt",    "Command",     cxxopts::value<std::string>())
        ("v,value",  "Value",       cxxopts::value<std::string>())
        ("t,to",     "Destination", cxxopts::value<std::string>())
        ("a,amount", "Amount",      cxxopts::value<amount_t>())
        ("h,help",   "Print usage");

    parser.parse_positional({"opt", "value"});
    cmdopts = parser.parse(argc, argv);

    if (cmdopts.count("help"))
    {
        std::cout << parser.help() << std::endl;
        exit(0);
    }

    std::string opt  = getopt<std::string>("opt");

    if (opt == "get") // local
    {
        settings settings;
        std::string value = getopt<std::string>("value");
        if (value == "reward_address")
        {
            std::cout << "reward address: " << settings.reward_address() << std::endl;
            exit(0);
        }
        else if (value == "main_net")
        {
            std::cout << "is main net: " << settings.main_net() << std::endl;
            exit(0);
        }
        else
        {
            std::cout << "error: setting not found" << std::endl;
            return -1;
        }
    }
    if (opt == "set") // local
    {
        settings settings;
        std::string value = getopt<std::string>("value");

        if (auto del = value.find('='); del != std::string::npos)
        {
            std::string sub1 = value.substr(0, del);
            std::string sub2 = value.substr(del + 1);

            if (sub1 == "reward_address")
            {
                settings.set_reward_address(sub2);
                std::cout << "reward address updated to " << sub2 << std::endl;
                return 0;
            }
            else if (sub1 == "main_net")
            {
                settings.set_main_net(sub2 == "true");
                std::cout << "mode set to " << (sub2 == "true" ? "main net" : "dev net") << std::endl;
                return 0;
            }
            else
            {
                std::cout << "error: setting not found" << std::endl;
                return -1;
            }
        }
        std::cout << "error: wrong setting format" << std::endl;
        return -1;
    }

    blockchain chain;
    node node;

    if (opt == "print-blockchain") // full node
    {
        chain.print();
        return 0;
    }
    if (opt == "print-wallet") // full node
    {
        wallet wallet;
        for (auto &addr : wallet.addresses())
        {
            // todo: flag that would print the whole address
            std::cout << "address: [" << addr.substr(0, 16) << "... ] "
                      << "balance: " << chain.get_balance(addr) << std::endl;
        }
        return 0;
    }
    if (opt == "mine") // miner node
    {
        miner miner1{chain, wallet().get_active_address()};
        std::cout << "Mining started" << std::endl;
        miner1.start();
        return 0;
    }
    if (opt == "wallet-send") // full node
    {
        wallet wallet;
        std::string dest = getopt<std::string>("to");
        amount_t amount = getopt<amount_t>("amount");

        chain.push(wallet.send(dest, amount));
        std::cout << amount << " sent to " << dest << std::endl;
        return 0;
    }

    std::cerr << opt << " is an undefined parameter" << std::endl;
    return -1;
}
