#include <boost/program_options.hpp>
#include <iostream>
#include <net/node.h>
#include <net/message.h>
#include <settings.h>
#include <blockchain.h>
#include <wallet.h>
#include <block.h>
#include <miner.h>

using namespace encoin;
namespace po = boost::program_options;

int main(int argc, char **argv)
{
    po::options_description desc("encoin - a simple cryptocurrency");
    desc.add_options()
        ("cmd",    po::value<std::string>(), "command")
        ("value",  po::value<std::string>(), "value")
        ("to",     po::value<std::string>(), "destination")
        ("amount", po::value<amount_t>(),    "amount")
        ("help",                             "print usage");

    po::positional_options_description pos_desc;
    pos_desc.add("cmd", 1);
    pos_desc.add("value", 1);

    po::variables_map cmdopts;
    try
    {
        po::store(po::command_line_parser(argc, argv).options(desc).positional(pos_desc).run(), cmdopts);
        boost::program_options::notify(cmdopts);
    }
    catch (boost::program_options::error& e)
    {
        std::cout << "error: " << e.what() << "\n";
        return -1;
    }

    if (cmdopts.count("help") || !cmdopts.count("cmd"))
    {
        std::cout << desc << std::endl;
        exit(0);
    }

    std::string opt  = cmdopts["cmd"].as<std::string>();

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

    blockchain chain{true};
    node node{chain, "127.0.0.1", settings().port()};
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    // todo: if not empty ask node for longer chain

    if (chain.is_empty())
    {
        block_list blocks = node.dispatch<get_chain_message>();
        std::cout << blocks.size() << " new blocks fetched" << std::endl;
        chain.set_blocks(blocks);
    }

    {
        tx_list fetched_pool = node.dispatch<get_pool_message>();
        int tx_counter = 0;
        for (auto &tx : fetched_pool)
        {
            if (!chain.contains(tx))
            {
                chain.push(tx), tx_counter++;
            }
        }
        std::cout << tx_counter << " new transactions fetched" << std::endl;
    }

    if (opt == "create-random")
    {
        chain = blockchain::random_filled();
        std::cout << "chain filled with random values" << std::endl;
        return 0;
    }
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
