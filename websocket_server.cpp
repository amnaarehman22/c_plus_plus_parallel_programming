#include "websocket_server.h"
#include <iostream>
#include <thread>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio.hpp>

double generateRandomCoordinate() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<double> dis(-10.0, 10.0);
    return dis(gen);
}

WebsocketServer::WebsocketServer(net::io_context& ioc, tcp::endpoint endpoint)
    : ioc_(ioc), acceptor_(ioc, endpoint) {}

void WebsocketServer::run() {
    try {
        for (;;) {
            tcp::socket socket(ioc_);
            acceptor_.accept(socket);
            std::thread{&WebsocketServer::doSession, this, std::move(socket)}.detach();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error in WebSocket server run: " << e.what() << std::endl;
    }
}

void WebsocketServer::doSession(tcp::socket socket) {
    try {
        // Construct a WebSocket stream by moving the socket
        websocket::stream<tcp::socket> ws(std::move(socket));

        // Perform the WebSocket handshake
        ws.set_option(websocket::stream_base::decorator(
            [](websocket::response_type& res) {
                res.set(http::field::access_control_allow_origin, "*");
                res.set(http::field::access_control_allow_methods, "GET, POST");
                res.set(http::field::access_control_allow_headers, "content-type");
            }));
        ws.accept();

        std::cout << "WebSocket handshake completed with " << ws.next_layer().remote_endpoint() << std::endl;

        // Continuously send sensor data until the client closes the connection
        for (;;) {
            try {
                // Mock sensor data generation
                std::string sensorData = generateSensorData();

                // Send JSON formatted sensor data
                ws.write(net::buffer(sensorData));

                // Simulate delay between sensor data updates
                std::this_thread::sleep_for(std::chrono::seconds(1));
            } catch (const boost::system::system_error& e) {
                if (e.code() == websocket::error::closed) {
                    // Connection was closed gracefully by the client
                    std::cout << "WebSocket connection closed by client." << std::endl;
                    break; // Exit the loop on graceful closure
                } else {
                    // Other Boost system errors
                    std::cerr << "Boost error in WebSocket session: " << e.what() << std::endl;
                    break; // Exit the loop on other errors
                }
            } catch (const std::exception& e) {
                // General errors
                std::cerr << "Error in WebSocket session: " << e.what() << std::endl;
                break; // Exit the loop on other errors
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error in WebSocket session setup: " << e.what() << std::endl;
    }
}

std::string generateSensorData() {
    boost::property_tree::ptree root;
    root.put("sensor_id", 1);

    boost::property_tree::ptree coordinates;
    coordinates.put("x", generateRandomCoordinate());
    coordinates.put("y", generateRandomCoordinate());
    coordinates.put("z", generateRandomCoordinate());

    root.add_child("coordinates", coordinates);

    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S");
    root.put("timestamp", ss.str());

    std::stringstream ss_json;
    boost::property_tree::write_json(ss_json, root);
    return ss_json.str();
}
