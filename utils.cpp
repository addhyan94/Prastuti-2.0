#include "utils.h"

std::vector<Student> students;

void clearScreen() {
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#else
    system("clear");
#endif
}

// ... baaki sab functions ...

void showLoading(const std::string& msg, int dots, int ms) {
    std::cout << "\n" << msg;
    for (int i = 0; i < dots; ++i) {
        std::cout << "." << std::flush;
        // std::this_thread::sleep_for line ko hata diya
    }
    std::cout << "\n";
}

void showWelcomeScreen() {
    clearScreen();
    std::cout << "\033[1;36m";
    std::cout << "=========================================================\n";
    std::cout << "                Welcome to EDUNOVA SYSTEMS\n";
    std::cout << "              Student Record Management System\n";
    std::cout << "=========================================================\n";
    std::cout << "Company: EduNova      Team: Team Error\n";
    std::cout << "---------------------------------------------------------\n";
    std::cout << "\n\"This project is not the end, it's the beginning of a smarter,\n more secure digital system.\"\n";
    std::cout << "\033[0m";
    // std::this_thread::sleep_for line ko hata diya
}

// ... baaki sab functions ...

/*#include "utils.h"
#include <thread>
#include <chrono>

std::vector<Student> students;

void clearScreen() {
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#else
    system("clear");
#endif
}

std::string getTodayDateStr() {
    time_t t = time(nullptr);
    tm* lt = localtime(&t);
    char buf[20];
    sprintf(buf, "%02d-%02d-%04d", lt->tm_mday, lt->tm_mon + 1, lt->tm_year + 1900);
    return std::string(buf);
}

std::string getNowTimeStr() {
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);
    char buf[20];
    strftime(buf, sizeof(buf), "%H:%M:%S", localTime);
    return std::string(buf);
}

std::string generate5DigitCode() {
    srand((unsigned)time(nullptr) + (unsigned)rand());
    int v = 10000 + rand() % 90000;
    return std::to_string(v);
}

// Dummy hash function for demonstration
std::string hashPassword(const std::string& password) {
    std::string hashed = "";
    for (char c : password) {
        hashed += std::to_string(static_cast<int>(c));
    }
    return hashed;
}

void pressEnterToContinue() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void showLoading(const std::string& msg, int dots, int ms) {
    std::cout << "\n" << msg;
    for (int i = 0; i < dots; ++i) {
        std::cout << "." << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
    std::cout << "\n";
}

void showWelcomeScreen() {
    clearScreen();
    std::cout << "\033[1;36m"; // Cyan color
    std::cout << "=========================================================\n";
    std::cout << "                Welcome to EDUNOVA SYSTEMS\n";
    std::cout << "              Student Record Management System\n";
    std::cout << "=========================================================\n";
    std::cout << "Company: EduNova      Team: Team Error\n";
    std::cout << "---------------------------------------------------------\n";
    std::cout << "\n\"This project is not the end, it’s the beginning of a smarter,\n more secure digital system.\"\n";
    std::cout << "\033[0m"; // Reset color
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
}

std::vector<std::string> splitCSV(const std::string& line) {
    std::vector<std::string> result;
    std::stringstream ss(line);
    std::string item;
    while (std::getline(ss, item, ',')) {
        result.push_back(item);
    }
    return result;
}

void loadStudents() {
    students.clear();
    std::ifstream fin(STUDENTS_FILE);
    if (!fin.is_open()) {
        std::cout << "Students file not found. A new one will be created.\n";
        return;
    }
    std::string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        auto parts = splitCSV(line);
        if (parts.size() < 7) continue;
        Student s;
        try {
            s.rollNo = std::stoi(parts[0]);
            s.name = parts[1];
            s.passwordHash = parts[2];
            s.securityKey = parts[3];
            s.marks1 = std::stof(parts[4]);
            s.marks2 = std::stof(parts[5]);
            s.attendanceCount = std::stoi(parts[6]);
            students.push_back(s);
        } catch (...) {
            std::cout << "Error reading data. Skipping corrupted line.\n";
            continue;
        }
    }
    fin.close();
}

void saveStudents() {
    std::ofstream fout(STUDENTS_FILE);
    if (!fout.is_open()) {
        std::cerr << "Error: Could not save student data.\n";
        return;
    }
    for (const auto& s : students) {
        fout << s.rollNo << "," << s.name << "," << s.passwordHash << "," << s.securityKey << ","
             << s.marks1 << "," << s.marks2 << "," << s.attendanceCount << "\n";
    }
    fout.close();
}

int findStudentByRoll(int roll) {
    for (size_t i = 0; i < students.size(); ++i) {
        if (students[i].rollNo == roll) return (int)i;
    }
    return -1;
}*/