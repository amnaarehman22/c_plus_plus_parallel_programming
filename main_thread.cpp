#include "main_thread.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <random>
#include <thread>  // For std::this_thread::sleep_for

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

    while (true) {
        // Generate random sensor data
        int sensorId = 1;  // Dummy sensor ID
        double x = disCoord(gen);
        double y = disCoord(gen);
        double z = disCoord(gen);

        std::string sensorData = generateDummySensorData(sensorId, x, y, z);

        // Simulate processing delay
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // TODO: Store sensorData in caching struct or send to websocket server
        // For demonstration, just print the generated sensor data
        std::cout << "Generated sensor data: " << sensorData << std::endl;
    }
}
