#include "Bill.h"
using namespace std;

Bill::Bill(int b, int p, int a, float am, const char* st, const char* dt) {
    bid = b;
    pid = p;
    apid = a;
    amount = am;
    Validator::myCopy(status, st, 20);
    Validator::myCopy(date, dt, 15);
}

int Bill::getId() const { return bid; }
int Bill::getPid() const { return pid; }
int Bill::getApid() const { return apid; }
float Bill::getAmount() const { return amount; }
const char* Bill::getStat() const { return status; }
const char* Bill::getDate() const { return date; }

void Bill::setStat(const char* s) {
    Validator::myCopy(status, s, 20);
}
