// Custom exception implementation
#include "HospitalException.h"
using namespace std;

// Copy error message
HospitalException::HospitalException(const char* msg) {
	int i = 0;
	while (msg[i] != '\0' && i < 199) {
		message[i] = msg[i];
		i++;
	}
	message[i] = '\0';
}

// Return error message
const char* HospitalException::what() const {
	return message;
}