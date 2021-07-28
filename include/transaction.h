#ifndef TRANSCATION_H
#define TRANSCATION_H

#include <string>

namespace Encoin {

class Transaction
{
public:
    Transaction(std::string from, std::string to, double value);

    double amount() const;
    std::string sender() const;
    std::string receiver() const;

protected:
    double _amount;
    std::string _sender;
    std::string _receiver;
};

}


#endif // TRANSCATION_H
