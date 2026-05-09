#include "Appointment.h"
using namespace std;

Appointment::Appointment(int a, int p, int d, const char* dt, const char* sl, const char* st) {
	aid = a;
	pid = p;
	did = d;
	Validator::myCopy(date, dt, 15);
	Validator::myCopy(slot, sl, 10);
	Validator::myCopy(status, st, 20);
}

int Appointment::getId() const {
	return aid;
}
int Appointment::getPid() const {
	return pid;
}
int Appointment::getDid() const {
	return did;
}
const char* Appointment::getDate() const {
	return date;
}
const char* Appointment::getSlot() const {
	return slot;
}
const char* Appointment::getStat() const {
	return status;
}

void Appointment::setStat(const char* s) {
	Validator::myCopy(status, s, 20);
}

bool Appointment::operator==(const Appointment& o) const {
	if (Validator::myEq(status, "cancelled")) {
		return false;
	}
	if (Validator::myEq(o.status, "cancelled")) {
		return false;
	}
	if (did != o.did) {
		return false;
	}
	if (!Validator::myEq(date, o.date)) {
		return false;
	}
	if (!Validator::myEq(slot, o.slot)) {
		return false;
	}
	return true;
}

ostream& operator<<(ostream& os, const Appointment& a) {
	os << "Apt[" << a.aid << "] " << a.date << " " << a.slot << " " << a.status << endl;
	return os;
}
