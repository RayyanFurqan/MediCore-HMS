#pragma once
#include "Person.h"
#include <iostream>
using namespace std;

//Doctor class
class Doctor : public Person {
private:
	char spec[100];
	float fee;
public:
	Doctor() {
		id = 0;
		fee = 0.0f;
		name[0] = '\0';
		password[0] = '\0';
		contact[0] = '\0';
		spec[0] = '\0';
	}
	Doctor(int id, const char* name, const char* spec, float fee, const char* contact, const char* pass);
	const char* getSpec() const;
	float getFee() const;
	void display() const override;
	void displayMenu() const override;
	bool operator==(const Doctor& o) const;
	friend ostream& operator<<(ostream& os, const Doctor& d);
};
