#include <iostream>
#include "server.h"
#include <boost/asio/ip/tcp.hpp>

Server::Server()
: context(),
  nextConnection(context)
{
}

void Server::run()
{
	try
	{
		boost::asio::ip::tcp::endpoint localhost(boost::asio::ip::address_v4::loopback(), DEFAULT_PORT);
		boost::asio::ip::tcp::acceptor ac(context, localhost);
		ac.async_accept(nextConnection, localhost);
		context.run();
	}
	catch (std::exception& e)
	{
		std::cout << "exception: " << e.what() << std::endl;
	}
}

void Server::OnConnect(boost::system::error_code ec)
{
	if (ec)
	{
		std::cout << "Accept failed: " << ec.what() << std::endl;
		return;
	}

	connections.push_back(Connection(std::move(nextConnection)));
}
