#include "Patient.h"
#include "Validator.h"
using namespace std;

Patient::Patient(int pid, const char* pname, int page, const char* pg, const char* pcon, const char* ppass, float pbal) : Person(pid, pname, ppass, pcon) {
	age = page;
	Validator::myCopy(gender, pg, 5);
	balance = pbal;
}

int Patient::getAge() const {
	return age;
}

const char* Patient::getGender() const {
	return gender;
}

float Patient::getBal() const {
	return balance;
}

void Patient::setBal(float b) {
	balance = b;
}

void Patient::display() const {
	cout << "ID: " << id << " Name: " << name << " Bal: " << balance << endl;
}

void Patient::displayMenu() const {
	cout << "Welcome, " << name << "  Balance: PKR " << balance << endl;
}

Patient& Patient::operator+=(float amt) {
	balance += amt;
	return *this;
}

Patient& Patient::operator-=(float amt) {
	balance -= amt;
	return *this;
}

bool Patient::operator==(const Patient& o) const {
	if (id == o.id) {
		return true;
	}
	else {
		return false;
	}
}

ostream& operator<<(ostream& os, const Patient& p) {
	os << "[" << p.id << "] " << p.name << "  PKR " << p.balance << endl;
	return os;
}

