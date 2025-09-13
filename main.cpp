#include "utils.h"
#include "admin.h"
#include "student.h"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    loadStudents();
    showLoading("Starting EDUNOVA System", 3, 200);
    showWelcomeScreen();

    int choice = 0;
    do {
        clearScreen();
        std::cout << "\033[1;32m===== EDUNOVA - Student Management System =====\033[0m\n";
        std::cout << "1. Admin (Teacher)\n";
        std::cout << "2. Student\n";
        std::cout << "3. Exit\n";
        std::cout << "Enter your choice: ";
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\033[1;31m❌ Invalid input. Please enter a number.\033[0m\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                adminLoginFlow();
                break;
            case 2:
                studentLoginFlow();
                break;
            case 3:
                std::cout << "Saving data and exiting...\n";
                saveStudents();
                break;
            default:
                std::cout << "\033[1;31m❌ Invalid choice. Please try again.\033[0m\n";
                pressEnterToContinue();
        }

    } while (choice != 3);

    return 0;
}