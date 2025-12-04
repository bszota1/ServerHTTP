#ifndef LISTENER_H
#define LISTENER_H

#include <boost/beast/core.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <memory>
#include <iostream>

namespace beast = boost::beast;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;


class Listener : public std::enable_shared_from_this<Listener>{
    net::io_context& ioc_;
    tcp::acceptor acceptor_;

public:
    Listener(net::io_context& ioc, const tcp::endpoint& endpoint);
    void run();
private:
    void do_accept();
    void on_accept(beast::error_code ec, tcp::socket socket);
};


#endif //LISTENER_H
