#include <SFML/Graphics.hpp>
#include "Screens.h"
#include "Storage.h"
#include "FileHandler.h"
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "Validator.h"
#include <iostream>
using namespace std;

//forward declare patient/doctor/admin menu screens
void showPatientMenu(sf::RenderWindow&, sf::Font&, Patient*, Storage<Doctor>&, Storage<Appointment>&, Storage<Bill>&, Storage<Prescription>&, FileHandler&);
void showDoctorMenu(sf::RenderWindow&, sf::Font&, Doctor*, Storage<Patient>&, Storage<Appointment>&, Storage<Prescription>&, FileHandler&);
void showAdminMenu(sf::RenderWindow&, sf::Font&, Admin*, Storage<Patient>&, Storage<Doctor>&, Storage<Appointment>&, Storage<Bill>&, Storage<Prescription>&, FileHandler&);

void showMain(sf::RenderWindow& win, sf::Font& font, Storage<Patient>& pts, Storage<Doctor>& drs, Storage<Appointment>& apts, Storage<Bill>& bls, Storage<Prescription>& prs, Admin*& adm, FileHandler& fh) {
	while (win.isOpen()) {
		sf::Event e;
		while (win.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				win.close();
			}
			if (isClick(e, 330, 220, 240, 50)) {
				//patient login
				char* idBuf = new char[20];
				char* passBuf = new char[50];
				idBuf[0] = '\0';
				passBuf[0] = '\0';
				int idLen = 0, pLen = 0;
				bool idFoc = true;
				char* err = new char[100];
				err[0] = '\0';
				int tries = 0;
				bool done = false;
				while (win.isOpen() && !done) {
					sf::Event ev;
					while (win.pollEvent(ev)) {
						if (ev.type == sf::Event::Closed) {
							win.close();
						}
						if (idFoc) {
							typeChar(ev, idBuf, 20, idLen);
						}
						else {
							typeChar(ev, passBuf, 50, pLen);
						}
						if (ev.type == sf::Event::KeyPressed) {
							if (ev.key.code == sf::Keyboard::Tab) {
								idFoc = !idFoc;
							}
						}
						if (isClick(ev, 330, 420, 240, 45)) {
							int pid = Validator::toInt(idBuf);
							Patient* p = pts.findById(pid);
							if (p != nullptr && Validator::myEq(p->getPass(), passBuf)) {
								showPatientMenu(win, font, p, drs, apts, bls, prs, fh);
								done = true;
							}
							else {
								tries++;
								if (tries >= 3) {
									Validator::myCopy(err, "Account locked. Contact admin.", 100);
									fh.addSecLog("Patient", pid, "locked");
									done = true;
								}
								else {
									Validator::myCopy(err, "Wrong ID or password.", 100);
								}
							}
						}
						if (isClick(ev, 330, 480, 240, 40)) {
							done = true;
						}
					}
					win.clear(sf::Color(18, 32, 65));
					drawTxt(win, "Patient Login", 350, 90, font, 28, sf::Color::White);
					drawTxt(win, "Patient ID:", 200, 190, font, 15, sf::Color(180, 200, 255));
					drawBox(win, 200, 215, 500, 40, idBuf, font, idFoc);
					drawTxt(win, "Password:", 200, 275, font, 15, sf::Color(180, 200, 255));
					drawBox(win, 200, 300, 500, 40, passBuf, font, !idFoc);
					drawTxt(win, "Press Tab to switch field", 200, 352, font, 12, sf::Color(130, 150, 180));
					drawBtn(win, 330, 420, 240, 45, "Login", font, sf::Color(30, 80, 160), sf::Color::White);
					drawBtn(win, 330, 480, 240, 40, "Back", font, sf::Color(60, 60, 80), sf::Color::White);
					if (err[0] != '\0') {
						drawTxt(win, err, 200, 535, font, 14, sf::Color(255, 100, 100));
					}
					win.display();
				}
				delete[] idBuf;
				delete[] passBuf;
				delete[] err;
			}
			if (isClick(e, 330, 295, 240, 50)) {
				// DOCTOR LOGIN
				char idBuf[20] = "";
				char passBuf[50] = "";
				int idLen = 0, pLen = 0;
				bool idFoc = true;
				char err[100] = "";
				bool done = false;
				int tries = 0;

				while (win.isOpen() && !done) {
					sf::Event ev;
					while (win.pollEvent(ev)) {
						if (ev.type == sf::Event::Closed) {
							win.close();
						}
						if (idFoc) {
							typeChar(ev, idBuf, 20, idLen);
						}
						else {
							typeChar(ev, passBuf, 50, pLen);
						}
						if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Tab) {
							idFoc = !idFoc;
						}

						if (isClick(ev, 330, 420, 240, 45)) {
							if (tries >= 3) {
								Validator::myCopy(err, "Account locked. Contact admin.", 100);
								fh.addSecLog("Doctor", Validator::toInt(idBuf), "locked");
								done = true;
								continue;
							}

							int did = Validator::toInt(idBuf);
							Doctor* d = drs.findById(did);

							if (d != nullptr && Validator::myEq(d->getPass(), passBuf)) {
								showDoctorMenu(win, font, d, pts, apts, prs, fh);
								done = true;
							}
							else {
								tries++;
								if (tries >= 3) {
									Validator::myCopy(err, "Account locked. Contact admin.", 100);
									fh.addSecLog("Doctor", did, "locked");
								}
								else {
									Validator::myCopy(err, "Wrong ID or password.", 100);
								}
							}
						}
						if (isClick(ev, 330, 480, 240, 40)) {
							done = true;
						}
					}
					win.clear(sf::Color(18, 32, 65));
					drawTxt(win, "Doctor Login", 355, 90, font, 28, sf::Color::White);
					drawTxt(win, "Doctor ID:", 200, 190, font, 15, sf::Color(180, 200, 255));
					drawBox(win, 200, 215, 500, 40, idBuf, font, idFoc);
					drawTxt(win, "Password:", 200, 275, font, 15, sf::Color(180, 200, 255));
					drawBox(win, 200, 300, 500, 40, passBuf, font, !idFoc);
					drawBtn(win, 330, 420, 240, 45, "Login", font, sf::Color(30, 80, 160), sf::Color::White);
					drawBtn(win, 330, 480, 240, 40, "Back", font, sf::Color(60, 60, 80), sf::Color::White);
					if (err[0] != '\0') {
						drawTxt(win, err, 200, 535, font, 14, sf::Color(255, 100, 100));
					}
					win.display();
				}
			}
			if (isClick(e, 330, 370, 240, 50)) {
				// ADMIN LOGIN
				char idBuf[20] = "";
				char passBuf[50] = "";
				int idLen = 0, pLen = 0;
				bool idFoc = true;
				char err[100] = "";
				bool done = false;
				int tries = 0;

				while (win.isOpen() && !done) {
					sf::Event ev;
					while (win.pollEvent(ev)) {
						if (ev.type == sf::Event::Closed) {
							win.close();
						}
						if (idFoc) {
							typeChar(ev, idBuf, 20, idLen);
						}
						else {
							typeChar(ev, passBuf, 50, pLen);
						}
						if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Tab) {
							idFoc = !idFoc;
						}

						if (isClick(ev, 330, 420, 240, 45)) {
							if (tries >= 3) {
								Validator::myCopy(err, "Account locked. Contact admin.", 100);
								fh.addSecLog("Admin", Validator::toInt(idBuf), "locked");
								done = true;
								continue;
							}

							int aid = Validator::toInt(idBuf);

							if (adm != nullptr && adm->getId() == aid && Validator::myEq(adm->getPass(), passBuf)) {
								showAdminMenu(win, font, adm, pts, drs, apts, bls, prs, fh);
								done = true;
							}
							else {
								tries++;
								if (tries >= 3) {
									Validator::myCopy(err, "Account locked. Contact admin.", 100);
									fh.addSecLog("Admin", aid, "locked");
								}
								else {
									Validator::myCopy(err, "Wrong ID or password.", 100);
								}
							}
						}
						if (isClick(ev, 330, 480, 240, 40)) {
							done = true;
						}
					}
					win.clear(sf::Color(18, 32, 65));
					drawTxt(win, "Admin Login", 360, 90, font, 28, sf::Color::White);
					drawTxt(win, "Admin ID:", 200, 190, font, 15, sf::Color(180, 200, 255));
					drawBox(win, 200, 215, 500, 40, idBuf, font, idFoc);
					drawTxt(win, "Password:", 200, 275, font, 15, sf::Color(180, 200, 255));
					drawBox(win, 200, 300, 500, 40, passBuf, font, !idFoc);
					drawBtn(win, 330, 420, 240, 45, "Login", font, sf::Color(30, 80, 160), sf::Color::White);
					drawBtn(win, 330, 480, 240, 40, "Back", font, sf::Color(60, 60, 80), sf::Color::White);
					if (err[0] != '\0') {
						drawTxt(win, err, 200, 535, font, 14, sf::Color(255, 100, 100));
					}
					win.display();
				}
			}
			if (isClick(e, 330, 455, 240, 45)) {
				win.close();
			}
		}

		win.clear(sf::Color(18, 32, 65));
		drawTxt(win, "MediCore Hospital Management System", 230, 80, font, 26, sf::Color::White);
		drawTxt(win, "Select your role to login:", 320, 155, font, 15, sf::Color(180, 200, 230));
		drawBtn(win, 330, 220, 240, 50, "Patient Login", font, sf::Color(20, 70, 150), sf::Color::White);
		drawBtn(win, 330, 295, 240, 50, "Doctor Login", font, sf::Color(20, 100, 80), sf::Color::White);
		drawBtn(win, 330, 370, 240, 50, "Admin Login", font, sf::Color(100, 50, 20), sf::Color::White);
		drawBtn(win, 330, 455, 240, 45, "Exit", font, sf::Color(80, 20, 20), sf::Color::White);
		win.display();
	}
}