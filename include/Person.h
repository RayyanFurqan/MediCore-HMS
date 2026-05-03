#pragma once

//Person: abstract base for Patient, Doctor, Admin
class Person {
protected:
    int  id;
    char name[100];
    char password[50];
    char contact[15];
public:
    Person() {
        id = 0;
        name[0] = '\0';
        password[0] = '\0';
        contact[0] = '\0';
    }
    Person(int id, const char* name,const char* pass, const char* contact);
    int getId()  const;
    const char* getName()    const;
    const char* getPass()    const;
    const char* getContact() const;
    virtual void display()     const = 0;
    virtual void displayMenu() const = 0;
    virtual ~Person() {}
};