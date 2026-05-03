#pragma once
#include "Person.h"
#include <iostream>
using namespace std;

//Admin class
class Admin : public Person {
public:
    Admin() {
        id = 0;
        name[0] = '\0';
        password[0] = '\0';
        contact[0] = '\0';
    }
    Admin(int id, const char* name, const char* contact, const char* pass);
    void display() const override;
    void displayMenu() const override;
};
