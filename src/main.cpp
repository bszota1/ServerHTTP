#include "listener.h"
#include <boost/asio/signal_set.hpp>
#include <iostream>
#include <vector>
#include <thread>

namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

int main(int argc, char* argv[]) {
    try {
        auto const address = net::ip::make_address("0.0.0.0");
        unsigned short const port = 8080;
        auto const threads = std::max(1u, std::thread::hardware_concurrency());

        std::cout << "--- HTTP Server Intiialization ---" << std::endl;
        std::cout << "Adress: " << address << ":" << port << std::endl;
        std::cout << "Operating threads: " << threads << std::endl;

        net::io_context ioc{static_cast<int>(threads)};
        std::make_shared<Listener>(ioc, tcp::endpoint{address, port})->run();

        net::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait(
            [&](beast::error_code const&, int) {
                ioc.stop();
            });

        std::vector<std::thread> v;
        v.reserve(threads - 1);

        for(auto i = threads - 1; i > 0; --i)
            v.emplace_back(
            [&ioc] {
                ioc.run();
            });

        ioc.run();

        for(auto& t : v)
            t.join();

    } catch (const std::exception& e) {
        std::cerr << "Critical Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Server terminated." << std::endl;
    return EXIT_SUCCESS;
}