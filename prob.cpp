#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>
using namespace std;

// Цвета для консоли
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define RESET "\033[0m"

// Типы данных
enum class UserRole { Operator, Admin };
enum class SensorType { Gas, Temperature, Pressure };

struct User {
    string username;
    string password;
    UserRole role;
};

struct Sensor {
    int id;
    SensorType type;
    string location;
    double value;
    double threshold;
    
    string getTypeName() {
        switch(type) {
            case SensorType::Gas: return "Gas";
            case SensorType::Temperature: return "Temperature";
            case SensorType::Pressure: return "Pressure";
            default: return "Unknown";
        }
    }
};

struct Incident {
    int id;
    string description;
    time_t timestamp;
    string getTime() {
        char buf[20];
        strftime(buf, 20, "%Y-%m-%d %H:%M:%S", localtime(&timestamp));
        return buf;
    }
};

// База данных
class Database {
public:
    vector<User> users;
    vector<Sensor> sensors;
    vector<Incident> incidents;
    int nextSensorId = 1;
    int nextIncidentId = 1;
    
    Database() {
        // Добавляем пользователей по умолчанию
        users.push_back({"admin", "admin123", UserRole::Admin});
        users.push_back({"operator", "operator123", UserRole::Operator});
    }
    
    User* authenticate(string username, string password) {
        for (auto& user : users) {
            if (user.username == username && user.password == password) {
                return &user;
            }
        }
        return nullptr;
    }
    
    void addSensor(SensorType type, string loc, double th) {
        sensors.push_back({nextSensorId++, type, loc, 0.0, th});
    }
    
    void addIncident(string desc) {
        incidents.push_back({nextIncidentId++, desc, time(nullptr)});
    }
    
    void updateSensorValue(int id, double newValue) {
        for (auto& s : sensors) {
            if (s.id == id) {
                s.value = newValue;
                if (newValue > s.threshold) {
                    cout << RED << "WARNING: " << s.getTypeName() 
                         << " sensor #" << id << " exceeded threshold! (" 
                         << newValue << " > " << s.threshold << ")" << RESET << endl;
                    addIncident(s.getTypeName() + " threshold exceeded at " + s.location);
                }
                return;
            }
        }
        cout << "Sensor not found!" << endl;
    }
};

// Пользовательский интерфейс
class UI {
    Database db;
    User* currentUser = nullptr;
    
    bool requireAdmin() {
        if (!currentUser || currentUser->role != UserRole::Admin) {
            cout << RED << "Admin privileges required!" << RESET << endl;
            return false;
        }
        return true;
    }
    
    bool requireAuth() {
        if (!currentUser) {
            cout << RED << "Please login first!" << RESET << endl;
            return false;
        }
        return true;
    }
    
    void login() {
        string username, password;
        cout << "\n=== Login ===" << endl;
        cout << "Username: "; cin >> username;
        cout << "Password: "; cin >> password;
        
        currentUser = db.authenticate(username, password);
        if (currentUser) {
            cout << GREEN << "Welcome, " << username << "!" << RESET << endl;
        } else {
            cout << RED << "Invalid credentials!" << RESET << endl;
        }
    }
    
    void logout() {
        currentUser = nullptr;
        cout << YELLOW << "Logged out successfully." << RESET << endl;
    }
    
    void showMainMenu() {
        cout << "\n=== Industrial Monitoring ===" << endl;
        if (currentUser) {
            cout << "Logged in as: " << currentUser->username 
                 << (currentUser->role == UserRole::Admin ? " (Admin)" : "") << endl;
        }
        cout << "1. Login\n2. Add Sensor\n3. View Sensors\n4. Update Sensor\n5. View Incidents\n6. Add Incident\n7. Logout\n8. Exit\nChoice: ";
    }
    
    void addSensor() {
        if (!requireAdmin()) return;
        
        cout << "\nSensor Type (0-Gas, 1-Temp, 2-Pressure): ";
        int type; cin >> type;
        cout << "Location: "; 
        string loc; cin.ignore(); getline(cin, loc);
        cout << "Threshold: ";
        double th; cin >> th;
        db.addSensor(static_cast<SensorType>(type), loc, th);
        cout << GREEN << "Sensor added!" << RESET << endl;
    }
    
    void viewSensors() {
        if (!requireAuth()) return;
        
        cout << "\n=== Sensors ===" << endl;
        for (auto& s : db.sensors) {
            cout << "ID: " << s.id << " | Type: " << s.getTypeName() 
                 << " | Location: " << s.location << " | Value: " << s.value 
                 << " | Threshold: " << s.threshold << endl;
        }
    }
    
    void updateSensor() {
        if (!requireAuth()) return;
        
        cout << "\nSensor ID: ";
        int id; cin >> id;
        cout << "New Value: ";
        double val; cin >> val;
        db.updateSensorValue(id, val);
    }
    
    void viewIncidents() {
        if (!requireAuth()) return;
        
        cout << "\n=== Incidents ===" << endl;
        for (auto& i : db.incidents) {
            cout << "ID: " << i.id << " | Time: " << i.getTime() 
                 << " | " << i.description << endl;
        }
    }
    
    void addIncident() {
        if (!requireAuth()) return;
        
        cout << "\nDescription: ";
        string desc; cin.ignore(); getline(cin, desc);
        db.addIncident(desc);
        cout << GREEN << "Incident added!" << RESET << endl;
    }
    
public:
    void run() {
        // Добавим тестовые данные
        db.addSensor(SensorType::Temperature, "Boiler Room", 100.0);
        db.addSensor(SensorType::Pressure, "Main Pipeline", 50.0);
        
        while (true) {
            showMainMenu();
            int choice; cin >> choice;
            
            switch (choice) {
                case 1: login(); break;
                case 2: addSensor(); break;
                case 3: viewSensors(); break;
                case 4: updateSensor(); break;
                case 5: viewIncidents(); break;
                case 6: addIncident(); break;
                case 7: logout(); break;
                case 8: return;
                default: cout << "Invalid choice!" << endl;
            }
        }
    }
};

int main() {
    UI ui;
    ui.run();
    return 0;
}