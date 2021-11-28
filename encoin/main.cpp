#include <iostream>
#include <wallet.h>
#include <blockchain.h>
#include <block.h>
#include <miner.h>
#include <cxxopts.hpp>
#include <base16.h>

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
#if false
    // test data
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

    chain.print();
    return 0;
#endif
    cxxopts::Options parser("encoin", "A simple cryptocurrency");
    parser.add_options()
        ("m,mode", "Mode type", cxxopts::value<std::string>())
        ("c,cmd", "Command type", cxxopts::value<std::string>())
        ("o,opt", "Option type", cxxopts::value<std::string>())
        ("v,value", "Value type", cxxopts::value<std::string>())
        ("h,help", "Print usage");
    

    parser.parse_positional({"mode", "cmd", "opt", "value"});
    cmdopts = parser.parse(argc, argv);

    if (cmdopts.count("help"))
    {
        std::cout << parser.help() << std::endl;
        exit(0);
    }

    std::string mode_type = getopt<std::string>("mode");
    if (mode_type == "blockchain")
    {
        std::string cmd_type = getopt<std::string>("cmd");
        if (cmd_type == "print")
        {
            blockchain().print();
            exit(0);
        }
        std::cout << "mining mode selected" << std::endl;
        exit(0);
    }
    else if (mode_type == "mine")
    {
        std::cout << "mining mode selected" << std::endl;
        exit(0);
    }
    else if (mode_type == "wallet") 
    {
        std::string cmd_type = getopt<std::string>("cmd");
        if (cmd_type == "address") 
        {
            wallet wallet;
            auto &&address = wallet.get_active_address();
            std::cout << "your current address is " << address << std::endl;
            exit(0);
        }
        else if (cmd_type == "balance")
        {
                blockchain chain;
                std::string address = getopt<std::string>("opt");
                std::cout << "current balance of " << address.substr(0, 16) << "... address is "
                          << chain.get_balance(address) << " encoins!" << std::endl;
                exit(0);
        }
        else if (cmd_type == "send")
        {
            std::cout << "send cmd selected" << std::endl;
            exit(0);
        }
        else undefopt(cmd_type);
    }
    else if (mode_type == "settings")
    {
        std::string cmd_type = getopt<std::string>("cmd");
        if (cmd_type == "set")
        {
            std::string opt = getopt<std::string>("opt");
            std::string value = getopt<std::string>("value");
            std::cout << "setting " << opt << " to " << value << std::endl;
            exit(0);
        }
        else if (cmd_type == "get")
        {
            std::string opt = getopt<std::string>("opt");
            std::cout << opt << " equals " << "" << std::endl;
            exit(0);
        }
    }
    else undefopt(mode_type);
    return 0;
}
