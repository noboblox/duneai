#pragma once
#include <boost/asio/ip/tcp.hpp>

class Connection
{
public:
    explicit Connection(boost::asio::ip::tcp::socket&& aSocket)
        : socket(std::move(aSocket))
    {
    }

private:
    boost::asio::ip::tcp::socket socket;
};
