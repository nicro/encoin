#include "transaction.h"

namespace Encoin {

Transaction::Transaction(std::string from, std::string to, double value)
    : _sender(from), _receiver(to), _amount(value)
{
}

double Transaction::amount() const
{
    return _amount;
}

std::string Transaction::sender() const
{
    return _sender;
}

std::string Transaction::receiver() const
{
    return _receiver;
}

}
