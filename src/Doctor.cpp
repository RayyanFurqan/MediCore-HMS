#include "Doctor.h"
#include "Validator.h"
using namespace std;

Doctor::Doctor(int pid, const char* pname, const char* pspec, float pfee, const char* pcon, const char* ppass) : Person(pid, pname, ppass, pcon) {
	Validator::myCopy(spec, pspec, 100);
	fee = pfee;
}

const char* Doctor::getSpec() const {
	return spec;
}

float Doctor::getFee() const {
	return fee;
}

void Doctor::display() const {
	cout << "ID: " << id << " Name: " << name << " Spec: " << spec << " Fee: " << fee << endl;
}

void Doctor::displayMenu() const {
	cout << "Dr. " << name << " (" << spec << ")  Fee: PKR " << fee << endl;
}

bool Doctor::operator==(const Doctor& o) const {
	if (id == o.id) {
		return true;
	}
	else {
		return false;
	}
}

ostream& operator<<(ostream& os, const Doctor& d) {
	os << "[" << d.id << "] Dr. " << d.name << "  " << d.spec << "  PKR " << d.fee << endl;
	return os;
}
