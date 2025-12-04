
#ifndef ROUTER_H
#define ROUTER_H

#include <boost/beast/http.hpp>
#include <string>

namespace beast = boost::beast;
namespace http = beast::http;

class Router {
    public:
    static http::response<http::string_body> handle_request(http::request<http::string_body> req);
};




#endif
