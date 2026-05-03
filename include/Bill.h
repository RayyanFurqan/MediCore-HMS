#pragma once
#include "Validator.h"
#include <iostream>
using namespace std;

//Bill class
class Bill {
private:
    int bid;
    int pid;
    int apid;
    float amount;
    char status[20];
    char date[15];
public:
    Bill() {
        bid = 0;
        pid = 0;
        apid = 0;
        amount = 0.0f;
        status[0] = '\0';
        date[0] = '\0';
    }
    Bill(int b, int p, int a, float am, const char* st, const char* dt);
    int getId() const;
    int getPid() const;
    int getApid() const;
    float getAmount() const;
    const char* getStat() const;
    const char* getDate() const;
    void setStat(const char* s);
};
