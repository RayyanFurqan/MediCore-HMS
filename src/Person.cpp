#include "Person.h"
#include "Validator.h"
using namespace std;

Person::Person(int pid, const char* pname, const char* ppass, const char* pcon) {
	id = pid;
	Validator::myCopy(name, pname, 100);
	Validator::myCopy(password, ppass, 50);
	Validator::myCopy(contact, pcon, 15);
}

int Person::getId() const {
	return id;
}

const char* Person::getName() const {
	return name;
}
const char* Person::getPass() const {
	return password;
}

const char* Person::getContact() const {
	return contact;
}