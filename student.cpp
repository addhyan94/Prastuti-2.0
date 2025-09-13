#include "student.h"

void studentLoginFlow() {
    std::cout << "Enter Roll: ";
    int roll;
    if (!(std::cin >> roll)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\033[1;31m❌ Invalid input. Please enter a number.\033[0m\n";
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    int idx = findStudentByRoll(roll);
    if (idx == -1) {
        std::cout << "\033[1;31m❌ Roll number not found.\033[0m\n";
        return;
    }

    if (students[idx].passwordHash.empty()) {
        std::cout << "This is your first login. Please set a password and security key now.\n";
        std::string newPass, newKey;
        std::cout << "Set your password: ";
        std::getline(std::cin, newPass);
        std::cout << "Set a Security Key (for password reset): ";
        std::getline(std::cin, newKey);
        students[idx].passwordHash = hashPassword(newPass);
        students[idx].securityKey = newKey;
        saveStudents();
        std::cout << "\033[1;32m✅ Password and security key saved.\033[0m\n";
    }

    std::cout << "Enter Password: ";
    std::string pass;
    std::getline(std::cin, pass);

    if (hashPassword(pass) != students[idx].passwordHash) {
        std::cout << "\033[1;31m❌ Wrong password.\033[0m\n";
        return;
    }

    int ch = 0;
    do {
        clearScreen();
        std::cout << "\033[1;34m===== 🧑‍🎓 STUDENT DASHBOARD =====\033[0m\n";
        std::cout << "Welcome, " << students[idx].name << "!\n";
        std::cout << "1. View Marks\n";
        std::cout << "2. View Attendance Count\n";
        std::cout << "3. Mark Attendance\n";
        std::cout << "4. About Section\n";
        std::cout << "5. Logout\n";
        std::cout << "Enter choice: ";
        if (!(std::cin >> ch)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (ch) {
            case 1: {
                std::cout << "\n\033[1;33m===== Your Marks =====\033[0m\n";
                std::cout << "1st Sessional: " << (students[idx].marks1 >= 0 ? std::to_string(static_cast<int>(students[idx].marks1)) : "N/A") << "\n";
                std::cout << "2nd Sessional: " << (students[idx].marks2 >= 0 ? std::to_string(static_cast<int>(students[idx].marks2)) : "N/A") << "\n";
                float total = 0;
                int cnt = 0;
                if (students[idx].marks1 >= 0) {
                    total += students[idx].marks1;
                    cnt++;
                }
                if (students[idx].marks2 >= 0) {
                    total += students[idx].marks2;
                    cnt++;
                }
                if (cnt > 0) {
                    float average = total / cnt;
                    std::cout << "Average Marks: " << average << "\n";
                    if (average < 40) std::cout << "🙁 Don't be sad, keep trying!\n";
                    else std::cout << "😊 Great job! Keep it up!\n";
                } else {
                    std::cout << "No marks entered yet.\n";
                }
                break;
            }
            case 2:
                std::cout << "\n\033[1;33m===== Attendance =====\033[0m\n";
                std::cout << "Total Attendance Count: " << students[idx].attendanceCount << "\n";
                break;
            case 3:
                studentMarkAttendance(students[idx].rollNo);
                break;
            case 4:
                std::cout << "\n\033[1;35m===== About Section =====\033[0m\n";
                std::cout << "Company: EduNova\nTeam: Team Error\nMembers:\n";
                std::cout << "1. Addhyan Tiwari - Leader & Developer\n";
                std::cout << "2. Pratiksha Mishra - Documentation & Presentation\n";
                std::cout << "3. Vaibhav Kushwaha - Tester & Ideas\n";
                std::cout << "4. Manjari Pateriya - Research & Support\n";
                std::cout << "--------------------------------------------\n";
                break;
            case 5:
                std::cout << "\033[1;32m✅ Logging out...\033[0m\n";
                break;
            default:
                std::cout << "\033[1;31m❌ Invalid choice.\033[0m\n";
        }
        if (ch != 5) pressEnterToContinue();
    } while (ch != 5);
}

void studentForgotPassword() {
    std::cout << "Enter Roll: ";
    int roll;
    std::cin >> roll;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    int idx = findStudentByRoll(roll);
    if (idx == -1) {
        std::cout << "\033[1;31m❌ Roll number not found.\033[0m\n";
        return;
    }
    std::cout << "Enter Security Key: ";
    std::string key;
    std::getline(std::cin, key);
    if (key == students[idx].securityKey) {
        std::cout << "Enter new password: ";
        std::string newPass;
        std::getline(std::cin, newPass);
        students[idx].passwordHash = hashPassword(newPass);
        saveStudents();
        std::cout << "\033[1;32m✅ Password reset successfully.\033[0m\n";
    } else {
        std::cout << "\033[1;31m❌ Security key mismatch.\033[0m\n";
    }
}

void studentMarkAttendance(int roll) {
    std::ifstream finp(ATTENDANCE_PASS_FILE);
    std::string pass;
    if (finp.is_open()) {
        std::getline(finp, pass);
        finp.close();
    }
    if (pass.empty()) {
        std::cout << "\033[1;31m❌ Attendance session is not active right now.\033[0m\n";
        return;
    }
    int idx = findStudentByRoll(roll);
    if (idx == -1) {
        std::cout << "\033[1;31m❌ Student record not found.\033[0m\n";
        return;
    }

    // Check if already marked today
    std::string today = getTodayDateStr();
    std::ifstream frec(ATTENDANCE_RECORD_FILE);
    std::string line;
    while (std::getline(frec, line)) {
        if (line.empty()) continue;
        auto p = splitCSV(line);
        if (p.size() >= 4 && p[0] == today && p[1] == std::to_string(roll)) {
            std::cout << "\033[1;33m⚠️ You have already marked attendance today.\033[0m\n";
            frec.close();
            return;
        }
    }
    frec.close();

    int attempts = 0;
    std::string entered;
    while (attempts < 3) {
        std::cout << "Enter session code: ";
        std::getline(std::cin, entered);
        if (entered == pass) {
            // Append record with date, roll, name, and time
            std::ofstream fo(ATTENDANCE_RECORD_FILE, std::ios::app);
            fo << today << "," << roll << "," << students[idx].name << "," << getNowTimeStr() << "\n";
            fo.close();
            // Increment attendance count
            students[idx].attendanceCount += 1;
            saveStudents();
            std::cout << "\033[1;32m✅ Attendance marked successfully! Good job!\033[0m\n";
            return;
        } else {
            attempts++;
            std::cout << "\033[1;31m❌ Wrong code. Attempts left: " << (3 - attempts) << "\033[0m\n";
        }
    }
    std::cout << "\033[1;31m❌ Too many wrong attempts. You are blocked for this session.\033[0m\n";
}