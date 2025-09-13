#include "admin.h"
#include <iomanip>

bool checkAdminLogin() {
    std::string pass;
    std::cout << "\nEnter Admin Password: ";
    std::getline(std::cin, pass);

    std::ifstream fin(ADMIN_PASS_FILE);
    std::string storedHash;
    if (fin.is_open()) {
        std::getline(fin, storedHash);
        fin.close();
    } else {
        // First-time setup
        storedHash = hashPassword("admin123");
        std::ofstream fout(ADMIN_PASS_FILE);
        fout << storedHash;
        fout.close();
    }

    if (hashPassword(pass) == storedHash) {
        std::cout << "✅ Login successful!\n";
        return true;
    }
    std::cout << "\033[1;31m❌ Wrong admin password.\033[0m\n";
    return false;
}

void adminLoginFlow() {
    if (!checkAdminLogin()) {
        pressEnterToContinue();
        return;
    }
    int ch = 0;
    do {
        clearScreen();
        std::cout << "\033[1;33m===== 🏫 ADMIN DASHBOARD =====\033[0m\n";
        std::cout << "1.  Add Student\n";
        std::cout << "2.  Display All Students\n";
        std::cout << "3.  Search Student\n";
        std::cout << "4.  Edit Student Details\n";
        std::cout << "5.  Delete Student\n";
        std::cout << "6.  Enter Marks\n";
        std::cout << "7.  Start Attendance Session\n";
        std::cout << "8.  Close Attendance Session\n";
        std::cout << "9.  View All Attendance Records\n";
        std::cout << "10. Change Admin Password\n";
        std::cout << "11. Logout\n";
        std::cout << "Enter your choice: ";
        if (!(std::cin >> ch)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (ch) {
            case 1: addStudentConsole(); break;
            case 2: displayStudentsTable(); break;
            case 3: searchStudentConsole(); break;
            case 4: editStudentConsole(); break;
            case 5: deleteStudentConsole(); break;
            case 6: enterMarksConsole(); break;
            case 7: startAttendanceSession(); break;
            case 8: closeAttendanceSession(); break;
            case 9: viewAllAttendanceRecords(); break;
            case 10: changeAdminPassword(); break;
            case 11: std::cout << "\033[1;32m✅ Admin logged out successfully.\033[0m\n"; break;
            default: std::cout << "\033[1;31m❌ Invalid choice. Please try again.\033[0m\n";
        }
        if (ch != 11) pressEnterToContinue();
    } while (ch != 11);
}

void changeAdminPassword() {
    std::string newPass;
    std::cout << "Enter new admin password: ";
    std::getline(std::cin, newPass);
    std::ofstream fout(ADMIN_PASS_FILE);
    fout << hashPassword(newPass);
    fout.close();
    std::cout << "\033[1;32m✅ Admin password updated.\033[0m\n";
}

void addStudentConsole() {
    Student s;
    std::cout << "Enter Roll No: ";
    std::cin >> s.rollNo;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (findStudentByRoll(s.rollNo) != -1) {
        std::cout << "\033[1;31m❌ Roll number already exists.\033[0m\n";
        return;
    }
    std::cout << "Enter Name: ";
    std::getline(std::cin, s.name);
    std::cout << "Set Password: ";
    std::string pass;
    std::getline(std::cin, pass);
    s.passwordHash = hashPassword(pass);
    std::cout << "Set Security Key (for forgot password): ";
    std::getline(std::cin, s.securityKey);

    s.marks1 = -1;
    s.marks2 = -1;
    s.attendanceCount = 0;
    students.push_back(s);
    saveStudents();
    std::cout << "\033[1;32m✅ Student added successfully.\033[0m\n";
}

void editStudentConsole() {
    std::cout << "Enter Roll to edit: ";
    int roll;
    std::cin >> roll;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    int idx = findStudentByRoll(roll);
    if (idx == -1) {
        std::cout << "\033[1;31m❌ Student not found.\033[0m\n";
        return;
    }
    std::cout << "Current Name: " << students[idx].name << "\nEnter new name (press Enter to keep old): ";
    std::string newName;
    std::getline(std::cin, newName);
    if (!newName.empty()) {
        students[idx].name = newName;
    }
    std::cout << "Change password? (y/n): ";
    char ch;
    std::cin >> ch;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (ch == 'y' || ch == 'Y') {
        std::cout << "Enter new password: ";
        std::string newPass;
        std::getline(std::cin, newPass);
        students[idx].passwordHash = hashPassword(newPass);
    }
    saveStudents();
    std::cout << "\033[1;32m✅ Student details updated.\033[0m\n";
}

void deleteStudentConsole() {
    std::cout << "Enter Roll to delete: ";
    int roll;
    std::cin >> roll;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    int idx = findStudentByRoll(roll);
    if (idx == -1) {
        std::cout << "\033[1;31m❌ Student not found.\033[0m\n";
        return;
    }
    std::cout << "\033[1;33m⚠️ Confirm deletion for student '" << students[idx].name << "'? (yes to confirm): \033[0m";
    std::string confirm;
    std::getline(std::cin, confirm);
    if (confirm == "yes") {
        students.erase(students.begin() + idx);
        saveStudents();
        std::cout << "\033[1;32m✅ Student deleted.\033[0m\n";
    } else {
        std::cout << "\033[1;31m❌ Deletion cancelled.\033[0m\n";
    }
}

void displayStudentsTable() {
    clearScreen();
    std::cout << "\033[1;33m===== 🧑‍🎓 ALL STUDENTS =====\033[0m\n";
    std::cout << std::left << std::setw(6) << "Roll" << " | "
              << std::setw(25) << "Name" << " | "
              << std::setw(7) << "Marks1" << " | "
              << std::setw(7) << "Marks2" << " | "
              << "Attendance\n";
    std::cout << "---------------------------------------------------------------\n";
    for (const auto& s : students) {
        std::cout << std::left << std::setw(6) << s.rollNo << " | "
                  << std::setw(25) << s.name << " | "
                  << std::setw(7) << (s.marks1 >= 0 ? std::to_string(static_cast<int>(s.marks1)) : "N/A") << " | "
                  << std::setw(7) << (s.marks2 >= 0 ? std::to_string(static_cast<int>(s.marks2)) : "N/A") << " | "
                  << std::setw(9) << s.attendanceCount << "\n";
    }
    std::cout << "---------------------------------------------------------------\n";
}

void searchStudentConsole() {
    std::cout << "Enter roll number or name substring: ";
    std::string query;
    std::getline(std::cin, query);
    std::cout << "\033[1;33m===== SEARCH RESULTS =====\033[0m\n";
    bool found = false;
    for (const auto& s : students) {
        std::string rollStr = std::to_string(s.rollNo);
        if (rollStr.find(query) != std::string::npos || s.name.find(query) != std::string::npos) {
            std::cout << s.rollNo << " | " << s.name << " | Attendance: " << s.attendanceCount << "\n";
            found = true;
        }
    }
    if (!found) {
        std::cout << "\033[1;31m❌ No matching students found.\033[0m\n";
    }
}

void enterMarksConsole() {
    std::cout << "Enter student roll number: ";
    int roll;
    std::cin >> roll;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    int idx = findStudentByRoll(roll);
    if (idx == -1) {
        std::cout << "\033[1;31m❌ Student not found.\033[0m\n";
        return;
    }
    std::cout << "Enter which sessional exam (1 or 2): ";
    int s;
    std::cin >> s;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter marks (0-100): ";
    float m;
    std::cin >> m;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (m < 0 || m > 100) {
        std::cout << "\033[1;31m❌ Invalid marks. Marks must be between 0 and 100.\033[0m\n";
        return;
    }

    if (s == 1) {
        students[idx].marks1 = m;
    } else if (s == 2) {
        students[idx].marks2 = m;
    } else {
        std::cout << "\033[1;31m❌ Invalid sessional choice. Please enter 1 or 2.\033[0m\n";
        return;
    }
    saveStudents();
    std::cout << "\033[1;32m✅ Marks saved successfully.\033[0m\n";
}

void startAttendanceSession() {
    std::string code = generate5DigitCode();
    std::ofstream fout(ATTENDANCE_PASS_FILE);
    if (!fout.is_open()) {
        std::cerr << "Error: Could not open attendance pass file.\n";
        return;
    }
    fout << code;
    fout.close();
    std::cout << "\033[1;32m✅ Attendance session started. Session code is: " << code << "\033[0m\n";
}

void closeAttendanceSession() {
    std::string today = getTodayDateStr();
    clearScreen();
    std::cout << "\033[1;33m===== 📊 ATTENDANCE LIST for " << today << " =====\033[0m\n";
    std::cout << std::left << std::setw(10) << "Roll" << std::setw(25) << "Name" << "Time" << "\n";
    std::cout << "--------------------------------------------\n";
    std::ifstream fin(ATTENDANCE_RECORD_FILE);
    bool any = false;
    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        auto parts = splitCSV(line);
        if (parts.size() >= 4 && parts[0] == today) {
            std::cout << std::left << std::setw(10) << parts[1] << std::setw(25) << parts[2] << parts[3] << "\n";
            any = true;
        }
    }
    fin.close();
    if (!any) std::cout << "\033[1;31mNo attendance marked today.\033[0m\n";

    // Clear session pass file
    std::ofstream fout(ATTENDANCE_PASS_FILE);
    fout.close();
    std::cout << "\n\033[1;31m❌ Attendance session closed.\033[0m\n";
}

void viewAllAttendanceRecords() {
    clearScreen();
    std::cout << "\033[1;33m===== 📈 ALL ATTENDANCE RECORDS =====\033[0m\n";
    std::cout << std::left << std::setw(12) << "Date" << " | "
              << std::setw(8) << "Time" << " | "
              << std::setw(6) << "Roll" << " | "
              << "Name\n";
    std::cout << "--------------------------------------------------\n";
    std::ifstream fin(ATTENDANCE_RECORD_FILE);
    if (!fin.is_open()) {
        std::cout << "❌ No attendance records yet.\n";
        return;
    }
    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        auto parts = splitCSV(line);
        if (parts.size() >= 4) {
            std::cout << std::left << std::setw(12) << parts[0] << " | "
                      << std::setw(8) << parts[3] << " | "
                      << std::setw(6) << parts[1] << " | "
                      << parts[2] << "\n";
        }
    }
    fin.close();
}