// logic.h - Бизнес-логика
#pragma once
#include "database.h"
#include <ctime>
#include <sstream>

class SafetyService {
    SensorRepository& sensor_repo;
    IncidentRepository& incident_repo;
    
    string formatTime(time_t time) {
        tm* tm = localtime(&time);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm);
        return string(buffer);
    }
    
public:
    SafetyService(SensorRepository& sr, IncidentRepository& ir)
        : sensor_repo(sr), incident_repo(ir) {}
    
    bool register_sensor(const Sensor& s) {
        return sensor_repo.add(s);
    }
    
    string check_sensor(int id) {
        Sensor* s = sensor_repo.find(id);
        if (!s) return "Sensor not found!";
        
        if (s->current_value > s->threshold) {
            return "DANGER! Value " + to_string(s->current_value) + 
                   " exceeds threshold " + to_string(s->threshold);
        }
        return "OK";
    }
    
    int report_incident(const string& type, 
                       const vector<int>& sensor_ids) {
        Incident inc;
        inc.type = type;
        inc.related_sensors = sensor_ids;
        inc.timestamp = time(nullptr);
        inc.status = "active";
        
        // Определяем уровень опасности
        int danger_count = 0;
        for (int id : sensor_ids) {
            Sensor* s = sensor_repo.find(id);
            if (s && s->current_value > s->threshold)
                danger_count++;
        }
        
        inc.severity = danger_count > 2 ? "high" : 
                      danger_count > 0 ? "medium" : "low";
        
        return incident_repo.add(inc);
    }
    
    string getAllSensorsInfo() {
        const auto& sensors = sensor_repo.getAll();
        if (sensors.empty()) return "No sensors registered\n";
        
        ostringstream oss;
        oss << "\n=== All Sensors ===\n";
        for (const auto& s : sensors) {
            oss << "ID: " << s.id << "\n"
                << "Type: " << s.type << "\n"
                << "Location: " << s.location << "\n"
                << "Current value: " << s.current_value << "\n"
                << "Threshold: " << s.threshold << "\n"
                << "Last calibration: " << formatTime(s.last_calibration) << "\n\n";
        }
        return oss.str();
    }
    
    string getAllIncidentsInfo() {
        const auto& incidents = incident_repo.getAll();
        if (incidents.empty()) return "No incidents reported\n";
        
        ostringstream oss;
        oss << "\n=== All Incidents ===\n";
        for (const auto& inc : incidents) {
            oss << "ID: " << inc.id << "\n"
                << "Type: " << inc.type << "\n"
                << "Severity: " << inc.severity << "\n"
                << "Status: " << inc.status << "\n"
                << "Timestamp: " << formatTime(inc.timestamp) << "\n"
                << "Related sensors: ";
            
            for (int id : inc.related_sensors)
                oss << id << " ";
            oss << "\n\n";
        }
        return oss.str();
    }
};