
#include "../../include/session.h"
#include "../../include/router.h"
#include <iostream>

// Constructor. Takes ownership of the client socket by moving it
// into the session object to manage the connection's lifecycle.
Session::Session(tcp::socket socket):
    socket_(std::move(socket)) {
}
// Entry point for the session. Starts the asynchronous operation loop
// by initiating the first read operation.
void Session::run() {
    do_read();
}
// Initiates an asynchronous read from the socket.
// It clears the request parser and uses the flat_buffer to store incoming bytes.
// 'shared_from_this()' is used to keep the Session object alive during the async operation.
void Session::do_read() {
    req_ = {};
    http::async_read(socket_, buffer_, req_,
        beast::bind_front_handler(
            &Session::on_read,
            shared_from_this()));
}
// Completion handler for the async_read operation.
// It handles errors, passes the parsed request to the Router,
// and initiates the writing of the generated response.
void Session::on_read(beast::error_code ec, std::size_t bytes_transferred) {
    if(ec == http::error::end_of_stream) {
        socket_.shutdown(tcp::socket::shutdown_send, ec);
        return;
    }
    if(ec) {
        std::cerr << "read error: " << ec.message() << "\n";
        return;
    }

    auto res = Router::handle_request(std::move(req_));
    do_write(std::move(res));
}
// Completion handler for the async_write operation.
// It checks if the connection needs to be closed (based on the response headers)
// or if we should loop back to 'do_read' to handle the next request (Keep-Alive).
void Session::on_write(bool close, beast::error_code ec, std::size_t bytes_transferred) {
    if(ec) {
        std::cerr << "write error: " << ec.message() << "\n";
        return;
    }
    if(close) {
        socket_.shutdown(tcp::socket::shutdown_send, ec);
        return;
    }
    do_read();
}
// Initiates an asynchronous write of the HTTP response.
// Crucially, it moves the response object to the heap (via shared_ptr)
// to ensure it stays alive until the async_write operation completes.
void Session::do_write(http::response<http::string_body> res) {
    bool close = res.need_eof();
    auto sp = std::make_shared<http::response<http::string_body>>(std::move(res));
    http::async_write(socket_, *sp,
        [self = shared_from_this(), sp, close](beast::error_code ec, std::size_t bytes) {
            self->on_write(close, ec, bytes);
        });
}

