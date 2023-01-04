#ifndef SERVER_H_
#define SERVER_H_

#include <boost/asio/io_context.hpp>
#include "connection.h"

class Server
{
public:
    static constexpr int DEFAULT_PORT = 19111;

    explicit Server();
    void run();

private:
    void OnConnect(boost::system::error_code ec);
private:
    boost::asio::io_context context;
    boost::asio::ip::tcp::socket nextConnection;

    std::vector<Connection> connections;
};

#endif
