#pragma once
#include "Validator.h"
#include <iostream>
using namespace std;

//Appointment class
class Appointment {
private:
    int aid;
    int pid;
    int did;
    char date[15];
    char slot[10];
    char status[20];
public:
    Appointment() {
        aid = 0;
        pid = 0;
        did = 0;
        date[0] = '\0';
        slot[0] = '\0';
        status[0] = '\0';
    }
    Appointment(int a, int p, int d, const char* dt, const char* sl, const char* st);
    int getId() const;
    int getPid() const;
    int getDid() const;
    const char* getDate() const;
    const char* getSlot() const;
    const char* getStat() const;
    void setStat(const char* s);
    bool operator==(const Appointment& o) const;
    friend ostream& operator<<(ostream& os, const Appointment& a);
};
