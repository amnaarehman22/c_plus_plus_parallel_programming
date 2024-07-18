#include "main_thread.h"
#include "websocket_server.h"
#include <iostream>
#include <boost/asio.hpp>
#include <thread>

namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

int main() {
    try {
        // Initialize Boost Asio io_context
        net::io_context io_context;

        // Create a separate thread for the main sensor data processing
        std::thread mainThreadObj(mainThread);

        // Define endpoint for websocket server
        tcp::endpoint endpoint(tcp::v4(), 8080); // Use port 8080 for websocket

        // Create and start the websocket server
        WebsocketServer websocketServer(io_context, endpoint);
        websocketServer.run();

        // Join the main thread to the main process
        if (mainThreadObj.joinable()) {
            mainThreadObj.join();
        }

        // Run the io_context to handle asynchronous operations
        io_context.run();
    } catch(const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}
