
#include "../../include/listener.h"
#include "../../include/session.h"
#include <iostream>
#include <boost/asio/strand.hpp>
Listener::Listener(net::io_context& ioc,const tcp::endpoint& endpoint):
    ioc_(ioc),
    acceptor_(net::make_strand(ioc)){

    beast::error_code ec;
    acceptor_.open(endpoint.protocol(), ec);
    if (ec) {
        std::cerr<<"Open error: "<<ec.message()<<std::endl;
        return;
    }

    acceptor_.set_option(net::socket_base::reuse_address(true),ec);
    acceptor_.bind(endpoint, ec);
    if (ec) {
        std::cerr<<"Bind error: "<<ec.message()<<std::endl;
        return;
    }
    acceptor_.listen(net::socket_base::max_listen_connections, ec);
    if (ec) {
        std::cerr << "Listen error: " << ec.message() << std::endl;
        return;
    }
}
// Public entry point to start the accept loop.
void Listener::run() {
    do_accept();
}
// Initiates the asynchronous accept operation.
void Listener::do_accept() {
    acceptor_.async_accept(
        net::make_strand(ioc_),
        beast::bind_front_handler(
            &Listener::on_accept,
            shared_from_this()));
}
// Completion handler called when a new client connects.
void Listener::on_accept(beast::error_code ec, tcp::socket socket) {
    if(ec) {
        std::cerr << "Accept error: " << ec.message() << "\n";
    } else {
        std::make_shared<Session>(std::move(socket))->run();
    }
    do_accept();
}