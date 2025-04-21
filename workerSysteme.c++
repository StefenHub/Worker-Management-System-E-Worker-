#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <windows.h>
#include <fstream>
#include <limits>
using namespace std;

struct Worker {
    string id;
    string name;
    char gender;
    string position;
    double wage;
    int hours;
};

void pressEnterToContinue() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

bool isIdExists(Worker workers[], int count, const string& id) {
    for (int i = 0; i < count; i++) {
        if (workers[i].id == id) {
            return true;
        }
    }
    return false;
}

string generateId(Worker workers[], int count) {
    static int nextNum = 1;
    int maxNum = 99999;

    if (nextNum > maxNum) {
        return "";
    }

    ostringstream oss;
    oss << "w" << setw(5) << setfill('0') << nextNum;
    string newId = oss.str();

    nextNum++;

    while (isIdExists(workers, count, newId) && nextNum <= maxNum) {
        oss.str("");
        oss << "w" << setw(5) << setfill('0') << nextNum;
        newId = oss.str();
        nextNum++;
    }

    if (nextNum > maxNum && isIdExists(workers, count, newId)) {
        return "";
    }

    return newId;
}

void calculateSalary(Worker w, double& salaryBeforeTax, double& tax, double& salaryAfterTax) {
    salaryBeforeTax = w.wage * w.hours;
    tax = 0.0;
    salaryAfterTax = salaryBeforeTax;

    if (salaryBeforeTax >= 1000) {
        if (w.gender == 'F' || w.gender == 'f') {
            tax = salaryBeforeTax * 0.25;
            salaryAfterTax = salaryBeforeTax * 0.75;
        } else if (w.gender == 'M' || w.gender == 'm') {
            tax = salaryBeforeTax * 0.30;
            salaryAfterTax = salaryBeforeTax * 0.70;
        }
    }
}

void displayWorker(Worker w) {
    double salaryBeforeTax, tax, salaryAfterTax;
    calculateSalary(w, salaryBeforeTax, tax, salaryAfterTax);

    cout << fixed << setprecision(2);
    cout << "ID: " << w.id << endl;
    cout << "Name: " << w.name << endl;
    cout << "Gender: " << w.gender << endl;
    cout << "Position: " << w.position << endl;
    cout << "Wage: $" << w.wage << endl;
    cout << "Hours: " << w.hours << endl;
    cout << "Gross Salary: $" << salaryBeforeTax << endl;
    cout << "Tax Money: $" << tax << endl;
    cout << "Net Salary: $" << salaryAfterTax << endl;
}

void addWorker(Worker workers[], int& count, int maxSize) {
    char continueAdding;
    int workersAdded = 0;

    do {
        if (count >= maxSize) {
            cout << "Worker database is full!" << endl;
            break;
        }

        Worker w;
        w.id = generateId(workers, count);
        if (w.id.empty()) {
            cout << "No more unique IDs available!" << endl;
            break;
        }

        cout << "\n=========[Enter details for Worker]=========" << endl;
        cout << "ID: " << w.id << endl;
        cin.ignore();
        cout << "Name: ";
        getline(cin, w.name);
        cout << "Gender (M/F): ";
        cin >> w.gender;
        while (w.gender != 'M' && w.gender != 'm' && w.gender != 'F' && w.gender != 'f') {
            cout << "Invalid gender! Please enter 'M' or 'F': ";
            cin >> w.gender;
        }
        cin.ignore();
        cout << "Position: ";
        getline(cin, w.position);
        cout << "Wage per hour: ";
        cin >> w.wage;
        while (w.wage <= 0) {
            cout << "Wage must be positive! Enter again: ";
            cin >> w.wage;
        }
        cout << "Hours worked: ";
        cin >> w.hours;
        while (w.hours <= 0) {
            cout << "Hours must be positive! Enter again: ";
            cin >> w.hours;
        }

        workers[count] = w;
        count++;
        workersAdded++;
        cout << "Worker " << w.id << " added successfully!" << endl;

        if (count >= maxSize) {
            cout << "Worker database is full! Cannot add more workers." << endl;
            break;
        }

        if (w.id.empty()) {
            cout << "No more unique IDs available! Cannot add more workers." << endl;
            break;
        }

        cout << "\nDo you want to add another worker? (y/n): ";
        cin >> continueAdding;
    } while (continueAdding == 'y' || continueAdding == 'Y');

    if (workersAdded > 0) {
        cout << "\nAdded " << workersAdded << " worker(s) successfully!" << endl;
    } else {
        cout << "\nNo workers were added." << endl;
    }
    pressEnterToContinue();
}

void editWorker(Worker workers[], int count) {
    if (count == 0) {
        cout << "\nNo workers in the database!" << endl;
        pressEnterToContinue();
        return;
    }

    char choice;
    cout << "\nEdit by (I)D or (N)ame? (I/N): ";
    cin >> choice;
    cin.ignore();

    if (choice == 'I' || choice == 'i') {
        string id;
        cout << "Enter Worker ID to edit: ";
        cin >> id;
        
        for (int i = 0; i < count; i++) {
            if (workers[i].id == id) {
                cout << "\nCurrent details for worker ID " << id << ":" << endl;
                displayWorker(workers[i]);
                cout << "\nEnter new details (press Enter to keep current value):" << endl;

                cin.ignore();
                cout << "Name (" << workers[i].name << "): ";
                string newName;
                getline(cin, newName);
                if (!newName.empty()) {
                    workers[i].name = newName;
                }

                cout << "Gender (M/F) (" << workers[i].gender << "): ";
                string newGender;
                getline(cin, newGender);
                if (!newGender.empty() && (newGender == "M" || newGender == "m" || newGender == "F" || newGender == "f")) {
                    workers[i].gender = newGender[0];
                }

                cout << "Position (" << workers[i].position << "): ";
                string newPosition;
                getline(cin, newPosition);
                if (!newPosition.empty()) {
                    workers[i].position = newPosition;
                }

                cout << "Wage per hour ($" << fixed << setprecision(2) << workers[i].wage << "): ";
                string newWageStr;
                getline(cin, newWageStr);
                if (!newWageStr.empty()) {
                    try {
                        double newWage = stod(newWageStr);
                        if (newWage <= 0) {
                            cout << "Wage must be positive! Keeping current value." << endl;
                        } else {
                            workers[i].wage = newWage;
                        }
                    } catch (...) {
                        cout << "Invalid wage input. Keeping current value." << endl;
                    }
                }

                cout << "Hours worked (" << workers[i].hours << "): ";
                string newHoursStr;
                getline(cin, newHoursStr);
                if (!newHoursStr.empty()) {
                    try {
                        int newHours = stoi(newHoursStr);
                        if (newHours <= 0) {
                            cout << "Hours must be positive! Keeping current value." << endl;
                        } else {
                            workers[i].hours = newHours;
                        }
                    } catch (...) {
                        cout << "Invalid hours input. Keeping current value." << endl;
                    }
                }

                cout << "Worker updated successfully!" << endl;
                pressEnterToContinue();
                return;
            }
        }
        cout << "Worker not found!" << endl;
    } else if (choice == 'N' || choice == 'n') {
        string name;
        cout << "Enter Worker Name to edit: ";
        getline(cin, name);

        string searchName = name;
        transform(searchName.begin(), searchName.end(), searchName.begin(), ::tolower);

        int matches = 0;
        cout << "\nMatching workers:" << endl;
        for (int i = 0; i < count; i++) {
            string workerName = workers[i].name;
            transform(workerName.begin(), workerName.end(), workerName.begin(), ::tolower);
            if (workerName == searchName) {
                matches++;
                cout << "\nWorker " << matches << ":" << endl;
                displayWorker(workers[i]);
                cout << "------------------------" << endl;
            }
        }

        if (matches == 0) {
            cout << "No workers found with name '" << name << "'!" << endl;
            pressEnterToContinue();
            return;
        }

        string id;
        if (matches == 1) {
            for (int i = 0; i < count; i++) {
                string workerName = workers[i].name;
                transform(workerName.begin(), workerName.end(), workerName.begin(), ::tolower);
                if (workerName == searchName) {
                    id = workers[i].id;
                    break;
                }
            }
        } else {
            cout << "Multiple workers found. Enter the ID of the worker to edit: ";
            cin >> id;
            bool validId = false;
            for (int i = 0; i < count; i++) {
                if (workers[i].id == id) {
                    string workerName = workers[i].name;
                    transform(workerName.begin(), workerName.end(), workerName.begin(), ::tolower);
                    if (workerName == searchName) {
                        validId = true;
                        break;
                    }
                }
            }
            if (!validId) {
                cout << "No worker with ID '" << id << "' and name '" << name << "' found!" << endl;
                pressEnterToContinue();
                return;
            }
        }

        for (int i = 0; i < count; i++) {
            if (workers[i].id == id) {
                cout << "\nCurrent details for worker ID " << id << ":" << endl;
                displayWorker(workers[i]);
                cout << "\nEnter new details (press Enter to keep current value):" << endl;

                cin.ignore();
                cout << "Name (" << workers[i].name << "): ";
                string newName;
                getline(cin, newName);
                if (!newName.empty()) {
                    workers[i].name = newName;
                }

                cout << "Gender (M/F) (" << workers[i].gender << "): ";
                string newGender;
                getline(cin, newGender);
                if (!newGender.empty() && (newGender == "M" || newGender == "m" || newGender == "F" || newGender == "f")) {
                    workers[i].gender = newGender[0];
                }

                cout << "Position (" << workers[i].position << "): ";
                string newPosition;
                getline(cin, newPosition);
                if (!newPosition.empty()) {
                    workers[i].position = newPosition;
                }

                cout << "Wage per hour ($" << fixed << setprecision(2) << workers[i].wage << "): ";
                string newWageStr;
                getline(cin, newWageStr);
                if (!newWageStr.empty()) {
                    try {
                        double newWage = stod(newWageStr);
                        if (newWage <= 0) {
                            cout << "Wage must be positive! Keeping current value." << endl;
                        } else {
                            workers[i].wage = newWage;
                        }
                    } catch (...) {
                        cout << "Invalid wage input. Keeping current value." << endl;
                    }
                }

                cout << "Hours worked (" << workers[i].hours << "): ";
                string newHoursStr;
                getline(cin, newHoursStr);
                if (!newHoursStr.empty()) {
                    try {
                        int newHours = stoi(newHoursStr);
                        if (newHours <= 0) {
                            cout << "Hours must be positive! Keeping current value." << endl;
                        } else {
                            workers[i].hours = newHours;
                        }
                    } catch (...) {
                        cout << "Invalid hours input. Keeping current value." << endl;
                    }
                }

                cout << "Worker updated successfully!" << endl;
                pressEnterToContinue();
                return;
            }
        }
    } else {
        cout << "Invalid choice! Please enter 'I' or 'N'." << endl;
    }
    pressEnterToContinue();
}

void deleteWorker(Worker workers[], int& count) {
    if (count == 0) {
        cout << "\nNo workers in the database!" << endl;
        pressEnterToContinue();
        return;
    }

    char choice;
    cout << "\nDelete by (I)D or (N)ame? (I/N): ";
    cin >> choice;
    cin.ignore();

    if (choice == 'I' || choice == 'i') {
        string id;
        cout << "Enter Worker ID to delete: ";
        cin >> id;
        
        for (int i = 0; i < count; i++) {
            if (workers[i].id == id) {
                for (int j = i; j < count - 1; j++) {
                    workers[j] = workers[j + 1];
                }
                count--;
                cout << "Worker deleted successfully!" << endl;
                pressEnterToContinue();
                return;
            }
        }
        cout << "Worker not found!" << endl;
    } else if (choice == 'N' || choice == 'n') {
        string name;
        cout << "Enter Worker Name to delete: ";
        getline(cin, name);

        string searchName = name;
        transform(searchName.begin(), searchName.end(), searchName.begin(), ::tolower);

        int matches = 0;
        cout << "\nMatching workers:" << endl;
        for (int i = 0; i < count; i++) {
            string workerName = workers[i].name;
            transform(workerName.begin(), workerName.end(), workerName.begin(), ::tolower);
            if (workerName == searchName) {
                matches++;
                cout << "\nWorker " << matches << ":" << endl;
                displayWorker(workers[i]);
                cout << "------------------------" << endl;
            }
        }

        if (matches == 0) {
            cout << "No workers found with name '" << name << "'!" << endl;
            pressEnterToContinue();
            return;
        }

        if (matches == 1) {
            for (int i = 0; i < count; i++) {
                string workerName = workers[i].name;
                transform(workerName.begin(), workerName.end(), workerName.begin(), ::tolower);
                if (workerName == searchName) {
                    for (int j = i; j < count - 1; j++) {
                        workers[j] = workers[j + 1];
                    }
                    count--;
                    cout << "Worker deleted successfully!" << endl;
                    pressEnterToContinue();
                    return;
                }
            }
        } else {
            string id;
            cout << "Multiple workers found. Enter the ID of the worker to delete: ";
            cin >> id;
            for (int i = 0; i < count; i++) {
                if (workers[i].id == id) {
                    string workerName = workers[i].name;
                    transform(workerName.begin(), workerName.end(), workerName.begin(), ::tolower);
                    if (workerName == searchName) {
                        for (int j = i; j < count - 1; j++) {
                            workers[j] = workers[j + 1];
                        }
                        count--;
                        cout << "Worker deleted successfully!" << endl;
                        pressEnterToContinue();
                        return;
                    }
                }
            }
            cout << "No worker with ID '" << id << "' and name '" << name << "' found!" << endl;
        }
    } else {
        cout << "Invalid choice! Please enter 'I' or 'N'." << endl;
    }
    pressEnterToContinue();
}

void searchWorker(Worker workers[], int count) {
    if (count == 0) {
        cout << "\nNo workers in the database!" << endl;
        pressEnterToContinue();
        return;
    }

    char choice;
    do {
        cout << "\nSearch by (I)D or (N)ame, or 'e' to exit: ";
        cin >> choice;
        cin.ignore();

        if (choice == 'I' || choice == 'i') {
            string id;
            cout << "Enter Worker ID to search: ";
            cin >> id;
            
            for (int i = 0; i < count; i++) {
                if (workers[i].id == id) {
                    cout << "\nWorker found:" << endl;
                    displayWorker(workers[i]);
                    pressEnterToContinue();
                    return;
                }
            }
            cout << "Worker not found!" << endl;
            pressEnterToContinue();
        } else if (choice == 'N' || choice == 'n') {
            string name;
            cout << "Enter Worker Name to search: ";
            getline(cin, name);

            string searchName = name;
            transform(searchName.begin(), searchName.end(), searchName.begin(), ::tolower);

            int matches = 0;
            cout << "\nMatching workers:" << endl;
            for (int i = 0; i < count; i++) {
                string workerName = workers[i].name;
                transform(workerName.begin(), workerName.end(), workerName.begin(), ::tolower);
                if (workerName == searchName) {
                    matches++;
                    cout << "\nWorker " << matches << ":" << endl;
                    displayWorker(workers[i]);
                    cout << "------------------------" << endl;
                }
            }

            if (matches == 0) {
                cout << "No workers found with name '" << name << "'!" << endl;
            } else {
                cout << "Found " << matches << " worker(s) with name '" << name << "'." << endl;
                pressEnterToContinue();
                return;
            }
            pressEnterToContinue();
        } else if (choice == 'e' || choice == 'E') {
            pressEnterToContinue();
            return;
        } else {
            cout << "Invalid choice! Please enter 'I', 'N', or 'e'." << endl;
            pressEnterToContinue();
        }
    } while (true);
}

void sortById(Worker workers[], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (workers[j].id > workers[j + 1].id) {
                Worker temp = workers[j];
                workers[j] = workers[j + 1];
                workers[j + 1] = temp;
            }
        }
    }
}

void sortBySalary(Worker workers[], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            double salary1, tax1, afterTax1;
            double salary2, tax2, afterTax2;
            calculateSalary(workers[j], salary1, tax1, afterTax1);
            calculateSalary(workers[j + 1], salary2, tax2, afterTax2);
            if (afterTax1 < afterTax2) {
                Worker temp = workers[j];
                workers[j] = workers[j + 1];
                workers[j + 1] = temp;
            }
        }
    }
}

void displayAllWorkers(Worker workers[], int count, bool bySalary = false) {
    if (count == 0) {
        cout << "\nNo workers in the database!" << endl;
        pressEnterToContinue();
        return;
    }
    
    if (bySalary) {
        sortBySalary(workers, count);
        cout << "\nWorkers sorted by Salary (Descending):" << endl;
    } else {
        sortById(workers, count);
        cout << "\nWorkers sorted by ID (Ascending):" << endl;
    }
    
    for (int i = 0; i < count; i++) {
        cout << "\nWorker " << i + 1 << ":" << endl;
        displayWorker(workers[i]);
        cout << "------------------------" << endl;
    }
    pressEnterToContinue();
}

int displayMenu() {
    int choice;
    cout << "\n=========[Worker Management System]=========" << endl;
    cout << "[1]. Add New Worker(s)" << endl;
    cout << "[2]. Edit Worker Information" << endl;
    cout << "[3]. Delete Worker" << endl;
    cout << "[4]. Search Worker" << endl;
    cout << "[5]. Show All Workers (by ID)" << endl;
    cout << "[6]. Show All Workers (by Salary)" << endl;
    cout << "[7]. Exit" << endl;
    cout << "=============================================" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    return choice;
}

void printLogo() {
    HANDLE consoleColor = GetStdHandle(STD_OUTPUT_HANDLE);
    fstream file("logo.txt");
    if (!file.is_open()) {
        cout << "Error: Could not open logo.txt!" << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        SetConsoleTextAttribute(consoleColor, 4);
        cout << right << line << endl;
        Sleep(120);
    }
    file.close();
}

int main() {
    const int MAX_WORKERS = 100;
    Worker workers[MAX_WORKERS];
    int count = 0;
    int choice;
    printLogo();
    do {
        choice = displayMenu();
        system("cls");
        switch (choice) {
            case 1:
                addWorker(workers, count, MAX_WORKERS);
                break;
            case 2:
                editWorker(workers, count);
                break;
            case 3:
                deleteWorker(workers, count);
                break;
            case 4:
                searchWorker(workers, count);
                break;
            case 5:
                displayAllWorkers(workers, count);
                break;
            case 6:
                displayAllWorkers(workers, count, true);
                break;
            case 7:
                cout << "Exiting program..." << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
                pressEnterToContinue();
        }
    } while (choice != 7);
    
    return 0;
}