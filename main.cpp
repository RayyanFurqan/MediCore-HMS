#include <SFML/Graphics.hpp>
#include "Storage.h"
#include "FileHandler.h"
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "HospitalException.h"
#include <iostream>
using namespace std;

//forward declare all screen functions
void showMain(sf::RenderWindow&, sf::Font&, Storage<Patient>&, Storage<Doctor>&, Storage<Appointment>&, Storage<Bill>&, Storage<Prescription>&, Admin*&, FileHandler&);

int main() {
    sf::RenderWindow win(sf::VideoMode(900, 650), "MediCore Hospital Management System");
    win.setFramerateLimit(60);

    sf::Font font;
    font.loadFromFile("Roboto-Regular.ttf");

    Storage<Patient> pts;
    Storage<Doctor> drs;
    Storage<Appointment> apts;
    Storage<Bill> bls;
    Storage<Prescription> prs;
    Admin* adm = nullptr;
    FileHandler fh;
    fh.loadPatients(pts);
    fh.loadDoctors(drs);
    fh.loadAdmin(adm);
    fh.loadAppointments(apts);
    fh.loadBills(bls);
    fh.loadPrescs(prs);

    showMain(win, font, pts, drs, apts, bls, prs, adm, fh);

    delete adm;
    return 0;
}