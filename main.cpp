// main.cpp - Пользовательский интерфейс
#include <iostream>
#include "logic.h"
using namespace std;

void display_menu() {
    cout << "\nIndustrial Safety System\n";
    cout << "1. Register sensor\n";
    cout << "2. Update sensor reading\n";
    cout << "3. Check sensor status\n";
    cout << "4. Report incident\n";
    cout << "5. View all sensors\n";
    cout << "6. View all incidents\n";
    cout << "7. Exit\n";
    cout << "Choose option: ";
}

Sensor input_sensor() {
    Sensor s;
    cout << "Enter sensor ID: ";
    cin >> s.id;
    cout << "Enter type (gas/temp/pressure): ";
    cin >> s.type;
    cout << "Enter location: ";
    cin.ignore();
    getline(cin, s.location);
    cout << "Enter threshold: ";
    cin >> s.threshold;
    s.current_value = 0;
    s.last_calibration = time(nullptr);
    return s;
}

int main() {
    SensorRepository sensor_repo;
    IncidentRepository incident_repo;
    SafetyService service(sensor_repo, incident_repo);
    
    int choice;
    do {
        display_menu();
        cin >> choice;
        
        try {
            switch(choice) {
                case 1: {
                    Sensor s = input_sensor();
                    if (service.register_sensor(s))
                        cout << "Sensor registered!\n";
                    break;
                }
                case 2: {
                    int id;
                    double value;
                    cout << "Enter sensor ID: ";
                    cin >> id;
                    cout << "Enter new value: ";
                    cin >> value;
                    
                    Sensor* s = sensor_repo.find(id);
                    if (s) {
                        s->current_value = value;
                        sensor_repo.update(*s);
                        cout << "Value updated. Status: " 
                             << service.check_sensor(id) << "\n";
                    } else {
                        cout << "Sensor not found!\n";
                    }
                    break;
                }
                case 3: {
                    int id;
                    cout << "Enter sensor ID: ";
                    cin >> id;
                    cout << service.check_sensor(id) << "\n";
                    break;
                }
                case 4: {
                    string type;
                    vector<int> sensor_ids;
                    int sensor_id;
                    
                    cout << "Enter incident type: ";
                    cin >> type;
                    cout << "Enter related sensor IDs (0 to finish):\n";
                    while (cin >> sensor_id && sensor_id != 0) {
                        sensor_ids.push_back(sensor_id);
                    }
                    
                    int inc_id = service.report_incident(type, sensor_ids);
                    cout << "Incident #" << inc_id << " reported\n";
                    break;
                }
                case 5: {
                    cout << service.getAllSensorsInfo();
                    break;
                }
                case 6: {
                    cout << service.getAllIncidentsInfo();
                    break;
                }
                case 7: {
                    cout << "Exiting...\n";
                    break;
                }
                default: {
                    cout << "Invalid option!\n";
                    break;
                }
            }
        } catch (...) {
            cerr << "Error occurred!\n";
        }
    } while (choice != 7);
    
    return 0;
}