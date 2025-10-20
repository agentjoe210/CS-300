
// Created by Phauxmlee on 10/12/2025

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>

using namespace std;

struct Course {
    string courseID;
    string courseName;
    vector<string> preReq;
};

class HashTable {
private:
    struct Node {
        Course course;
        Node* next;
        Node(const Course& c) : course(c), next(nullptr) {}
    };
    vector<Node*> table;
    size_t tableSize;

    size_t hashFunction(const string& courseID) const {
        size_t hash = 0;
        for (char c : courseID) {
            hash += c;
        }
        return hash % tableSize;
    }

public:
    HashTable(size_t size = 100) : tableSize(size) {
        table.resize(tableSize, nullptr);
    }

    void insert(const Course& course) {
        size_t index = hashFunction(course.courseID);
        Node* newNode = new Node(course);
        newNode->next = table[index];
        table[index] = newNode;
    }

    Course* find(const string& courseID) {
        size_t index = hashFunction(courseID);
        Node* current = table[index];
        while (current) {
            if (current->course.courseID == courseID) {
                return &current->course;
            }
            current = current->next;
        }
        return nullptr;
    }

    vector<Course> getAllCourses() const {
        vector<Course> courses;
        for (const auto& node : table) {
            Node* current = node;
            while (current) {
                courses.push_back(current->course);
                current = current->next;
            }
        }
        return courses;
    }

    ~HashTable() {
        for (auto& node : table) {
            while (node) {
                Node* temp = node;
                node = node->next;
                delete temp;
            }
        }
    }
};

void loadFile(HashTable& hashTable) {
    string fileName;
    cout << "Please enter the file name: ";
    getline(cin, fileName);

    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Error: File '" << fileName << "' could not be opened.\n";
        return;
    }

    string line;
    bool hasData = false;
    while (getline(file, line)) {
        if (line.empty()) continue; // Skip empty lines
        stringstream ss(line);
        string courseID, courseName, prereq;
        vector<string> preReq;

        // Read courseID and courseName, check for validity
        if (getline(ss, courseID, ',') && getline(ss, courseName, ',')) {
            if (courseID.empty() || courseName.empty()) continue; // Skip invalid rows
            while (getline(ss, prereq, ',')) {
                if (!prereq.empty()) { // Ignore empty fields
                    preReq.push_back(prereq);
                }
            }
            Course course{courseID, courseName, preReq};
            hashTable.insert(course);
            hasData = true;
        }
    }

    if (!hasData) {
        cout << "No valid course data found in file.\n";
    } else {
        cout << "File loaded successfully.\n";
    }
    file.close();
}

void printCourseList(HashTable& hashTable) {
    vector<Course> courses = hashTable.getAllCourses();
    if (courses.empty()) {
        cout << "No courses loaded.\n";
        return;
    }

    sort(courses.begin(), courses.end(), [](const Course& a, const Course& b) {
        return a.courseID < b.courseID;
    });

    cout << "\nComputer Science and Math Course List:\n";
    cout << "_____________________________________________________\n";
    for (const auto& course : courses) {
        if (course.courseID.find("CSCI") == 0 || course.courseID.find("MATH") == 0) {
            cout << course.courseID << ": " << course.courseName << "\n";
        }
    }
    cout << "_____________________________________________________\n";
}

void printCourseInfo(HashTable& hashTable) {
    string courseID;
    cout << "Please enter the course ID: ";
    getline(cin, courseID);

    if (courseID.empty()) {
        cout << "Error: Course ID cannot be empty.\n";
        return;
    }

    Course* course = hashTable.find(courseID);
    if (!course) {
        cout << "Error: Course '" << courseID << "' not found.\n";
        return;
    }

    cout << "\nCourse Information:\n";
    cout << "_____________________________________________________\n";
    cout << "Course: " << course->courseID << " - " << course->courseName << "\n";
    cout << "Prerequisites: ";
    if (course->preReq.empty()) {
        cout << "None\n";
    } else {
        for (size_t i = 0; i < course->preReq.size(); ++i) {
            cout << course->preReq[i];
            Course* prereq = hashTable.find(course->preReq[i]);
            if (prereq) {
                cout << " (" << prereq->courseName << ")";
            }
            if (i < course->preReq.size() - 1) {
                cout << ", ";
            }
        }
        cout << "\n";
    }
    cout << "_____________________________________________________\n";
}

void displayMenu(HashTable& hashTable) { //Switch menu for ease of inputing into Main()
    int choice = 0;
    while (choice != 9) {
        cout << "\nGood afternoon! How may I help you today?\n";
        cout << "1) Load File into data source\n";
        cout << "2) Print ordered courses\n";
        cout << "3) Print single course information\n";
        cout << "9) Exit application\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {  // Helps clear input errors and infinite loops
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore();

        switch (choice) {
            case 1:
                loadFile(hashTable);
                break;
            case 2:
                printCourseList(hashTable);
                break;
            case 3:
                printCourseInfo(hashTable);
                break;
            case 9:
                cout << "Exiting program.\n";
                break;
            default:
                cout << "Invalid choice. Please select 1, 2, 3, or 9.\n";
        }
    }
}

int main() {
    HashTable hashTable;
    displayMenu(hashTable);
    return 0;
}