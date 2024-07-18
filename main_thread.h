#ifndef MAIN_THREAD_H
#define MAIN_THREAD_H

#include <string>

// Function to generate dummy sensor data in JSON format
std::string generateDummySensorData(int sensorId, double x, double y, double z);

// Main thread function to simulate sensor data processing
void mainThread();

#endif // MAIN_THREAD_H
