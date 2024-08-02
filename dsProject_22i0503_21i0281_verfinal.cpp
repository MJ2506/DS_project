#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

struct Station {
    int id;
    int x, y;
    string name;

    void printDetails() {
        cout << "ID: " << id << ", Name: " << name << ", Coordinates: (" << x << ", " << y << ")" << endl;
    }
};

struct Incident {
    int id;
    int x, y;
    int reportTime;
    int responseTime;
    int reportedFromStationId;
};

struct Dispatcher {
    int id;
    int x, y;
};

struct StationNode {
    Station station;
    StationNode* next;
};

struct IncidentNode {
    Incident incident;
    IncidentNode* next;
};

struct DispatcherNode {
    Dispatcher dispatcher;
    DispatcherNode* next;
};

class EmergencyManager {
private:
    StationNode* stations;         // Head of stations linked list
    IncidentNode* incidents;       // Head of incidents linked list
    DispatcherNode* dispatchers;   // Head of dispatchers linked list

    int calculateShortestDistance(int x1, int y1, int x2, int y2) {
        return abs(x1 - x2) + abs(y1 - y2);
    }

public:
    EmergencyManager() : stations(nullptr), incidents(nullptr), dispatchers(nullptr) {}

    ~EmergencyManager() {
        StationNode* currentStation = stations;
        while (currentStation != nullptr) {
            StationNode* nextStation = currentStation->next;
            delete currentStation;
            currentStation = nextStation;
        }

        IncidentNode* currentIncident = incidents;
        while (currentIncident != nullptr) {
            IncidentNode* nextIncident = currentIncident->next;
            delete currentIncident;
            currentIncident = nextIncident;
        }

        DispatcherNode* currentDispatcher = dispatchers;
        while (currentDispatcher != nullptr) {
            DispatcherNode* nextDispatcher = currentDispatcher->next;
            delete currentDispatcher;
            currentDispatcher = nextDispatcher;
        }
    }

    void addStation(int id, int x, int y, const string& name) {
        StationNode* newNode = new StationNode{ {id, x, y, name}, stations };
        stations = newNode;
    }

    void addIncident(int id, int x, int y, int reportTime, int responseTime) {
        IncidentNode* newNode = new IncidentNode{ {id, x, y, reportTime, responseTime, -1}, incidents };
        incidents = newNode;
    }

    void addDispatcher(int id, int x, int y) {
        DispatcherNode* newNode = new DispatcherNode{ {id, x, y}, dispatchers };
        dispatchers = newNode;
    }

    void printLocations() {
        cout << "Stations:\n";
        StationNode* currentStation = stations;
        while (currentStation != nullptr) {
            currentStation->station.printDetails();
            currentStation = currentStation->next;
        }

        cout << "Incidents:\n";
        IncidentNode* currentIncident = incidents;
        while (currentIncident != nullptr) {
            cout << "ID: " << currentIncident->incident.id << ", Coordinates: (" << currentIncident->incident.x << ", " << currentIncident->incident.y << ")" << endl;
            currentIncident = currentIncident->next;
        }

        cout << "Dispatchers:\n";
        DispatcherNode* currentDispatcher = dispatchers;
        while (currentDispatcher != nullptr) {
            cout << "ID: " << currentDispatcher->dispatcher.id << ", Coordinates: (" << currentDispatcher->dispatcher.x << ", " << currentDispatcher->dispatcher.y << ")" << endl;
            currentDispatcher = currentDispatcher->next;
        }
    }

    void displayMap() {
        cout << "Visual Representation:\n";
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                bool found = false;

                // Check stations
                StationNode* currentStation = stations;
                while (currentStation != nullptr) {
                    if (currentStation->station.x == i && currentStation->station.y == j) {
                        cout << "S ";
                        found = true;
                        break;
                    }
                    currentStation = currentStation->next;
                }

                // Check incidents if no station found
                if (!found) {
                    IncidentNode* currentIncident = incidents;
                    while (currentIncident != nullptr) {
                        if (currentIncident->incident.x == i && currentIncident->incident.y == j) {
                            cout << "I ";
                            found = true;
                            break;
                        }
                        currentIncident = currentIncident->next;
                    }
                }

                // Empty space
                if (!found) {
                    cout << ". ";
                }
            }
            cout << endl;
        }
    }

    int calculateShortestDistanceToStation(int incidentId) {
        // Find the incident
        IncidentNode* incidentNode = incidents;
        while (incidentNode != nullptr && incidentNode->incident.id != incidentId) {
            incidentNode = incidentNode->next;
        }
        if (incidentNode == nullptr) {
            cout << "Incident with ID " << incidentId << " not found.\n";
            return -1;
        }

        // Find the reporting station
        StationNode* stationNode = stations;
        while (stationNode != nullptr && stationNode->station.id != incidentNode->incident.reportedFromStationId) {
            stationNode = stationNode->next;
        }
        if (stationNode == nullptr) {
            cout << "Station with ID " << incidentNode->incident.reportedFromStationId << " not found.\n";
            return -1;
        }

        return calculateShortestDistance(incidentNode->incident.x, incidentNode->incident.y, stationNode->station.x, stationNode->station.y);
    }

    void assignDispatcher(int incidentId) {
        // Find the incident
        IncidentNode* incidentNode = incidents;
        while (incidentNode != nullptr && incidentNode->incident.id != incidentId) {
            incidentNode = incidentNode->next;
        }
        if (incidentNode == nullptr) {
            cout << "Incident with ID " << incidentId << " not found.\n";
            return;
        }

        int minDistance = INT_MAX;
        DispatcherNode* closestDispatcher = nullptr;
        DispatcherNode* dispatcherNode = dispatchers;
        while (dispatcherNode != nullptr) {
            int distance = calculateShortestDistance(incidentNode->incident.x, incidentNode->incident.y, dispatcherNode->dispatcher.x, dispatcherNode->dispatcher.y);
            if (distance < minDistance) {
                minDistance = distance;
                closestDispatcher = dispatcherNode;
            }
            dispatcherNode = dispatcherNode->next;
        }

        if (closestDispatcher == nullptr) {
            cout << "No available dispatchers.\n";
            return;
        }

        cout << "Incident assigned to dispatcher ID " << closestDispatcher->dispatcher.id << ".\n";
    }

    void reportIncident(int incidentId) {
        // Find the incident
        IncidentNode* incidentNode = incidents;
        while (incidentNode != nullptr && incidentNode->incident.id != incidentId) {
            incidentNode = incidentNode->next;
        }
        if (incidentNode == nullptr) {
            cout << "Incident with ID " << incidentId << " not found.\n";
            return;
        }

        // Select a station
        cout << "Select a station:\n";
        StationNode* stationNode = stations;
        int index = 1;
        while (stationNode != nullptr) {
            cout << index++ << ". " << stationNode->station.name << endl;
            stationNode = stationNode->next;
        }

        int stationIndex;
        cout << "Enter station index: ";
        cin >> stationIndex;

        stationNode = stations;
        for (int i = 1; i < stationIndex && stationNode != nullptr; ++i) {
            stationNode = stationNode->next;
        }

        if (stationNode == nullptr) {
            cout << "Invalid station index.\n";
            return;
        }

        incidentNode->incident.reportedFromStationId = stationNode->station.id;
        cout << "Incident reported from station ID " << stationNode->station.id << ".\n";
    }

    void autoAddCustomerAssignIncident() {
        addStation(1, 0, 0, "Central");
        addIncident(1, 3, 3, 12, 13);

        reportIncident(1);
        assignDispatcher(1);

        int shortestDistance = calculateShortestDistanceToStation(1);
        if (shortestDistance != -1) {
            cout << "Shortest distance to station for incident with ID 1 is: " << shortestDistance << endl;
        }
    }

    void saveToFile(const string& filename) {
        ofstream outFile(filename);
        if (!outFile) {
            cerr << "Error opening file for writing.\n";
            return;
        }

        outFile << "Stations:\n";
        StationNode* stationNode = stations;
        while (stationNode != nullptr) {
            outFile << stationNode->station.id << " " << stationNode->station.x << " " << stationNode->station.y << " " << stationNode->station.name << "\n";
            stationNode = stationNode->next;
        }

        outFile << "Incidents:\n";
        IncidentNode* incidentNode = incidents;
        while (incidentNode != nullptr) {
            outFile << incidentNode->incident.id << " " << incidentNode->incident.x << " " << incidentNode->incident.y << " "
                << incidentNode->incident.reportTime << " " << incidentNode->incident.responseTime << " "
                << incidentNode->incident.reportedFromStationId << "\n";
            incidentNode = incidentNode->next;
        }

        outFile << "Dispatchers:\n";
        DispatcherNode* dispatcherNode = dispatchers;
        while (dispatcherNode != nullptr) {
            outFile << dispatcherNode->dispatcher.id << " " << dispatcherNode->dispatcher.x << " " << dispatcherNode->dispatcher.y << "\n";
            dispatcherNode = dispatcherNode->next;
        }

        outFile.close();
        cout << "Data saved to " << filename << endl;
    }

    void loadFromFile(const string& filename) {
        ifstream inFile(filename);
        if (!inFile) {
            cerr << "Error opening file for reading.\n";
            return;
        }

        // Clear existing data
        this->~EmergencyManager();

        string line;

        // Load stations
        getline(inFile, line);  // Skip "Stations:"
        while (getline(inFile, line) && !line.empty()) {
            int id, x, y;
            string name;
            istringstream iss(line);
            iss >> id >> x >> y;
            getline(iss, name);
            addStation(id, x, y, name.substr(1));  // Skip leading space
        }

        // Load incidents
        getline(inFile, line);  // Skip "Incidents:"
        while (getline(inFile, line) && !line.empty()) {
            int id, x, y, reportTime, responseTime, reportedFromStationId;
            istringstream iss(line);
            iss >> id >> x >> y >> reportTime >> responseTime >> reportedFromStationId;
            addIncident(id, x, y, reportTime, responseTime);
            incidents->incident.reportedFromStationId = reportedFromStationId;
        }

        // Load dispatchers
        getline(inFile, line);  // Skip "Dispatchers:"
        while (getline(inFile, line) && !line.empty()) {
            int id, x, y;
            istringstream iss(line);
            iss >> id >> x >> y;
            addDispatcher(id, x, y);
        }

        inFile.close();
        cout << "Data loaded from " << filename << endl;
    }
};

int main() {
    EmergencyManager manager;
    int choice;

    while (true) {
        cout << "1. Add Station\n";
        cout << "2. Add Incident\n";
        cout << "3. Add Dispatcher\n";
        cout << "4. Print Locations\n";
        cout << "5. Display Map\n";
        cout << "6. Report Incident\n";
        cout << "7. Assign Dispatcher\n";
        cout << "8. Calculate Shortest Distance\n";
        cout << "9. Auto Add and Assign Incident\n";
        cout << "10. Save Data to File\n";
        cout << "11. Load Data from File\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            int id, x, y;
            string name;
            cout << "Enter Station ID, X, Y, Name: ";
            cin >> id >> x >> y;
            cin.ignore();  // Ignore newline character after integer inputs
            getline(cin, name);
            manager.addStation(id, x, y, name);
            break;
        }
        case 2: {
            int id, x, y, reportTime, responseTime;
            cout << "Enter Incident ID, X, Y, Report Time, Response Time: ";
            cin >> id >> x >> y >> reportTime >> responseTime;
            manager.addIncident(id, x, y, reportTime, responseTime);
            break;
        }
        case 3: {
            int id, x, y;
            cout << "Enter Dispatcher ID, X, Y: ";
            cin >> id >> x >> y;
            manager.addDispatcher(id, x, y);
            break;
        }
        case 4:
            manager.printLocations();
            break;
        case 5:
            manager.displayMap();
            break;
        case 6: {
            int incidentId;
            cout << "Enter Incident ID to report: ";
            cin >> incidentId;
            manager.reportIncident(incidentId);
            break;
        }
        case 7: {
            int incidentId;
            cout << "Enter Incident ID to assign dispatcher: ";
            cin >> incidentId;
            manager.assignDispatcher(incidentId);
            break;
        }
        case 8: {
            int incidentId;
            cout << "Enter Incident ID to calculate distance: ";
            cin >> incidentId;
            manager.calculateShortestDistanceToStation(incidentId);
            break;
        }
        case 9:
            manager.autoAddCustomerAssignIncident();
            break;
        case 10: {
            string filename;
            cout << "Enter filename to save data: ";
            cin >> filename;
            manager.saveToFile(filename);
            break;
        }
        case 11: {
            string filename;
            cout << "Enter filename to load data: ";
            cin >> filename;
            manager.loadFromFile(filename);
            break;
        }
        case 0:
            return 0;
        default:
            cout << "Invalid choice. Try again.\n";
            break;
        }
    }
}