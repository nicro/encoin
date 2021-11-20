#include <iostream>
#include <wallet.h>
#include <blockchain.h>
#include <block.h>
#include <miner.h>
#include <cxxopts.hpp>

using namespace encoin;
using argument_parser = cxxopts::Options;

int main(int argc, char **argv)
{
    argument_parser parser("encoin", "A simple cryptocurrency");
    parser.add_options()
        ("n,node", "Type of node", cxxopts::value<std::string>())
        ("h,help", "Print usage");
    

    parser.parse_positional({"node"});
    auto cmdopts = parser.parse(argc, argv);

    if (cmdopts.count("help"))
    {
        std::cout << parser.help() << std::endl;
        exit(0);
    }

    if (cmdopts.count("node")) 
    {
        std::string node_type = cmdopts["node"].as<std::string>();
        if (node_type == "miner") 
        {
            std::cout << "miner mode selected" << std::endl;
            exit(0);
        } 
        else if (node_type == "wallet") 
        {
            wallet wallet;
            auto &&address = wallet.get_active_address();
            std::cout << "Your current address is " << address << std::endl;
            exit(0);
        }
    }

    return 0;
}
