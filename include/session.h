
#ifndef SESSION_H
#define SESSION_H

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <memory>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
    tcp::socket socket_;
    beast::flat_buffer buffer_;
    http::request<http::string_body> req_;
    public:
    explicit Session(tcp::socket socket);
    void run();
private:
    void do_read();
    void on_read(beast::error_code ec, std::size_t bytes_transferred);
    void do_write(http::response<http::string_body> res);
    void on_write(bool close, beast::error_code,std::size_t bytes_transferred);

};



#endif //SESSION_H
