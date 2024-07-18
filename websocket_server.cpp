#include "websocket_server.h"
#include <iostream>
#include <thread>
#include <memory>
#include <string>

WebsocketServer::WebsocketServer(net::io_context& ioc, tcp::endpoint endpoint)
    : ioc_(ioc), acceptor_(ioc, endpoint) {}

void WebsocketServer::run() {
    try {
        for (;;) {
            tcp::socket socket(ioc_);
            acceptor_.accept(socket);
            
            // Capture socket by reference to avoid copying/moving issues
            std::thread{&WebsocketServer::doSession, this, std::ref(socket)}.detach();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void WebsocketServer::doSession(tcp::socket& socket) {
    try {
        websocket::stream<tcp::socket> ws{std::move(socket)};
        ws.accept();

        for (;;) {
            beast::flat_buffer buffer;
            ws.read(buffer);
            ws.text(ws.got_text());
            ws.write(buffer.data());
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
