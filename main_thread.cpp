#include "main_thread.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <random>
#include <thread>
#include <boost/beast.hpp>
#include <boost/asio.hpp>

using tcp = boost::asio::ip::tcp;
namespace websocket = boost::beast::websocket;
namespace net = boost::asio;

// Function to generate dummy sensor data in JSON format
std::string generateDummySensorData(int sensorId, double x, double y, double z) {
    // Get current timestamp
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    // Format timestamp
    std::stringstream timestamp_ss;
    timestamp_ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S");

    // Construct JSON string
    std::stringstream json_ss;
    json_ss << "{"
            << "\"sensor_id\": " << sensorId << ","
            << "\"coordinates\": {"
            << "\"x\": " << x << ","
            << "\"y\": " << y << ","
            << "\"z\": " << z
            << "},"
            << "\"timestamp\": \"" << timestamp_ss.str() << "\""
            << "}";

    return json_ss.str();
}

// Main thread function to simulate sensor data processing
void mainThread() {
    std::cout << "Main thread started.\n";

    // Simulate generating dummy sensor data
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> disCoord(-10.0, 10.0);

    // Initialize Boost Asio io_context
    net::io_context io_context;

    // Define WebSocket endpoint and URI
    tcp::resolver resolver(io_context);
    websocket::stream<tcp::socket> ws(io_context);

    try {
        // Resolve the WebSocket endpoint
        auto const results = resolver.resolve("localhost", "8080");

        // Connect to the WebSocket server
        net::connect(ws.next_layer(), results.begin(), results.end());

        // Perform the WebSocket handshake
        ws.handshake("localhost", "/");

        while (true) {
            // Generate random sensor data
            int sensorId = 1;  // Dummy sensor ID
            double x = disCoord(gen);
            double y = disCoord(gen);
            double z = disCoord(gen);

            std::string sensorData = generateDummySensorData(sensorId, x, y, z);

            // Send sensor data via WebSocket
            ws.write(net::buffer(sensorData));

            // Simulate processing delay
            std::this_thread::sleep_for(std::chrono::seconds(1));

            // For demonstration, print the generated sensor data
            std::cout << "Generated sensor data: " << sensorData << std::endl;
        }

        // Close the WebSocket connection
        ws.close(websocket::close_code::normal);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
