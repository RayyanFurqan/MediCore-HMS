#include "FileHandler.h"
#include "Validator.h"
#include "HospitalException.h"
#include <fstream>
#include <ctime>
using namespace std;

//split csv line into fields array
void FileHandler::parseLine(char* line, char** fields, int& n) {
    int fi = 0;
    int ci = 0;
    for (int i = 0; line[i] != '\0' && line[i] != '\n' && line[i] != '\r'; i++) {
        if (line[i] == ',') {
            fields[fi][ci] = '\0';
            fi++;
            ci = 0;
        }
        else {
            if (ci < 299) {
                fields[fi][ci] = line[i];
                ci++;
            }
        }
    }
    fields[fi][ci] = '\0';
    n = fi + 1;
}

void FileHandler::rebuildLine(char* line, char** fields, int n) {
    int pos = 0;
    for (int f = 0; f < n; f++) {
        int c = 0;
        while (fields[f][c] != '\0') {
            line[pos] = fields[f][c];
            pos++;
            c++;
        }
        if (f < n - 1) {
            line[pos] = ',';
            pos++;
        }
    }
    line[pos] = '\0';
}

void FileHandler::loadPatients(Storage<Patient>& s) {
    ifstream f("data/patients.txt");
    if (!f.is_open()) {
        throw FileNotFoundException();
    }
    char* line = new char[500];
    bool first = true;
    while (f.getline(line, 500)) {
        if (first) {
            first = false;
            continue;
        }
        if (line[0] == '\0') {
            continue;
        }
        char** flds = new char*[20];
        for (int i = 0; i < 20; i++) {
            flds[i] = new char[300];
        }
        int n = 0;
        parseLine(line, flds, n);
        if (n >= 7) {
            Patient p(
                Validator::toInt(flds[0]),
                flds[1],
                Validator::toInt(flds[2]),
                flds[3],
                flds[4],
                flds[5],
                Validator::toFloat(flds[6])
            );
            s.add(p);
        }
        for (int i = 0; i < 20; i++) {
            delete[] flds[i];
        }
        delete[] flds;
    }
    delete[] line;
    f.close();
}

void FileHandler::loadDoctors(Storage<Doctor>& s) {
    ifstream f("data/doctors.txt");
    if (!f.is_open()) {
        throw FileNotFoundException();
    }
    char* line = new char[500];
    bool first = true;
    while (f.getline(line, 500)) {
        if (first) {
            first = false;
            continue;
        }
        if (line[0] == '\0') {
            continue;
        }
        char** flds = new char*[20];
        for (int i = 0; i < 20; i++) {
            flds[i] = new char[300];
        }
        int n = 0;
        parseLine(line, flds, n);
        if (n >= 6) {
            Doctor d(
                Validator::toInt(flds[0]),
                flds[1],
                flds[2],
                Validator::toFloat(flds[3]),
                flds[4],
                flds[5]
            );
            s.add(d);
        }
        for (int i = 0; i < 20; i++) {
            delete[] flds[i];
        }
        delete[] flds;
    }
    delete[] line;
    f.close();
}

void FileHandler::loadAdmin(Admin*& ptr) {
    ifstream f("data/admin.txt");
    if (!f.is_open()) {
        throw FileNotFoundException();
    }
    char* line = new char[500];
    bool first = true;
    while (f.getline(line, 500)) {
        if (first) {
            first = false;
            continue;
        }
        if (line[0] == '\0') {
            continue;
        }
        char** flds = new char*[20];
        for (int i = 0; i < 20; i++) {
            flds[i] = new char[300];
        }
        int n = 0;
        parseLine(line, flds, n);
        if (n >= 4) {
            ptr = new Admin(
                Validator::toInt(flds[0]),
                flds[1],
                flds[2],
                flds[3]
            );
        }
        for (int i = 0; i < 20; i++) {
            delete[] flds[i];
        }
        delete[] flds;
    }
    delete[] line;
    f.close();
}

void FileHandler::loadAppointments(Storage<Appointment>& s) {
    ifstream f("data/appointments.txt");
    if (!f.is_open()) {
        throw FileNotFoundException();
    }
    char* line = new char[500];
    bool first = true;
    while (f.getline(line, 500)) {
        if (first) {
            first = false;
            continue;
        }
        if (line[0] == '\0') {
            continue;
        }
        char** flds = new char*[20];
        for (int i = 0; i < 20; i++) {
            flds[i] = new char[300];
        }
        int n = 0;
        parseLine(line, flds, n);
        if (n >= 6) {
            Appointment a(
                Validator::toInt(flds[0]),
                Validator::toInt(flds[1]),
                Validator::toInt(flds[2]),
                flds[3],
                flds[4],
                flds[5]
            );
            s.add(a);
        }
        for (int i = 0; i < 20; i++) {
            delete[] flds[i];
        }
        delete[] flds;
    }
    delete[] line;
    f.close();
}

void FileHandler::loadBills(Storage<Bill>& s) {
    ifstream f("data/bills.txt");
    if (!f.is_open()) {
        throw FileNotFoundException();
    }
    char* line = new char[500];
    bool first = true;
    while (f.getline(line, 500)) {
        if (first) {
            first = false;
            continue;
        }
        if (line[0] == '\0') {
            continue;
        }
        char** flds = new char*[20];
        for (int i = 0; i < 20; i++) {
            flds[i] = new char[300];
        }
        int n = 0;
        parseLine(line, flds, n);
        if (n >= 6) {
            Bill b(
                Validator::toInt(flds[0]),
                Validator::toInt(flds[1]),
                Validator::toInt(flds[2]),
                Validator::toFloat(flds[3]),
                flds[4],
                flds[5]
            );
            s.add(b);
        }
        for (int i = 0; i < 20; i++) {
            delete[] flds[i];
        }
        delete[] flds;
    }
    delete[] line;
    f.close();
}

void FileHandler::loadPrescs(Storage<Prescription>& s) {
    ifstream f("data/prescriptions.txt");
    if (!f.is_open()) {
        throw FileNotFoundException();
    }
    char* line = new char[1200];
    bool first = true;
    while (f.getline(line, 1200)) {
        if (first) {
            first = false;
            continue;
        }
        if (line[0] == '\0') {
            continue;
        }
        char** flds = new char*[20];
        for (int i = 0; i < 20; i++) {
            flds[i] = new char[300];
        }
        int n = 0;
        parseLine(line, flds, n);
        if (n >= 7) {
            Prescription p(
                Validator::toInt(flds[0]),
                Validator::toInt(flds[1]),
                Validator::toInt(flds[2]),
                Validator::toInt(flds[3]),
                flds[4],
                flds[5],
                flds[6]
            );
            s.add(p);
        }
        for (int i = 0; i < 20; i++) {
            delete[] flds[i];
        }
        delete[] flds;
    }
    delete[] line;
    f.close();
}

void FileHandler::addPatient(const Patient& p) {
    ofstream f("data/patients.txt", ios::app);
    f << p.getId() << ","
      << p.getName() << ","
      << p.getAge() << ","
      << p.getGender() << ","
      << p.getContact() << ","
      << p.getPass() << ","
      << p.getBal() << endl;
    f.close();
}

void FileHandler::addDoctor(const Doctor& d) {
    ofstream f("data/doctors.txt", ios::app);
    f << d.getId() << ","
      << d.getName() << ","
      << d.getSpec() << ","
      << d.getFee() << ","
      << d.getContact() << ","
      << d.getPass() << endl;
    f.close();
}

void FileHandler::addAppt(const Appointment& a) {
    ofstream f("data/appointments.txt", ios::app);
    f << a.getId() << ","
      << a.getPid() << ","
      << a.getDid() << ","
      << a.getDate() << ","
      << a.getSlot() << ","
      << a.getStat() << endl;
    f.close();
}

void FileHandler::addBill(const Bill& b) {
    ofstream f("data/bills.txt", ios::app);
    f << b.getId() << ","
      << b.getPid() << ","
      << b.getApid() << ","
      << b.getAmount() << ","
      << b.getStat() << ","
      << b.getDate() << endl;
    f.close();
}

void FileHandler::addPresc(const Prescription& p) {
    ofstream f("data/prescriptions.txt", ios::app);
    f << p.getId() << ","
      << p.getApid() << ","
      << p.getPid() << ","
      << p.getDid() << ","
      << p.getDate() << ","
      << p.getMeds() << ","
      << p.getNotes() << endl;
    f.close();
}

void FileHandler::addSecLog(const char* role, int eid) {
    ofstream f("data/security_log.txt", ios::app);
    time_t t = time(nullptr);
    tm now;
    localtime_s(&now, &t);
    f << role << "," << eid << ","
      << (now.tm_mday < 10 ? "0" : "") << now.tm_mday << "-"
      << (now.tm_mon + 1 < 10 ? "0" : "") << (now.tm_mon + 1) << "-"
      << (now.tm_year + 1900) << " "
      << (now.tm_hour < 10 ? "0" : "") << now.tm_hour << ":"
      << (now.tm_min < 10 ? "0" : "") << now.tm_min << endl;
    f.close();
}

void FileHandler::updField(const char* fname, int tid, int fi, const char* val) {
    ifstream fin(fname);
    char** lines = new char*[200];
    for (int i = 0; i < 200; i++) {
        lines[i] = new char[500];
    }
    int lc = 0;
    while (fin.getline(lines[lc], 500)) {
        lc++;
    }
    fin.close();

    for (int i = 0; i < lc; i++) {
        char** flds = new char*[20];
        for (int j = 0; j < 20; j++) {
            flds[j] = new char[300];
        }
        int n = 0;
        char* cp = new char[500];
        Validator::myCopy(cp, lines[i], 500);
        parseLine(cp, flds, n);
        if (Validator::toInt(flds[0]) == tid) {
            Validator::myCopy(flds[fi], val, 300);
            rebuildLine(lines[i], flds, n);
        }
        delete[] cp;
        for (int j = 0; j < 20; j++) {
            delete[] flds[j];
        }
        delete[] flds;
    }

    ofstream fout(fname, ios::trunc);
    for (int i = 0; i < lc; i++) {
        fout << lines[i] << endl;
    }
    fout.close();

    for (int i = 0; i < 200; i++) {
        delete[] lines[i];
    }
    delete[] lines;
}

void FileHandler::delRecord(const char* fname, int tid) {
    ifstream fin(fname);
    char** lines = new char*[200];
    for (int i = 0; i < 200; i++) {
        lines[i] = new char[500];
    }
    int lc = 0;
    while (fin.getline(lines[lc], 500)) {
        lc++;
    }
    fin.close();

    ofstream fout(fname, ios::trunc);
    for (int i = 0; i < lc; i++) {
        char** flds = new char*[20];
        for (int j = 0; j < 20; j++) {
            flds[j] = new char[300];
        }
        int n = 0;
        char* cp = new char[500];
        Validator::myCopy(cp, lines[i], 500);
        parseLine(cp, flds, n);
        if (Validator::toInt(flds[0]) != tid) {
            fout << lines[i] << endl;
        }
        delete[] cp;
        for (int j = 0; j < 20; j++) {
            delete[] flds[j];
        }
        delete[] flds;
    }
    fout.close();

    for (int i = 0; i < 200; i++) {
        delete[] lines[i];
    }
    delete[] lines;
}
