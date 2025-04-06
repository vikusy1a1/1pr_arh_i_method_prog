#pragma once
#include "database.h"
#include <ctime>

class SafetyService {
    SensorRepository& sensor_repo;
    IncidentRepository& incident_repo;
    
public:
    SafetyService(SensorRepository& sr, IncidentRepository& ir)
        : sensor_repo(sr), incident_repo(ir) {}
    
    bool register_sensor(const Sensor& s) {
        return sensor_repo.add(s);
    }
    
    std::string check_sensor(int id) {
        Sensor* s = sensor_repo.find(id);
        if (!s) return "Sensor not found!";
        
        if (s->current_value > s->threshold) {
            return "DANGER! Value " + std::to_string(s->current_value) + 
                   " exceeds threshold " + std::to_string(s->threshold);
        }
        return "OK";
    }
    
    int report_incident(const std::string& type, 
                       const std::vector<int>& sensor_ids) {
        Incident inc;
        inc.type = type;
        inc.related_sensors = sensor_ids;
        inc.timestamp = time(nullptr);
        inc.status = "active";
        
        // Определяем уровень опасности
        int danger_count = 0;
        for (int id : sensor_ids) {
            if (sensor_repo.find(id)->current_value > 
                sensor_repo.find(id)->threshold)
                danger_count++;
        }
        
        inc.severity = danger_count > 2 ? "high" : 
                      danger_count > 0 ? "medium" : "low";
        
        return incident_repo.add(inc);
    }
};