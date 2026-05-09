#include "Prescription.h"
using namespace std;

Prescription::Prescription(int pr, int a, int p, int d, const char* dt, const char* m, const char* n) {
	prid = pr;
	apid = a;
	pid = p;
	did = d;
	Validator::myCopy(date, dt, 15);
	Validator::myCopy(meds, m, 500);
	Validator::myCopy(notes, n, 300);
}

int Prescription::getId() const { return prid; }
int Prescription::getApid() const { return apid; }
int Prescription::getPid() const { return pid; }
int Prescription::getDid() const { return did; }
const char* Prescription::getDate() const { return date; }
const char* Prescription::getMeds() const { return meds; }
const char* Prescription::getNotes() const { return notes; }
