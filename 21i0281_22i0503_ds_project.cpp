#include <iostream>   // I/O operations
#include <string>     // String handling
#include <climits>    // Limits constants
#include <fstream>    // File operations
#include <sstream>    // String stream

using namespace std;

struct Station {
    int id;          // Station ID
    int x;           // X coordinate
    int y;           // Y coordinate
    string name;     // Station name

    void printDetails() {  // Print details
        cout << "ID: " << id << ", Name: " << name << ", Coordinates: (" << x << ", " << y << ")" << endl;
    }
};

struct Incident {
    int id;                      // Incident ID
    int x;                       // X coordinate
    int y;                       // Y coordinate
    int reportTime;              // Report time
    int responseTime;            // Response time
    int reportedFromStationId;  // Report station
};

struct Dispatcher {
    int id;    // Dispatcher ID
    int x;     // X coordinate
    int y;     // Y coordinate
};

class EmergencyManager {
private:
    Station stations[10];         // Stations array
    Incident incidents[10];       // Incidents array
    Dispatcher dispatchers[10];   // Dispatchers array
    int numStations = 0;          // Station count
    int numIncidents = 0;         // Incident count
    int numDispatchers = 0;       // Dispatcher count

    int calculateShortestDistance(int x1, int y1, int x2, int y2) {  // Calculate distance
        return abs(x1 - x2) + abs(y1 - y2);
    }

public:
    void addStation(int id, int x, int y, const string& name) {  // Add station
        if (numStations < 10) {
            stations[numStations] = { id, x, y, name };
            ++numStations;
        }
        else {
            cout << "Max stations limit reached.\n";
        }
    }

    void addIncident(int id, int x, int y, int reportTime, int responseTime) {  // Add incident
        if (numIncidents < 10) {
            incidents[numIncidents] = { id, x, y, reportTime, responseTime, -1 };
            ++numIncidents;
        }
        else {
            cout << "Max incidents limit reached.\n";
        }
    }

    void addDispatcher(int id, int x, int y) {  // Add dispatcher
        if (numDispatchers < 10) {
            dispatchers[numDispatchers] = { id, x, y };
            ++numDispatchers;
        }
        else {
            cout << "Max dispatchers limit reached.\n";
        }
    }

    void printLocations() {  // Print locations
        cout << "Stations:\n";
        for (int i = 0; i < numStations; ++i) {
            stations[i].printDetails();
        }
        cout << "Incidents:\n";
        for (int i = 0; i < numIncidents; ++i) {
            cout << "ID: " << incidents[i].id << ", Coordinates: (" << incidents[i].x << ", " << incidents[i].y << ")" << endl;
        }
        cout << "Dispatchers:\n";
        for (int i = 0; i < numDispatchers; ++i) {
            cout << "ID: " << dispatchers[i].id << ", Coordinates: (" << dispatchers[i].x << ", " << dispatchers[i].y << ")" << endl;
        }
    }

    void displayMap() {  // Display map
        cout << "Visual Representation:\n";
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                bool found = false;
                for (int k = 0; k < numStations; ++k) {
                    if (stations[k].x == i && stations[k].y == j) {
                        cout << "S ";
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    for (int k = 0; k < numIncidents; ++k) {
                        if (incidents[k].x == i && incidents[k].y == j) {
                            cout << "I ";
                            found = true;
                            break;
                        }
                    }
                }
                if (!found) {
                    cout << ". ";
                }
            }
            cout << endl;
        }
    }

    int calculateShortestDistanceToStation(int incidentId) {  // Distance to station
        int incidentIndex = -1;
        for (int i = 0; i < numIncidents; ++i) {
            if (incidents[i].id == incidentId) {
                incidentIndex = i;
                break;
            }
        }

        if (incidentIndex == -1) {
            cout << "Incident with ID " << incidentId << " not found.\n";
            return -1;
        }

        int stationIndex = -1;
        for (int i = 0; i < numStations; ++i) {
            if (stations[i].id == incidents[incidentIndex].reportedFromStationId) {
                stationIndex = i;
                break;
            }
        }

        if (stationIndex == -1) {
            cout << "Station with ID " << incidents[incidentIndex].reportedFromStationId << " not found.\n";
            return -1;
        }

        return calculateShortestDistance(incidents[incidentIndex].x, incidents[incidentIndex].y, stations[stationIndex].x, stations[stationIndex].y);
    }

    void assignDispatcher(int incidentId) {  // Assign dispatcher
        int incidentIndex = -1;
        for (int i = 0; i < numIncidents; ++i) {
            if (incidents[i].id == incidentId) {
                incidentIndex = i;
                break;
            }
        }

        if (incidentIndex == -1) {
            cout << "Incident with ID " << incidentId << " not found.\n";
            return;
        }

        int minDistance = INT_MAX;
        int closestDispatcherIndex = -1;
        for (int i = 0; i < numDispatchers; ++i) {
            int distance = calculateShortestDistance(incidents[incidentIndex].x, incidents[incidentIndex].y, dispatchers[i].x, dispatchers[i].y);
            if (distance < minDistance) {
                minDistance = distance;
                closestDispatcherIndex = i;
            }
        }

        if (closestDispatcherIndex == -1) {
            cout << "No available dispatchers.\n";
            return;
        }

        cout << "Incident assigned to dispatcher ID " << dispatchers[closestDispatcherIndex].id << ".\n";
    }

    void reportIncident(int incidentId) {  // Report incident
        int incidentIndex = -1;
        for (int i = 0; i < numIncidents; ++i) {
            if (incidents[i].id == incidentId) {
                incidentIndex = i;
                break;
            }
        }

        if (incidentIndex == -1) {
            cout << "Incident with ID " << incidentId << " not found.\n";
            return;
        }

        cout << "Select a station:\n";
        for (int i = 0; i < numStations; ++i) {
            cout << i + 1 << ". " << stations[i].name << endl;
        }
        int stationIndex;
        cout << "Enter station index: ";
        cin >> stationIndex;

        if (stationIndex < 1 || stationIndex > numStations) {
            cout << "Invalid station index.\n";
            return;
        }

        incidents[incidentIndex].reportedFromStationId = stations[stationIndex - 1].id;
        cout << "Incident reported from station ID " << stations[stationIndex - 1].id << ".\n";
    }

    void autoAddCustomerAssignIncident() {  // Auto add and assign
        int stationId = 1;
        int stationX = 0;
        int stationY = 0;
        string stationName = "Central";
        addStation(stationId, stationX, stationY, stationName);

        int incidentId = 1;
        int incidentX = 3;
        int incidentY = 3;
        int reportTime = 12;
        int responseTime = 13;
        addIncident(incidentId, incidentX, incidentY, reportTime, responseTime);

        reportIncident(incidentId);
        assignDispatcher(incidentId);

        int shortestDistance = calculateShortestDistanceToStation(incidentId);
        if (shortestDistance != -1) {
            cout << "Shortest distance to station for incident with ID " << incidentId << " is: " << shortestDistance << endl;
        }
    }

    void saveToFile(const string& filename) {  // Save data to file
        ofstream outFile(filename);
        if (!outFile) {
            cerr << "Error opening file for writing.\n";
            return;
        }

        outFile << "Stations:\n";
        for (int i = 0; i < numStations; ++i) {
            outFile << stations[i].id << " " << stations[i].x << " " << stations[i].y << " " << stations[i].name << "\n";
        }

        outFile << "Incidents:\n";
        for (int i = 0; i < numIncidents; ++i) {
            outFile << incidents[i].id << " " << incidents[i].x << " " << incidents[i].y << " " << incidents[i].reportTime << " " << incidents[i].responseTime << " " << incidents[i].reportedFromStationId << "\n";
        }

        outFile << "Dispatchers:\n";
        for (int i = 0; i < numDispatchers; ++i) {
            outFile << dispatchers[i].id << " " << dispatchers[i].x << " " << dispatchers[i].y << "\n";
        }

        outFile.close();
        cout << "Data saved to " << filename << endl;
    }

    void loadFromFile(const string& filename) {  // Load data from file
        ifstream inFile(filename);
        if (!inFile) {
            cerr << "Error opening file for reading.\n";
            return;
        }

        string line;
        getline(inFile, line);  // Skip "Stations:"
        numStations = 0;
        while (getline(inFile, line) && !line.empty()) {
            int id, x, y;
            string name;
            istringstream iss(line);
            iss >> id >> x >> y;
            getline(iss, name);
            addStation(id, x, y, name.substr(1));  // Skip leading space
        }

        getline(inFile, line);  // Skip "Incidents:"
        numIncidents = 0;
        while (getline(inFile, line) && !line.empty()) {
            int id, x, y, reportTime, responseTime, reportedFromStationId;
            istringstream iss(line);
            iss >> id >> x >> y >> reportTime >> responseTime >> reportedFromStationId;
            addIncident(id, x, y, reportTime, responseTime);
            incidents[numIncidents - 1].reportedFromStationId = reportedFromStationId;
        }

        getline(inFile, line);  // Skip "Dispatchers:"
        numDispatchers = 0;
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
            cin.ignore();
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
            cin >> incidentId;
            manager.calculateShortestDistanceToStation(incidentId);
            break;
        }
              return 0;
        default:
            cout << "Invalid choice. Try again.\n";
            break;
        }
    }
}