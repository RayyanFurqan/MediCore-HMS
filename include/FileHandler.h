#pragma once
#include "Storage.h"
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include <iostream>
using namespace std;

//FileHandler - only class that touches files
class FileHandler {
private:
    void parseLine(char* line, char** fields, int& n);
    void rebuildLine(char* line, char** fields, int n);
public:
    void loadPatients(Storage<Patient>& s);
    void loadDoctors(Storage<Doctor>& s);
    void loadAdmin(Admin*& ptr);
    void loadAppointments(Storage<Appointment>& s);
    void loadBills(Storage<Bill>& s);
    void loadPrescs(Storage<Prescription>& s);

    void addPatient(const Patient& p);
    void addDoctor(const Doctor& d);
    void addAppt(const Appointment& a);
    void addBill(const Bill& b);
    void addPresc(const Prescription& p);
    void addSecLog(const char* role, int eid);
    void updField(const char* fname, int tid, int fi, const char* val);
    void delRecord(const char* fname, int tid);
};
