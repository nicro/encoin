#include <iostream>
#include <wallet.h>
#include <blockchain.h>
#include <block.h>
#include <thread>
#include <miner.h>
#include <cxxopts.hpp>
#include <base16.h>
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
        ("m,mode", "Mode type", cxxopts::value<std::string>())
        ("c,cmd", "Command type", cxxopts::value<std::string>())
        ("o,opt", "Option type", cxxopts::value<std::string>())
        ("v,value", "Value type", cxxopts::value<std::string>())
        ("t,to", "Destination type", cxxopts::value<std::string>())
        ("a,amount", "Amount type", cxxopts::value<amount_t>())
        ("h,help", "Print usage");

    parser.parse_positional({"mode", "cmd", "opt", "value"});
    cmdopts = parser.parse(argc, argv);

    if (cmdopts.count("help"))
    {
        std::cout << parser.help() << std::endl;
        exit(0);
    }

    std::string mode  = getopt<std::string>("mode");

    if (mode == "blockchain")
    {
        std::string cmd   = getopt<std::string>("cmd");
        if (cmd == "print")
        {
            blockchain().print();
            exit(0);
        }
        else if (cmd == "generate_random")
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
        else undefopt(cmd);
    }
    else if (mode == "mine")
    {
        blockchain chain;
        if (chain.is_empty())
        {
            std::cout << "blockchain is empty, please generate some" << std::endl;
            std::exit(-1);
        }
        wallet wallet;
        miner miner1{chain, wallet.get_active_address()};
        std::cout << "Mining started" << std::endl;

        miner1.add_transaction(transaction::create_random());
        miner1.add_transaction(transaction::create_random());
        miner1.add_transaction(transaction::create_random());
        miner1.add_transaction(transaction::create_random());
        miner1.add_transaction(transaction::create_random());
        std::cout << "5 new transactions added" << std::endl;

        miner1.start();


        exit(0);
    }
    else if (mode == "wallet")
    {
        std::string cmd   = getopt<std::string>("cmd");
        if (cmd == "address")
        {
            wallet wallet;
            auto &&address = wallet.get_active_address();
            std::cout << "your current address is " << address << std::endl;
            exit(0);
        }
        else if (cmd == "balance")
        {
            std::string opt   = getopt<std::string>("opt");
            blockchain chain;
            std::cout << "current balance of " << opt.substr(0, 16) << "... address is "
                      << chain.get_balance(opt) << " encoins!" << std::endl;
            exit(0);
        }
        else if (cmd == "send")
        {
            blockchain chain;
            wallet wallet;
            std::string dest = getopt<std::string>("to");
            amount_t amount = getopt<amount_t>("amount");

            block block1{wallet.send(dest, amount)};
            chain.push(block1);
            std::cout << amount << " sent to " << dest << std::endl;

            exit(0);
        }
        else undefopt(cmd);
    }
    else if (mode == "settings")
    {
        std::string cmd   = getopt<std::string>("cmd");
        std::string opt   = getopt<std::string>("opt");
        settings settings;

        if (cmd == "set")
        {
            std::string value = getopt<std::string>("value");

            if (opt == "reward_address")
            {
                settings.set_reward_address(value);
                std::cout << "reward address updated to " << value << std::endl;
                exit(0);
            }
            else if (opt == "main_net")
            {
                settings.set_main_net(value == "true");
                std::cout << "mode set to " << (value == "true" ? "main net" : "dev net") << std::endl;
                exit(0);
            }
            exit(0);
        }
        else if (cmd == "get")
        {
            if (opt == "reward_address")
            {
                std::cout << "reward address: " << settings.reward_address() << std::endl;
                exit(0);
            }
            else if (opt == "main_net")
            {
                std::cout << "main_net: " << settings.main_net() << std::endl;
                exit(0);
            }
            else undefopt(opt);
        }
    }
    else undefopt(mode);
    return 0;
}
