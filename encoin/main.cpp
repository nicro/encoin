#include <iostream>
#include <wallet.h>
#include <blockchain.h>
#include <block.h>
#include <thread>
#include <miner.h>
#include <cxxopts.hpp>
#include <crypto/base16.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <settings.h>

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

void undefopt(const std::string &opt) 
{
    std::cerr << opt << " is an undefined parameter" << std::endl;
    exit(-1);
}

int main(int argc, char **argv)
{
    cxxopts::Options parser("encoin", "A simple cryptocurrency");
    parser.add_options()
        ("c,opt", "Command type", cxxopts::value<std::string>())
        ("v,value", "Value type", cxxopts::value<std::string>())
        ("t,to", "Destination type", cxxopts::value<std::string>())
        ("a,amount", "Amount type", cxxopts::value<amount_t>())
        ("h,help", "Print usage");

    parser.parse_positional({"opt", "value"});
    cmdopts = parser.parse(argc, argv);

    if (cmdopts.count("help"))
    {
        std::cout << parser.help() << std::endl;
        exit(0);
    }

    std::string opt  = getopt<std::string>("opt");

    if      (opt == "print-blockchain") // full node
    {
        blockchain().print();
        exit(0);
    }
    else if (opt == "fill-blockchain-random") // local
    {
        blockchain chain;
        chain.remove_all();

        block b1 = {
            transaction::create_random(),
            transaction::create_random(),
            transaction::create_random()
        };

        wallet wallet1;
        const auto to = wallet1.create_new_address();

        block b2 = {
            transaction::create(wallet1.create_new_address(), to, 220),
            transaction::create(wallet1.create_new_address(), to, 54)
        };
        block b3;

        chain.push(b1);
        chain.push(b2);
        chain.push(b3);

        std::cout << "tables created" << std::endl;
        exit(0);
    }
    else if (opt == "mine") // miner node
    {
        blockchain chain;
        chain.push(transaction::create_random());
        chain.push(transaction::create_random());
        chain.push(transaction::create_random());
        chain.push(transaction::create_random());
        chain.push(transaction::create_random());
        std::cout << "5 new transactions added" << std::endl;

        if (chain.is_empty())
        {
            std::cout << "blockchain is empty, please generate some" << std::endl;
            std::exit(-1);
        }

        // ToDo: miner should also broadcast new blocks
        miner miner1{chain, wallet().get_active_address()};
        std::cout << "Mining started" << std::endl;
        miner1.start();
        exit(0);
    }
    else if (opt == "get-wallet-address") // full node
    {
        wallet wallet;
        auto &&address = wallet.get_active_address();
        std::cout << "your current address is " << address << std::endl;
        exit(0);
    }
    else if (opt == "wallet-balance") // full node
    {
        std::string value = getopt<std::string>("value");
        blockchain chain;
        std::cout << "current balance of " << value.substr(0, 16) << "... address is "
                  << chain.get_balance(value) << " encoins!" << std::endl;
        exit(0);
    }
    else if (opt == "wallet-send") // full node
    {
        blockchain chain;
        wallet wallet;
        std::string dest = getopt<std::string>("to");
        amount_t amount = getopt<amount_t>("amount");

        // ToDo: broadcast to network
        chain.push(wallet.send(dest, amount));
        std::cout << amount << " sent to " << dest << std::endl;
        exit(0);
    }
    else if (opt == "set") // local
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
                exit(0);
            }
            else if (sub1 == "main_net")
            {
                settings.set_main_net(sub2 == "true");
                std::cout << "mode set to " << (sub2 == "true" ? "main net" : "dev net") << std::endl;
                exit(0);
            }
        }
        exit(0);
    }
    else if (opt == "get") // local
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
    }
    else undefopt(opt);
    return 0;
}
