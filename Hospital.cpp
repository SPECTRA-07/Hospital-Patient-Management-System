#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <list>
#include <iomanip>

using namespace std;

// Structure to store patient details
struct Patient {
    int id;
    string name;
    int age;
    string condition; // e.g., "Critical" or "Stable"
    string admissionDate;

    // Comparator for priority queue (higher priority for critical patients)
    bool operator<(const Patient &other) const {
        return condition != "Critical" && other.condition == "Critical";
    }
};

// Class to manage the hospital system
class Hospital {
private:
    priority_queue<Patient> criticalPatients;    // Priority queue for critical patients
    queue<Patient> stablePatients;              // Queue for stable patients
    list<Patient> patientRecords;               // Linked list to maintain all patient records
    vector<int> rooms;                          // Rooms array (1 = occupied, 0 = vacant)

public:
    Hospital(int totalRooms) : rooms(totalRooms, 0) {}

    // Admit a patient
    void admitPatient(int id, string name, int age, string condition, string admissionDate) {
        int roomNumber = findAvailableRoom();
        if (roomNumber == -1) {
            cout << "No rooms available! Please wait.\n";
            return;
        }

        Patient newPatient = {id, name, age, condition, admissionDate};
        rooms[roomNumber] = 1; // Mark room as occupied

        if (condition == "Critical") {
            criticalPatients.push(newPatient);
        } else {
            stablePatients.push(newPatient);
        }

        patientRecords.push_back(newPatient);
        cout << "Patient " << name << " admitted successfully in room " << roomNumber + 1 << ".\n";
    }

    // Discharge a patient
    void dischargePatient(int id) {
        bool found = false;

        for (auto it = patientRecords.begin(); it != patientRecords.end(); ++it) {
            if (it->id == id) {
                int roomNumber = findRoomByPatient(*it);
                if (roomNumber != -1) {
                    rooms[roomNumber] = 0; // Mark room as vacant
                }
                cout << "Patient " << it->name << " discharged successfully.\n";
                patientRecords.erase(it);
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "Patient with ID " << id << " not found.\n";
        }
    }

    // Display all patient records sorted by admission date
    void displayPatientRecords() {
        if (patientRecords.empty()) {
            cout << "No patient records available.\n";
            return;
        }

        cout << left << setw(5) << "ID" << setw(20) << "Name" << setw(5) << "Age"
             << setw(10) << "Condition" << setw(15) << "Admission Date" << endl;
        cout << string(55, '-') << endl;

        for (const auto &patient : patientRecords) {
            cout << left << setw(5) << patient.id << setw(20) << patient.name
                 << setw(5) << patient.age << setw(10) << patient.condition
                 << setw(15) << patient.admissionDate << endl;
        }
    }

    // Manage critical patients (view and treat)
    void manageCriticalPatients() {
        if (criticalPatients.empty()) {
            cout << "No critical patients to manage.\n";
            return;
        }

        Patient patient = criticalPatients.top();
        criticalPatients.pop();
        cout << "Treating critical patient: " << patient.name << " (ID: " << patient.id << ")\n";

        // After treatment, patient condition is updated to "Stable"
        patient.condition = "Stable";
        stablePatients.push(patient);
    }

private:
    // Find an available room (returns -1 if no room is available)
    int findAvailableRoom() {
        for (size_t i = 0; i < rooms.size(); ++i) {
            if (rooms[i] == 0) {
                return i;
            }
        }
        return -1;
    }

    // Find room number by patient (returns -1 if not found)
    int findRoomByPatient(const Patient &patient) {
        for (size_t i = 0; i < rooms.size(); ++i) {
            if (rooms[i] == 1) { // Assuming patient is assigned to this room
                return i;
            }
        }
        return -1;
    }
};

// Main function
int main() {
    int totalRooms;
    cout << "Enter the total number of rooms in the hospital: ";
    cin >> totalRooms;

    Hospital hospital(totalRooms);

    int choice;
    do {
        cout << "\nHospital Management System\n";
        cout << "1. Admit Patient\n";
        cout << "2. Discharge Patient\n";
        cout << "3. Display Patient Records\n";
        cout << "4. Manage Critical Patients\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int id, age;
                string name, condition, admissionDate;
                cout << "Enter patient ID: ";
                cin >> id;
                cin.ignore();
                cout << "Enter patient name: ";
                getline(cin, name);
                cout << "Enter patient age: ";
                cin >> age;
                cin.ignore();
                cout << "Enter condition (Critical/Stable): ";
                getline(cin, condition);
                cout << "Enter admission date (DD-MM-YYYY): ";
                getline(cin, admissionDate);
                hospital.admitPatient(id, name, age, condition, admissionDate);
                break;
            }
            case 2: {
                int id;
                cout << "Enter patient ID to discharge: ";
                cin >> id;
                hospital.dischargePatient(id);
                break;
            }
            case 3:
                hospital.displayPatientRecords();
                break;
            case 4:
                hospital.manageCriticalPatients();
                break;
            case 5:
                cout << "Exiting the system. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 5);

    return 0;
}
