#include <SFML/Graphics.hpp>
#include "Screens.h"
#include "Storage.h"
#include "FileHandler.h"
#include "Admin.h"
#include "Patient.h"
#include "Doctor.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "Validator.h"
#include <cstdio>
#include <fstream>
#include <ctime>
using namespace std;

static int nextPatientId(Storage<Patient>& pts) {
	int mx = 0;
	for (int i = 0; i < pts.size(); i++) if (pts.getAll()[i].getId() > mx) mx = pts.getAll()[i].getId();
	return mx + 1;
}

static int nextDoctorId(Storage<Doctor>& drs) {
	int mx = 0;
	for (int i = 0; i < drs.size(); i++) if (drs.getAll()[i].getId() > mx) mx = drs.getAll()[i].getId();
	return mx + 1;
}

void addPatientAdmin(sf::RenderWindow& win, sf::Font& font, Storage<Patient>& pts, FileHandler& fh) {
	char name[100] = "", ageB[10] = "", gender[5] = "", con[15] = "", pass[50] = "", balB[30] = "", msg[140] = "";
	int ln = 0, la = 0, lg = 0, lc = 0, lp = 0, lb = 0;
	int foc = 0;
	while (win.isOpen()) {
		sf::Event e;
		while (win.pollEvent(e)) {
			if (e.type == sf::Event::Closed) win.close();
			if (foc == 0) typeChar(e, name, 100, ln);
			if (foc == 1) typeChar(e, ageB, 10, la);
			if (foc == 2) typeChar(e, gender, 5, lg);
			if (foc == 3) typeChar(e, con, 15, lc);
			if (foc == 4) typeChar(e, pass, 50, lp);
			if (foc == 5) typeChar(e, balB, 30, lb);
			if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Tab) foc = (foc + 1) % 6;
			if (isClick(e, 270, 560, 170, 40)) {
				if (!Validator::chkContact(con) || !Validator::chkPass(pass) || !Validator::chkFloat(balB)) {
					Validator::myCopy(msg, "Invalid contact/password/balance.", 140);
					continue;
				}
				Patient p(nextPatientId(pts), name, Validator::toInt(ageB), gender, con, pass, Validator::toFloat(balB));
				pts.add(p);
				fh.addPatient(p);
				Validator::myCopy(msg, "Patient added.", 140);
			}
			if (isClick(e, 460, 560, 170, 40)) return;
		}
		win.clear(sf::Color(18, 32, 65));
		drawTxt(win, "Add Patient", 360, 35, font, 28, sf::Color::White);
		drawBox(win, 140, 100, 620, 38, name, font, foc == 0);
		drawBox(win, 140, 150, 620, 38, ageB, font, foc == 1);
		drawBox(win, 140, 200, 620, 38, gender, font, foc == 2);
		drawBox(win, 140, 250, 620, 38, con, font, foc == 3);
		drawBox(win, 140, 300, 620, 38, pass, font, foc == 4);
		drawBox(win, 140, 350, 620, 38, balB, font, foc == 5);
		drawTxt(win, "Name / Age / Gender / Contact / Password / Balance", 140, 410, font, 14, sf::Color(180, 200, 255));
		drawBtn(win, 270, 560, 170, 40, "Save", font, sf::Color(20, 120, 80), sf::Color::White);
		drawBtn(win, 460, 560, 170, 40, "Back", font, sf::Color(60, 60, 80), sf::Color::White);
		if (msg[0] != '\0') drawTxt(win, msg, 140, 460, font, 14, sf::Color(255, 180, 120));
		win.display();
	}
}

void addDoctorAdmin(sf::RenderWindow& win, sf::Font& font, Storage<Doctor>& drs, FileHandler& fh) {
	char name[100] = "", spec[100] = "", feeB[30] = "", con[15] = "", pass[50] = "", msg[140] = "";
	int ln = 0, ls = 0, lf = 0, lc = 0, lp = 0;
	int foc = 0;
	while (win.isOpen()) {
		sf::Event e;
		while (win.pollEvent(e)) {
			if (e.type == sf::Event::Closed) win.close();
			if (foc == 0) typeChar(e, name, 100, ln);
			if (foc == 1) typeChar(e, spec, 100, ls);
			if (foc == 2) typeChar(e, feeB, 30, lf);
			if (foc == 3) typeChar(e, con, 15, lc);
			if (foc == 4) typeChar(e, pass, 50, lp);
			if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Tab) foc = (foc + 1) % 5;
			if (isClick(e, 270, 560, 170, 40)) {
				if (!Validator::chkContact(con) || !Validator::chkPass(pass) || !Validator::chkFloat(feeB)) {
					Validator::myCopy(msg, "Invalid contact/password/fee.", 140);
					continue;
				}
				Doctor d(nextDoctorId(drs), name, spec, Validator::toFloat(feeB), con, pass);
				drs.add(d);
				fh.addDoctor(d);
				Validator::myCopy(msg, "Doctor added.", 140);
			}
			if (isClick(e, 460, 560, 170, 40)) return;
		}
		win.clear(sf::Color(18, 32, 65));
		drawTxt(win, "Add Doctor", 360, 35, font, 28, sf::Color::White);
		drawBox(win, 140, 100, 620, 38, name, font, foc == 0);
		drawBox(win, 140, 150, 620, 38, spec, font, foc == 1);
		drawBox(win, 140, 200, 620, 38, feeB, font, foc == 2);
		drawBox(win, 140, 250, 620, 38, con, font, foc == 3);
		drawBox(win, 140, 300, 620, 38, pass, font, foc == 4);
		drawTxt(win, "Name / Specialization / Fee / Contact / Password", 140, 360, font, 14, sf::Color(180, 200, 255));
		drawBtn(win, 270, 560, 170, 40, "Save", font, sf::Color(20, 120, 80), sf::Color::White);
		drawBtn(win, 460, 560, 170, 40, "Back", font, sf::Color(60, 60, 80), sf::Color::White);
		if (msg[0] != '\0') drawTxt(win, msg, 140, 410, font, 14, sf::Color(255, 180, 120));
		win.display();
	}
}

void removePatientAdmin(sf::RenderWindow& win, sf::Font& font, Storage<Patient>& pts, FileHandler& fh) {
	char idBuf[20] = "", msg[120] = "";
	int len = 0;
	while (win.isOpen()) {
		sf::Event e;
		while (win.pollEvent(e)) {
			if (e.type == sf::Event::Closed) win.close();
			typeChar(e, idBuf, 20, len);
			if (isClick(e, 290, 300, 150, 40)) {
				int id = Validator::toInt(idBuf);
				if (pts.removeById(id)) {
					fh.delRecord("data/patients.txt", id);
					Validator::myCopy(msg, "Patient removed.", 120);
				} else Validator::myCopy(msg, "Patient not found.", 120);
			}
			if (isClick(e, 460, 300, 150, 40)) return;
		}
		win.clear(sf::Color(18, 32, 65));
		drawTxt(win, "Remove Patient", 325, 100, font, 28, sf::Color::White);
		drawBox(win, 180, 225, 460, 40, idBuf, font, true);
		drawBtn(win, 290, 300, 150, 40, "Remove", font, sf::Color(120, 70, 20), sf::Color::White);
		drawBtn(win, 460, 300, 150, 40, "Back", font, sf::Color(60, 60, 80), sf::Color::White);
		if (msg[0] != '\0') drawTxt(win, msg, 180, 370, font, 14, sf::Color(255, 180, 120));
		win.display();
	}
}

void removeDoctorAdmin(sf::RenderWindow& win, sf::Font& font, Storage<Doctor>& drs, FileHandler& fh) {
	char idBuf[20] = "", msg[120] = "";
	int len = 0;
	while (win.isOpen()) {
		sf::Event e;
		while (win.pollEvent(e)) {
			if (e.type == sf::Event::Closed) win.close();
			typeChar(e, idBuf, 20, len);
			if (isClick(e, 290, 300, 150, 40)) {
				int id = Validator::toInt(idBuf);
				if (drs.removeById(id)) {
					fh.delRecord("data/doctors.txt", id);
					Validator::myCopy(msg, "Doctor removed.", 120);
				} else Validator::myCopy(msg, "Doctor not found.", 120);
			}
			if (isClick(e, 460, 300, 150, 40)) return;
		}
		win.clear(sf::Color(18, 32, 65));
		drawTxt(win, "Remove Doctor", 330, 100, font, 28, sf::Color::White);
		drawBox(win, 180, 225, 460, 40, idBuf, font, true);
		drawBtn(win, 290, 300, 150, 40, "Remove", font, sf::Color(120, 70, 20), sf::Color::White);
		drawBtn(win, 460, 300, 150, 40, "Back", font, sf::Color(60, 60, 80), sf::Color::White);
		if (msg[0] != '\0') drawTxt(win, msg, 180, 370, font, 14, sf::Color(255, 180, 120));
		win.display();
	}
}

void viewAllApts(sf::RenderWindow& win, sf::Font& font, Storage<Appointment>& apts) {
	int offset = 0;
	while (win.isOpen()) {
		sf::Event e;
		while (win.pollEvent(e)) {
			if (e.type == sf::Event::Closed) win.close();
			if (e.type == sf::Event::MouseWheelScrolled) { offset -= (int)e.mouseWheelScroll.delta * 30; }
			if (isClick(e, 20, 20, 100, 35)) return;
		}
		if (offset < 0) offset = 0;
		win.clear(sf::Color(18, 32, 65));
		drawTxt(win, "All Appointments", 300, 30, font, 26, sf::Color::White);
		drawBtn(win, 20, 20, 100, 35, "Back", font, sf::Color(70, 70, 90), sf::Color::White);
		int y = 90 - offset;
		for (int i = 0; i < apts.size(); i++) {
			Appointment& a = apts.getAll()[i];
			char row[300];
			snprintf(row, sizeof(row), "ID:%d P:%d D:%d %s %s [%s]", a.getId(), a.getPid(), a.getDid(), a.getDate(), a.getSlot(), a.getStat());
			drawTxt(win, row, 45, (float)y, font, 16, sf::Color(210, 220, 240));
			y += 35;
		}
		win.display();
	}
}

void viewAllBills(sf::RenderWindow& win, sf::Font& font, Storage<Bill>& bls) {
	int offset = 0;
	while (win.isOpen()) {
		sf::Event e;
		while (win.pollEvent(e)) {
			if (e.type == sf::Event::Closed) win.close();
			if (e.type == sf::Event::MouseWheelScrolled) { offset -= (int)e.mouseWheelScroll.delta * 30; }
			if (isClick(e, 20, 20, 100, 35)) return;
		}
		if (offset < 0) offset = 0;
		win.clear(sf::Color(18, 32, 65));
		drawTxt(win, "All Bills", 360, 30, font, 26, sf::Color::White);
		drawBtn(win, 20, 20, 100, 35, "Back", font, sf::Color(70, 70, 90), sf::Color::White);
		int y = 90 - offset;
		for (int i = 0; i < bls.size(); i++) {
			Bill& b = bls.getAll()[i];
			char row[300];
			snprintf(row, sizeof(row), "ID:%d P:%d Ap:%d Amount:%.2f [%s]", b.getId(), b.getPid(), b.getApid(), b.getAmount(), b.getStat());
			drawTxt(win, row, 45, (float)y, font, 16, sf::Color(210, 220, 240));
			y += 35;
		}
		win.display();
	}
}

void dischargePatientAdmin(sf::RenderWindow& win, sf::Font& font, Storage<Patient>& pts, Storage<Appointment>& apts, Storage<Bill>& bls, FileHandler& fh) {
	char pidBuf[20] = "";
	int len = 0;
	char msg[180] = "";
	while (win.isOpen()) {
		sf::Event e;
		while (win.pollEvent(e)) {
			if (e.type == sf::Event::Closed) win.close();
			typeChar(e, pidBuf, 20, len);
			if (isClick(e, 290, 300, 150, 40)) {
				int pid = Validator::toInt(pidBuf);
				bool unpaid = false;
				bool pending = false;
				for (int i = 0; i < bls.size(); i++) {
					if (bls.getAll()[i].getPid() == pid && !Validator::myEq(bls.getAll()[i].getStat(), "paid")) unpaid = true;
				}
				for (int i = 0; i < apts.size(); i++) {
					if (apts.getAll()[i].getPid() == pid && Validator::myEq(apts.getAll()[i].getStat(), "pending")) pending = true;
				}
				if (unpaid || pending) {
					Validator::myCopy(msg, "Cannot discharge: unpaid bills or pending appointments.", 180);
				}
				else {
					if (pts.removeById(pid)) {
						fh.delRecord("data/patients.txt", pid);
						Validator::myCopy(msg, "Patient discharged successfully.", 180);
					}
					else {
						Validator::myCopy(msg, "Patient not found.", 180);
					}
				}
			}
			if (isClick(e, 460, 300, 150, 40)) return;
		}
		win.clear(sf::Color(18, 32, 65));
		drawTxt(win, "Discharge Patient", 310, 100, font, 28, sf::Color::White);
		drawBox(win, 180, 225, 460, 40, pidBuf, font, true);
		drawBtn(win, 290, 300, 150, 40, "Proceed", font, sf::Color(120, 80, 20), sf::Color::White);
		drawBtn(win, 460, 300, 150, 40, "Back", font, sf::Color(60, 60, 80), sf::Color::White);
		if (msg[0] != '\0') drawTxt(win, msg, 120, 370, font, 14, sf::Color(255, 180, 120));
		win.display();
	}
}

void viewAllPatientsAdmin(sf::RenderWindow& win, sf::Font& font, Storage<Patient>& pts) {
	int offset = 0;
	while (win.isOpen()) {
		sf::Event e;
		while (win.pollEvent(e)) {
			if (e.type == sf::Event::Closed) win.close();
			if (e.type == sf::Event::MouseWheelScrolled) { offset -= (int)e.mouseWheelScroll.delta * 30; }
			if (isClick(e, 20, 20, 100, 35)) return;
		}
		if (offset < 0) offset = 0;
		win.clear(sf::Color(18, 32, 65));
		drawTxt(win, "All Patients", 300, 30, font, 26, sf::Color::White);
		drawBtn(win, 20, 20, 100, 35, "Back", font, sf::Color(70, 70, 90), sf::Color::White);
		int y = 90 - offset;
		for (int i = 0; i < pts.size(); i++) {
			Patient& p = pts.getAll()[i];
			char row[200];
			snprintf(row, sizeof(row), "ID: %d | Name: %s | Contact: %s | Balance: %.2f", p.getId(), p.getName(), p.getContact(), p.getBal());
			drawTxt(win, row, 45, (float)y, font, 14, sf::Color(210, 220, 240));
			y += 35;
		}
		win.display();
	}
}

void viewAllDoctorsAdmin(sf::RenderWindow& win, sf::Font& font, Storage<Doctor>& drs) {
	int offset = 0;
	while (win.isOpen()) {
		sf::Event e;
		while (win.pollEvent(e)) {
			if (e.type == sf::Event::Closed) win.close();
			if (e.type == sf::Event::MouseWheelScrolled) { offset -= (int)e.mouseWheelScroll.delta * 30; }
			if (isClick(e, 20, 20, 100, 35)) return;
		}
		if (offset < 0) offset = 0;
		win.clear(sf::Color(18, 32, 65));
		drawTxt(win, "All Doctors", 300, 30, font, 26, sf::Color::White);
		drawBtn(win, 20, 20, 100, 35, "Back", font, sf::Color(70, 70, 90), sf::Color::White);
		int y = 90 - offset;
		for (int i = 0; i < drs.size(); i++) {
			Doctor& d = drs.getAll()[i];
			char row[200];
			snprintf(row, sizeof(row), "ID: %d | Name: %s | Specialization: %s | Fee: %.2f", d.getId(), d.getName(), d.getSpec(), d.getFee());
			drawTxt(win, row, 45, (float)y, font, 14, sf::Color(210, 220, 240));
			y += 35;
		}
		win.display();
	}
}

void viewAllAppointmentsAdmin(sf::RenderWindow& win, sf::Font& font, Storage<Appointment>& apts) {
	int offset = 0;
	while (win.isOpen()) {
		sf::Event e;
		while (win.pollEvent(e)) {
			if (e.type == sf::Event::Closed) win.close();
			if (e.type == sf::Event::MouseWheelScrolled) { offset -= (int)e.mouseWheelScroll.delta * 30; }
			if (isClick(e, 20, 20, 100, 35)) return;
		}
		if (offset < 0) offset = 0;
		win.clear(sf::Color(18, 32, 65));
		drawTxt(win, "All Appointments", 300, 30, font, 26, sf::Color::White);
		drawBtn(win, 20, 20, 100, 35, "Back", font, sf::Color(70, 70, 90), sf::Color::White);
		int y = 90 - offset;
		for (int i = 0; i < apts.size(); i++) {
			Appointment& a = apts.getAll()[i];
			char row[200];
			snprintf(row, sizeof(row), "ID: %d | Patient ID: %d | Doctor ID: %d | Date: %s | Time: %s | Status: %s", 
				a.getId(), a.getPid(), a.getDid(), a.getDate(), a.getSlot(), a.getStat());
			drawTxt(win, row, 45, (float)y, font, 14, sf::Color(210, 220, 240));
			y += 35;
		}
		win.display();
	}
}

void viewUnpaidBillsAdmin(sf::RenderWindow& win, sf::Font& font, Storage<Bill>& bls) {
	int offset = 0;
	while (win.isOpen()) {
		sf::Event e;
		while (win.pollEvent(e)) {
			if (e.type == sf::Event::Closed) win.close();
			if (e.type == sf::Event::MouseWheelScrolled) { offset -= (int)e.mouseWheelScroll.delta * 30; }
			if (isClick(e, 20, 20, 100, 35)) return;
		}
		if (offset < 0) offset = 0;
		win.clear(sf::Color(18, 32, 65));
		drawTxt(win, "Unpaid Bills", 300, 30, font, 26, sf::Color::White);
		drawBtn(win, 20, 20, 100, 35, "Back", font, sf::Color(70, 70, 90), sf::Color::White);
		int y = 90 - offset;
		for (int i = 0; i < bls.size(); i++) {
			Bill& b = bls.getAll()[i];
			if (!Validator::myEq(b.getStat(), "paid")) {
				char row[200];
				snprintf(row, sizeof(row), "Bill ID: %d | Patient ID: %d | Appointment ID: %d | Amount: %.2f | Status: %s", 
					b.getId(), b.getPid(), b.getApid(), b.getAmount(), b.getStat());
				drawTxt(win, row, 45, (float)y, font, 14, sf::Color(210, 220, 240));
				y += 35;
			}
		}
		win.display();
	}
}

void viewSecurityLogAdmin(sf::RenderWindow& win, sf::Font& font, FileHandler& fh) {
	int offset = 0;
	while (win.isOpen()) {
		sf::Event e;
		while (win.pollEvent(e)) {
			if (e.type == sf::Event::Closed) win.close();
			if (e.type == sf::Event::MouseWheelScrolled) { offset -= (int)e.mouseWheelScroll.delta * 30; }
			if (isClick(e, 20, 20, 100, 35)) return;
		}
		if (offset < 0) offset = 0;
		win.clear(sf::Color(18, 32, 65));
		drawTxt(win, "Security Log", 360, 30, font, 26, sf::Color::White);
		drawBtn(win, 20, 20, 100, 35, "Back", font, sf::Color(70, 70, 90), sf::Color::White);
		int y = 90 - offset;
		
		// Read security log file
		ifstream f("data/security_log.txt");
		if (!f.is_open()) {
			drawTxt(win, "No security events logged.", 45, 90, font, 16, sf::Color(210, 220, 240));
		} else {
			char line[200];
			int lineCount = 0;
			bool firstLine = true;
			while (f.getline(line, 200)) {
				if (firstLine) {
					firstLine = false;
					continue; // Skip header line
				}
				drawTxt(win, line, 45, (float)y, font, 14, sf::Color(210, 220, 240));
				y += 20;
				lineCount++;
				if (lineCount > 25) {
					break;
				} // Limit display to prevent overflow
			}
			f.close();
		}
		win.display();
	}
}

void dailyReportAdmin(sf::RenderWindow& win, sf::Font& font, Storage<Patient>& pts, Storage<Doctor>& drs, Storage<Appointment>& apts, Storage<Bill>& bls, Storage<Prescription>& prs, FileHandler& fh) {
	while (win.isOpen()) {
		sf::Event e;
		while (win.pollEvent(e)) {
			if (e.type == sf::Event::Closed) win.close();
			if (isClick(e, 350, 520, 180, 40)) return;
		}
		
		// Get today's date
		char today[15];
		time_t t = time(nullptr);
		tm now;
		localtime_s(&now, &t);
		strftime(today, 15, "%d-%m-%Y", &now);
		
		// Count today's appointments and revenue
		int todayPending = 0, todayCompleted = 0, todayNoShow = 0, todayCancelled = 0;
		float todayRevenue = 0.0f;
		
		for (int i = 0; i < apts.size(); i++) {
			Appointment& a = apts.getAll()[i];
			if (Validator::myEq(a.getDate(), today)) {
				if (Validator::myEq(a.getStat(), "pending")) todayPending++;
				else if (Validator::myEq(a.getStat(), "completed")) todayCompleted++;
				else if (Validator::myEq(a.getStat(), "no-show")) todayNoShow++;
				else if (Validator::myEq(a.getStat(), "cancelled")) todayCancelled++;
			}
		}
		
		// Calculate today's revenue from paid bills
		for (int i = 0; i < bls.size(); i++) {
			Bill& b = bls.getAll()[i];
			if (Validator::myEq(b.getDate(), today) && Validator::myEq(b.getStat(), "paid")) {
				todayRevenue += b.getAmount();
			}
		}
		
		// Patients with outstanding unpaid bills
		char patientSummary[300] = "";
		for (int i = 0; i < pts.size(); i++) {
			Patient& p = pts.getAll()[i];
			float totalOwed = 0.0f;
			for (int j = 0; j < bls.size(); j++) {
				Bill& b = bls.getAll()[j];
				if (b.getPid() == p.getId() && !Validator::myEq(b.getStat(), "paid")) {
					totalOwed += b.getAmount();
				}
			}
			if (totalOwed > 0) {
				char patientLine[200];
				snprintf(patientLine, sizeof(patientLine), "%s | Total Owed: %.2f", p.getName(), totalOwed);
				if (patientSummary[0] != '\0') {
					strcat_s(patientSummary, sizeof(patientSummary), "\n");
					strcat_s(patientSummary, sizeof(patientSummary), patientLine);
				} else {
					Validator::myCopy(patientSummary, patientLine, 300);
				}
			}
		}
		
		// Doctor-wise summary for today
		char doctorSummary[500] = "";
		for (int i = 0; i < drs.size(); i++) {
			Doctor& d = drs.getAll()[i];
			int docCompleted = 0, docPending = 0, docNoShow = 0;
			for (int j = 0; j < apts.size(); j++) {
				Appointment& a = apts.getAll()[j];
				if (Validator::myEq(a.getDate(), today) && a.getDid() == d.getId()) {
					if (Validator::myEq(a.getStat(), "completed")) docCompleted++;
					else if (Validator::myEq(a.getStat(), "pending")) docPending++;
					else if (Validator::myEq(a.getStat(), "no-show")) docNoShow++;
				}
			}
			
			char doctorLine[200];
			snprintf(doctorLine, sizeof(doctorLine), "%s | Completed: %d | Pending: %d | No-show: %d", 
				d.getName(), docCompleted, docPending, docNoShow);
			if (doctorSummary[0] != '\0') {
				strcat_s(doctorSummary, sizeof(doctorSummary), "\n");
				strcat_s(doctorSummary, sizeof(doctorSummary), doctorLine);
			} else {
				Validator::myCopy(doctorSummary, doctorLine, 500);
			}
		}

		win.clear(sf::Color(18, 32, 65));
		drawTxt(win, "Daily Report", 340, 80, font, 30, sf::Color::White);
		
		char report1[200], report2[200], report3[200], report4[200], report5[200];
		snprintf(report1, sizeof(report1), "Total appointments today: %d (Pending: %d Completed: %d No-show: %d Cancelled: %d)", 
			todayPending + todayCompleted + todayNoShow + todayCancelled, todayPending, todayCompleted, todayNoShow, todayCancelled);
		snprintf(report2, sizeof(report2), "Revenue collected today (paid bills): PKR %.2f", todayRevenue);
		snprintf(report3, sizeof(report3), "Patients with outstanding unpaid bills:\n%s", patientSummary);
		snprintf(report4, sizeof(report4), "Doctor-wise summary for today:\n%s", doctorSummary);
		
		drawTxt(win, report1, 150, 180, font, 20, sf::Color(210, 220, 240));
		drawTxt(win, report2, 150, 220, font, 20, sf::Color(210, 220, 240));
		drawTxt(win, report3, 150, 260, font, 20, sf::Color(210, 220, 240));
		drawTxt(win, report4, 150, 340, font, 20, sf::Color(210, 220, 240));
		drawBtn(win, 350, 520, 180, 40, "Back", font, sf::Color(70, 70, 90), sf::Color::White);
		win.display();
	}
}

void showAdminMenu(sf::RenderWindow& win, sf::Font& font, Admin* a, Storage<Patient>& pts, Storage<Doctor>& drs, Storage<Appointment>& apts, Storage<Bill>& bls, Storage<Prescription>& prs, FileHandler& fh) {
	while (win.isOpen()) {
		sf::Event e;
		while (win.pollEvent(e)) {
			if (e.type == sf::Event::Closed) win.close();
			if (isClick(e, 200, 140, 280, 45)) addDoctorAdmin(win, font, drs, fh);
			if (isClick(e, 200, 200, 280, 45)) removeDoctorAdmin(win, font, drs, fh);
			if (isClick(e, 200, 260, 280, 45)) viewAllPatientsAdmin(win, font, pts);
			if (isClick(e, 200, 320, 280, 45)) viewAllDoctorsAdmin(win, font, drs);
			if (isClick(e, 200, 380, 280, 45)) viewAllAppointmentsAdmin(win, font, apts);
			if (isClick(e, 520, 140, 280, 45)) viewUnpaidBillsAdmin(win, font, bls);
			if (isClick(e, 520, 200, 280, 45)) dischargePatientAdmin(win, font, pts, apts, bls, fh);
			if (isClick(e, 520, 260, 280, 45)) viewSecurityLogAdmin(win, font, fh);
			if (isClick(e, 520, 320, 280, 45)) dailyReportAdmin(win, font, pts, drs, apts, bls, prs, fh);
			if (isClick(e, 520, 380, 280, 40)) return;
		}
		win.clear(sf::Color(18, 32, 65));
		drawTxt(win, "Admin Panel - MediCore", 350, 60, font, 26, sf::Color::White);
		drawBtn(win, 200, 140, 280, 45, "1. Add Doctor", font, sf::Color(20, 70, 150), sf::Color::White);
		drawBtn(win, 200, 200, 280, 45, "2. Remove Doctor", font, sf::Color(20, 100, 80), sf::Color::White);
		drawBtn(win, 200, 260, 280, 45, "3. View All Patients", font, sf::Color(45, 75, 130), sf::Color::White);
		drawBtn(win, 200, 320, 280, 45, "4. View All Doctors", font, sf::Color(45, 75, 130), sf::Color::White);
		drawBtn(win, 200, 380, 280, 45, "5. View All Appointments", font, sf::Color(95, 75, 40), sf::Color::White);
		drawBtn(win, 520, 140, 280, 45, "6. View Unpaid Bills", font, sf::Color(120, 70, 20), sf::Color::White);
		drawBtn(win, 520, 200, 280, 45, "7. Discharge Patient", font, sf::Color(120, 80, 20), sf::Color::White);
		drawBtn(win, 520, 260, 280, 45, "8. View Security Log", font, sf::Color(45, 95, 130), sf::Color::White);
		drawBtn(win, 520, 320, 280, 45, "9. Generate Daily Report", font, sf::Color(45, 95, 130), sf::Color::White);
		drawBtn(win, 520, 380, 280, 40, "10. Logout", font, sf::Color(80, 20, 20), sf::Color::White);
		win.display();
	}
}
