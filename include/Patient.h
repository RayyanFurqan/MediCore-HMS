#pragma once
#include "Person.h"
#include <iostream>
using namespace std;

//Patient class
class Patient : public Person {
private:
    int age;
    char gender[5];
    float balance;
public:
    Patient() {
        id = 0;
        age = 0;
        balance = 0.0f;
        name[0] = '\0';
        password[0] = '\0';
        contact[0] = '\0';
        gender[0] = '\0';
    }
    Patient(int id, const char* name, int age, const char* gender, const char* contact, const char* pass, float bal);
    int getAge() const;
    const char* getGender() const;
    float getBal() const;
    void setBal(float b);
    void display() const override;
    void displayMenu() const override;
    Patient& operator+=(float amt);
    Patient& operator-=(float amt);
    bool operator==(const Patient& o) const;
    friend ostream& operator<<(ostream& os, const Patient& p);
};
