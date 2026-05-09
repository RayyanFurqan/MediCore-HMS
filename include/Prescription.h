#pragma once
#include "Validator.h"
#include <iostream>
using namespace std;

//Prescription class
class Prescription {
private:
	int prid;
	int apid;
	int pid;
	int did;
	char date[15];
	char meds[500];
	char notes[300];
public:
	Prescription() {
		prid = 0;
		apid = 0;
		pid = 0;
		did = 0;
		date[0] = '\0';
		meds[0] = '\0';
		notes[0] = '\0';
	}
	Prescription(int pr, int a, int p, int d, const char* dt, const char* m, const char* n);
	int getId() const;
	int getApid() const;
	int getPid() const;
	int getDid() const;
	const char* getDate() const;
	const char* getMeds() const;
	const char* getNotes() const;
};
