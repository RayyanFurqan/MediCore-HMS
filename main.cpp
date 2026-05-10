// Main entry point for MediCore Hospital Management System
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

// Forward declaration
void showMain(sf::RenderWindow&, sf::Font&, Storage<Patient>&, Storage<Doctor>&, Storage<Appointment>&, Storage<Bill>&, Storage<Prescription>&, Admin*&, FileHandler&);

// Main function
int main() {
	// Create window
	sf::RenderWindow win(sf::VideoMode(900, 650), "MediCore Hospital Management System");
	win.setFramerateLimit(60);

	// Load font
	sf::Font font;
	if (!font.loadFromFile("./Roboto-Regular.ttf")) {
		cout << "Error: Failed to load font file" << endl;
		return 0;
	}

	// Create storage for data
	Storage<Patient> pts;
	Storage<Doctor> drs;
	Storage<Appointment> apts;
	Storage<Bill> bls;
	Storage<Prescription> prs;
	Admin* adm = nullptr;
	FileHandler fh;

	// Load data from files
	try {
		fh.loadPatients(pts);
		fh.loadDoctors(drs);
		fh.loadAdmin(adm);
		fh.loadAppointments(apts);
		fh.loadBills(bls);
		fh.loadPrescs(prs);
	}
	catch (FileNotFoundException& e) {
		cout << "Error: " << e.what() << endl;
		return 0;
	}

	// Show main menu
	showMain(win, font, pts, drs, apts, bls, prs, adm, fh);

	// Clean up
	delete adm;
	return 0;
}