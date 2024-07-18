#include <iostream>
#include <boost/asio.hpp>
#include "websocket_server.h"

int main() {
    try {
        // Create an io_context for the server
        net::io_context ioc;

        // Define the endpoint (port 8080) the server will listen on
        tcp::endpoint endpoint(tcp::v4(), 8080);

        // Create and run the WebSocket server
        WebsocketServer server(ioc, endpoint);
        server.run();

        // Run the io_context to handle asynchronous events
        ioc.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
