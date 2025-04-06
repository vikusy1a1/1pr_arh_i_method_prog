// domain.h - Доменные сущности
#pragma once
#include <string>
#include <vector>
#include <chrono>
using namespace std;

struct Sensor {
    int id;
    string type; // "gas", "temperature", "pressure"
    string location;
    double current_value;
    double threshold;
    time_t last_calibration;
};

struct Incident {
    int id;
    string type;
    string severity; // "low", "medium", "high"
    string status;   // "active", "resolved"
    time_t timestamp;
    vector<int> related_sensors;
};