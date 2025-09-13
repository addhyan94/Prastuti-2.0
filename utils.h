#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <thread>
#include <ctime>
#include <sstream>

// Student struct
struct Student {
    int rollNo = 0;
    std::string name;
    std::string passwordHash;
    std::string securityKey;
    float marks1 = -1;
    float marks2 = -1;
    int attendanceCount = 0;
};

// Global variables
extern std::vector<Student> students;
static const std::string STUDENTS_FILE = "students.txt";
static const std::string ADMIN_PASS_FILE = "admin_pass.txt";
static const std::string ATTENDANCE_PASS_FILE = "attendance_pass.txt";
static const std::string ATTENDANCE_RECORD_FILE = "attendance_records.txt";

// Utility functions
void clearScreen();
std::string getTodayDateStr();
std::string getNowTimeStr();
std::string generate5DigitCode();
std::string hashPassword(const std::string& password);
void pressEnterToContinue();
void showLoading(const std::string& msg, int dots, int ms);
void showWelcomeScreen();

// Data management functions
void loadStudents();
void saveStudents();
int findStudentByRoll(int roll);
std::vector<std::string> splitCSV(const std::string& line);

#endif // UTILS_H