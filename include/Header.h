#pragma once

class HospitalException {
protected:
    char message[200];
public:
    HospitalException(const char* msg);
    virtual const char* what() const;
    virtual ~HospitalException() {}
};

class FileNotFoundException : public HospitalException {
public:
    FileNotFoundException(const char* m = "Required file not found.")
        :HospitalException(m) {
    }
};
class InsufficientFundsException : public HospitalException {
public:
    InsufficientFundsException(const char* m = "Insufficient balance.")
        :HospitalException(m) {
    }
};

class InvalidInputException : public HospitalException {
public:
    InvalidInputException(const char* m = "Invalid input.")
        :HospitalException(m) {
    }
};
class SlotUnavailableException : public HospitalException {
public:
    SlotUnavailableException(const char* m = "Time slot already taken.")
        :HospitalException(m) {
    }
};
