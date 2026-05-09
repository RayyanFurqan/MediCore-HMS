#include "Admin.h"
using namespace std;

Admin::Admin(int pid, const char* pname, const char* pcon, const char* ppass) : Person(pid, pname, ppass, pcon) {
}

void Admin::display() const {
	cout << "ID: " << id << " Name: " << name << " Contact: " << contact << endl;
}

void Admin::displayMenu() const {
	cout << "Admin Panel - " << name << endl;
}
