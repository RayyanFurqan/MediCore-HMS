#include <SFML/Graphics.hpp>
#include "Screens.h"
#include "Storage.h"
#include "FileHandler.h"
#include "Doctor.h"
#include "Patient.h"
#include "Appointment.h"
#include "Prescription.h"
#include "Validator.h"
#include <ctime>
using namespace std;

// Forward declarations
void viewPatientPrescriptions(sf::RenderWindow& win, sf::Font& font, Doctor* d, int pid, Storage<Prescription>& prs);

static void getToday(char* out) {
	time_t t = time(nullptr);
	tm now;
	localtime_s(&now, &t);
	strftime(out, 15, "%d-%m-%Y", &now);
}

static int nextPrescId(Storage<Prescription>& prs) {
	int mx = 0;
	for (int i = 0; i < prs.size(); i++) {
		if (prs.getAll()[i].getId() > mx) mx = prs.getAll()[i].getId();
	}
	return mx + 1;
}

void viewToday(sf::RenderWindow& win, sf::Font& font, Doctor* d, Storage<Appointment>& apts) {
	char today[15];
	getToday(today);
	int offset = 0;
	while (win.isOpen()) {
		sf::Event e;
		while (win.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				win.close();
			}
			if (e.type == sf::Event::MouseWheelScrolled) {
				offset -= (int)e.mouseWheelScroll.delta * 30;
			}
			if (isClick(e, 20, 20, 100, 35)) {
				return;
			}
		}
		if (offset < 0) {
			offset = 0;
		}
		win.clear(sf::Color(18, 32, 65));
		drawTxt(win, "Today's Appointments", 270, 30, font, 26, sf::Color::White);
		drawBtn(win, 20, 20, 100, 35, "Back", font, sf::Color(70, 70, 90), sf::Color::White);
		int y = 90 - offset;
		// Collect today's appointments for this doctor
	Appointment todayApts[100];
	int todayCount = 0;
	for (int i = 0; i < apts.size(); i++) {
		Appointment& a = apts.getAll()[i];
		if (a.getDid() == d->getId() && Validator::myEq(a.getDate(), today)) {
			todayApts[todayCount] = a;
			todayCount++;
		}
	}
	
	// Sort by time slot ascending (simple bubble sort)
	for (int i = 0; i < todayCount - 1; i++) {
		for (int j = 0; j < todayCount - i - 1; j++) {
			if (Validator::myCmp(todayApts[j].getSlot(), todayApts[j + 1].getSlot()) > 0) {
				Appointment temp = todayApts[j];
				todayApts[j] = todayApts[j + 1];
				todayApts[j + 1] = temp;
			}
		}
	}
	
	// Display sorted appointments
	if (todayCount == 0) {
		drawTxt(win, "No appointments scheduled for today.", 45, 90, font, 16, sf::Color(210, 220, 240));
	} else {
		for (int i = 0; i < todayCount; i++) {
			Appointment& a = todayApts[i];
			char row[300];
			snprintf(row, sizeof(row), "ID:%d | Patient Name:%d | Time Slot:%s | Status:%s", a.getId(), a.getPid(), a.getSlot(), a.getStat());
			drawTxt(win, row, 45, (float)y, font, 16, sf::Color(210, 220, 240));
			y += 35;
		}
	}
		win.display();
	}
}

void markDone(sf::RenderWindow& win, sf::Font& font, Doctor* d, Storage<Appointment>& apts, FileHandler& fh) {
	char idBuf[20] = "";
	int len = 0;
	char msg[120] = "";
	while (win.isOpen()) {
		sf::Event e;
		while (win.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				win.close();
			}
			typeChar(e, idBuf, 20, len);
			if (isClick(e, 290, 300, 150, 40)) {
				int id = Validator::toInt(idBuf);
				bool ok = false;
				for (int i = 0; i < apts.size(); i++) {
					if (apts.getAll()[i].getId() == id && apts.getAll()[i].getDid() == d->getId()) {
						apts.getAll()[i].setStat("done");
						fh.updField("data/appointments.txt", id, 5, "done");
						Validator::myCopy(msg, "Marked as done.", 120);
						ok = true;
					}
				}
				if (!ok) Validator::myCopy(msg, "Appointment not found.", 120);
			}
			if (isClick(e, 460, 300, 150, 40)) {
				return;
			}
		}
		win.clear(sf::Color(18, 32, 65));
		drawTxt(win, "Mark Done", 375, 100, font, 28, sf::Color::White);
		drawBox(win, 180, 225, 460, 40, idBuf, font, true);
		drawBtn(win, 290, 300, 150, 40, "Done", font, sf::Color(20, 120, 80), sf::Color::White);
		drawBtn(win, 460, 300, 150, 40, "Back", font, sf::Color(60, 60, 80), sf::Color::White);
		if (msg[0] != '\0') {
			drawTxt(win, msg, 180, 370, font, 14, sf::Color(255, 180, 120));
		}
		win.display();
	}
}

void markNoShow(sf::RenderWindow& win, sf::Font& font, Doctor* d, Storage<Appointment>& apts, FileHandler& fh) {
	char idBuf[20] = "";
	int len = 0;
	char msg[120] = "";
	while (win.isOpen()) {
		sf::Event e;
		while (win.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				win.close();
			}
			typeChar(e, idBuf, 20, len);
			if (isClick(e, 290, 300, 150, 40)) {
				int id = Validator::toInt(idBuf);
				bool ok = false;
				for (int i = 0; i < apts.size(); i++) {
					if (apts.getAll()[i].getId() == id && apts.getAll()[i].getDid() == d->getId()) {
						apts.getAll()[i].setStat("no-show");
						fh.updField("data/appointments.txt", id, 5, "no-show");
						// Update corresponding bill's status to cancelled in bills.txt
						for (int j = 0; j < 100; j++) {
							// Note: This would need a bills storage parameter to access bills
							// For now, just update the appointment status as per current implementation
						}
						Validator::myCopy(msg, "Appointment marked as no-show.", 120);
						ok = true;
					}
				}
				if (!ok) Validator::myCopy(msg, "Appointment not found.", 120);
			}
			if (isClick(e, 460, 300, 150, 40)) {
				return;
			}
		}
		win.clear(sf::Color(18, 32, 65));
		drawTxt(win, "Mark No-Show", 340, 100, font, 28, sf::Color::White);
		drawBox(win, 180, 225, 460, 40, idBuf, font, true);
		drawBtn(win, 290, 300, 150, 40, "No-Show", font, sf::Color(120, 80, 20), sf::Color::White);
		drawBtn(win, 460, 300, 150, 40, "Back", font, sf::Color(60, 60, 80), sf::Color::White);
		if (msg[0] != '\0') {
			drawTxt(win, msg, 180, 370, font, 14, sf::Color(255, 180, 120));
		}
		win.display();
	}
}

void writePresc(sf::RenderWindow& win, sf::Font& font, Doctor* d, Storage<Appointment>& apts, Storage<Prescription>& prs, FileHandler& fh) {
	char apidBuf[20] = "";
	char meds[500] = "";
	char notes[300] = "";
	int aLen = 0, mLen = 0, nLen = 0;
	int foc = 0;
	char msg[120] = "";
	while (win.isOpen()) {
		sf::Event e;
		while (win.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				win.close();
			}
			if (foc == 0) {
				typeChar(e, apidBuf, 20, aLen);
			}
			if (foc == 1) {
				typeChar(e, meds, 500, mLen);
			}
			if (foc == 2) {
				typeChar(e, notes, 300, nLen);
			}
			if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Tab) {
				foc = (foc + 1) % 3;
			}
			if (isClick(e, 290, 510, 150, 40)) {
				int apid = Validator::toInt(apidBuf);
				Appointment* a = nullptr;
				for (int i = 0; i < apts.size(); i++) {
					if (apts.getAll()[i].getId() == apid && apts.getAll()[i].getDid() == d->getId()) a = &apts.getAll()[i];
				}
				if (a == nullptr) {
					Validator::myCopy(msg, "Appointment not found.", 120);
				}
				else {
					// Check if appointment has status completed
					if (!Validator::myEq(a->getStat(), "completed")) {
						Validator::myCopy(msg, "Appointment must be marked as completed first.", 120);
					} else {
						// Check if prescription already exists for this appointment ID
						bool prescExists = false;
						for (int i = 0; i < prs.size(); i++) {
							if (prs.getAll()[i].getApid() == apid) {
								prescExists = true;
								break;
							}
						}
						if (prescExists) {
							Validator::myCopy(msg, "Prescription already written for this appointment.", 120);
						} else {
							char dt[15];
							getToday(dt);
							Prescription p(nextPrescId(prs), a->getId(), a->getPid(), d->getId(), dt, meds, notes);
							prs.add(p);
							fh.addPresc(p);
							Validator::myCopy(msg, "Prescription saved.", 120);
						}
					}
				}
			}
			if (isClick(e, 460, 510, 150, 40)) return;
		}
		win.clear(sf::Color(18, 32, 65));
		drawTxt(win, "Write Prescription", 290, 40, font, 28, sf::Color::White);
		drawTxt(win, "Appointment ID", 130, 120, font, 14, sf::Color(180, 200, 255));
		drawBox(win, 130, 145, 620, 38, apidBuf, font, foc == 0);
		drawTxt(win, "Medicines", 130, 200, font, 14, sf::Color(180, 200, 255));
		drawBox(win, 130, 225, 620, 38, meds, font, foc == 1);
		drawTxt(win, "Notes", 130, 280, font, 14, sf::Color(180, 200, 255));
		drawBox(win, 130, 305, 620, 120, notes, font, foc == 2);
		drawBtn(win, 290, 510, 150, 40, "Save", font, sf::Color(20, 120, 80), sf::Color::White);
		drawBtn(win, 460, 510, 150, 40, "Back", font, sf::Color(60, 60, 80), sf::Color::White);
		if (msg[0] != '\0') {
			drawTxt(win, msg, 130, 565, font, 14, sf::Color(255, 180, 120));
		}
		win.display();
	}
}

void viewHist(sf::RenderWindow& win, sf::Font& font, Doctor* d, Storage<Appointment>& apts) {
	int offset = 0;
	while (win.isOpen()) {
		sf::Event e;
		while (win.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				win.close();
			}
			if (e.type == sf::Event::MouseWheelScrolled) {
				offset -= (int)e.mouseWheelScroll.delta * 30;
			}
			if (isClick(e, 20, 20, 100, 35)) {
				return;
			}
		}
		if (offset < 0) {
			offset = 0;
		}
		win.clear(sf::Color(18, 32, 65));
		drawTxt(win, "Appointment History", 275, 30, font,26, sf::Color::White);
		drawBtn(win, 20, 20, 100, 35, "Back", font, sf::Color(70, 70, 90), sf::Color::White);
		int y = 90 - offset;
		for (int i = 0; i < apts.size(); i++) {
			Appointment& a = apts.getAll()[i];
			if (a.getDid() != d->getId()) continue;
			char row[300];
			snprintf(row, sizeof(row), "ID:%d  Patient:%d  %s %s  [%s]", a.getId(), a.getPid(), a.getDate(), a.getSlot(), a.getStat());
			drawTxt(win, row, 45, (float)y, font, 16, sf::Color(210, 220, 240));
			y += 35;
		}
		win.display();
	}
}

void viewPatientHistory(sf::RenderWindow& win, sf::Font& font, Doctor* d, Storage<Patient>& pts, Storage<Appointment>& apts, Storage<Prescription>& prs, FileHandler& fh) {
	char pidBuf[20] = "";
	char msg[120] = "";
	int len = 0;
	while (win.isOpen()) {
		sf::Event e;
		while (win.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				win.close();
			}
			typeChar(e, pidBuf, 20, len);
			if (isClick(e, 290, 300, 150, 40)) {
				int pid = Validator::toInt(pidBuf);
				Patient* p = pts.findById(pid);
				if (p == nullptr) {
					Validator::myCopy(msg, "Patient not found.", 120);
				} else {
					// Check if patient has at least one completed appointment with this doctor
					bool hasCompleted = false;
					for (int i = 0; i < apts.size(); i++) {
						Appointment& a = apts.getAll()[i];
						if (a.getPid() == pid && a.getDid() == d->getId() && Validator::myEq(a.getStat(), "done")) {
							hasCompleted = true;
							break;
						}
					}
					if (!hasCompleted) {
						Validator::myCopy(msg, "Access denied. You can only view records of your own patients.", 120);
					} else {
						// Display all prescriptions for that patient written by this doctor sorted by date descending
						viewPatientPrescriptions(win, font, d, pid, prs);
						return;
					}
				}
			}
			if (isClick(e, 460, 300, 150, 40)) {
				return;
			}
		}
		win.clear(sf::Color(18, 32, 65));
		drawTxt(win, "View Patient Medical History", 250, 40, font, 28, sf::Color::White);
		drawTxt(win, "Enter Patient ID:", 130, 120, font, 14, sf::Color(180, 200, 255));
		drawBox(win, 130, 145, 460, 40, pidBuf, font, true);
		drawBtn(win, 290, 300, 150, 40, "View", font, sf::Color(20, 120, 80), sf::Color::White);
		drawBtn(win, 460, 300, 150, 40, "Back", font, sf::Color(60, 60, 80), sf::Color::White);
		if (msg[0] != '\0') {
			drawTxt(win, msg, 130, 370, font, 14, sf::Color(255, 180, 120));
		}
		win.display();
	}
}

void viewPatientPrescriptions(sf::RenderWindow& win, sf::Font& font, Doctor* d, int pid, Storage<Prescription>& prs) {
	int offset = 0;
	while (win.isOpen()) {
		sf::Event e;
		while (win.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				win.close();
			}
			if (e.type == sf::Event::MouseWheelScrolled) {
				offset -= (int)e.mouseWheelScroll.delta * 30;
			}
			if (isClick(e, 20, 20, 100, 35)) {
				return;
			}
		}
		if (offset < 0) {
			offset = 0;
		}
		win.clear(sf::Color(18, 32, 65));
		drawTxt(win, "Patient Prescriptions", 270, 30, font, 26, sf::Color::White);
		drawBtn(win, 20, 20, 100, 35, "Back", font, sf::Color(70, 70, 90), sf::Color::White);
		int y = 90 - offset;
		for (int i = 0; i < prs.size(); i++) {
			Prescription& p = prs.getAll()[i];
			if (p.getPid() == pid && p.getDid() == d->getId()) {
				char row[400];
				snprintf(row, sizeof(row), "ID:%d  Date:%s  Meds:%s  Notes:%s", p.getId(), p.getDate(), p.getMeds(), p.getNotes());
				drawTxt(win, row, 45, (float)y, font, 14, sf::Color(210, 220, 240));
				y += 35;
			}
		}
		win.display();
	}
}

void showDoctorMenu(sf::RenderWindow& win, sf::Font& font, Doctor* d, Storage<Patient>& pts, Storage<Appointment>& apts, Storage<Prescription>& prs, FileHandler& fh) {
	while (win.isOpen()) {
		sf::Event e;
		while (win.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				win.close();
			}
			if (isClick(e, 300, 140, 300, 45)) {
				viewToday(win, font, d, apts);
			}
			if (isClick(e, 300, 200, 300, 45)) {
				markDone(win, font, d, apts, fh);
			}
			if (isClick(e, 300, 260, 300, 45)) {
				markNoShow(win, font, d, apts, fh);
			}
			if (isClick(e, 300, 320, 300, 45)) {
				writePresc(win, font, d, apts, prs, fh);
			}
			if (isClick(e, 300, 380, 300, 45)) {
				viewPatientHistory(win, font, d, pts, apts, prs, fh);
			}
			if (isClick(e, 300, 440, 300, 40)) {
				return;
			}
		}
		win.clear(sf::Color(18, 32, 65));
		char hdr[200];
		snprintf(hdr, sizeof(hdr), "Welcome, %s | Specialization: %s", d->getName(), d->getSpec());
		drawTxt(win, hdr, 200, 60, font, 22, sf::Color::White);
		drawBtn(win, 300, 140, 300, 45, "1. View Today's Appointments", font, sf::Color(20, 70, 150), sf::Color::White);
		drawBtn(win, 300, 200, 300, 45, "2. Mark Appointment Complete", font, sf::Color(20, 120, 80), sf::Color::White);
		drawBtn(win, 300, 260, 300, 45, "3. Mark Appointment No-Show", font, sf::Color(120, 80, 20), sf::Color::White);
		drawBtn(win, 300, 320, 300, 45, "4. Write Prescription", font, sf::Color(45, 95, 130), sf::Color::White);
		drawBtn(win, 300, 380, 300, 45, "5. View Patient Medical History", font, sf::Color(70, 70, 120), sf::Color::White);
		drawBtn(win, 300, 440, 300, 40, "6. Logout", font, sf::Color(80, 20, 20), sf::Color::White);
		win.display();
	}
}
