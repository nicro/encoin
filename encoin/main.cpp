#include <iostream>
#include <wallet.h>
#include <blockchain.h>
#include <block.h>
#include <miner.h>
#include <cxxopts.hpp>
#include <crypto/base16.h>
#include <settings.h>
#include <net/node.h>
#include <net/message.h>

using namespace encoin;

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
    cxxopts::ParseResult cmdopts = parser.parse(argc, argv);

    if (cmdopts.count("help") || !cmdopts.count("opt"))
    {
        std::cout << parser.help() << std::endl;
        exit(0);
    }

    std::string opt  = cmdopts["opt"].as<std::string>();

    if (opt == "get") // local
    {
        settings settings;
        if (!cmdopts.count("value"))
        {
            std::cout << "Please provide the name of a setting" << std::endl;
            return -1;
        }
        std::string value = cmdopts["value"].as<std::string>();
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
        else if (value == "port")
        {
            std::cout << "port: " << settings.port() << std::endl;
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
        if (!cmdopts.count("value"))
        {
            std::cout << "Please provide the name=value for a setting" << std::endl;
            return -1;
        }
        std::string value = cmdopts["value"].as<std::string>();

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
            else if (sub1 == "port")
            {
                settings.set_port(std::stoi(sub2));
                std::cout << "port set to " << sub2 << std::endl;
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
    node node{chain, "127.0.0.1", settings().port()};
    //std::this_thread::sleep_for(std::chrono::milliseconds(10));

    if (opt == "run-node")
    {
        node.run_server();
        std::cout << "listening on port " << node._port << std::endl;
        node.wait_server();
        return 0;
    }
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
        if (!cmdopts.count("to") || !cmdopts.count("amount"))
        {
            std::cout << "Either amount or target address is missing" << std::endl;
            return -1;
        }
        std::string dest = cmdopts["to"].as<std::string>();
        amount_t amount = cmdopts["amount"].as<amount_t>();

        auto tx = wallet.send(dest, amount);
        chain.push(tx);
        node.broadcast<new_tx_message>(tx);

        std::cout << amount << " sent to " << dest << std::endl;
        return 0;
    }

    std::cerr << opt << " is an undefined parameter" << std::endl;
    return -1;
}
