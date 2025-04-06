// database.h - Репозитории данных
#pragma once
#include "domain.h"
#include <vector>
#include <iomanip>
#include <ctime>

class SensorRepository {
    vector<Sensor> sensors;
public:
    bool add(const Sensor& s) {
        sensors.push_back(s);
        return true;
    }
    
    Sensor* find(int id) {
        for (auto& s : sensors)
            if (s.id == id) return &s;
        return nullptr;
    }
    
    bool update(const Sensor& s) {
        for (auto& existing : sensors)
            if (existing.id == s.id) {
                existing = s;
                return true;
            }
        return false;
    }
    
    const vector<Sensor>& getAll() const {
        return sensors;
    }
};

class IncidentRepository {
    vector<Incident> incidents;
    int next_id = 1;
public:
    int add(const Incident& i) {
        Incident new_inc = i;
        new_inc.id = next_id++;
        incidents.push_back(new_inc);
        return new_inc.id;
    }
    
    bool update(const Incident& i) {
        for (auto& existing : incidents)
            if (existing.id == i.id) {
                existing = i;
                return true;
            }
        return false;
    }
    
    const vector<Incident>& getAll() const {
        return incidents;
    }
};