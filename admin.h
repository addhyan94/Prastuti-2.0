#ifndef ADMIN_H
#define ADMIN_H

#include "utils.h"

// Admin specific functions
void adminLoginFlow();
void changeAdminPassword();
void addStudentConsole();
void editStudentConsole();
void deleteStudentConsole();
void displayStudentsTable();
void searchStudentConsole();
void enterMarksConsole();
void startAttendanceSession();
void closeAttendanceSession();
void viewAllAttendanceRecords();

#endif // ADMIN_H