#include <SFML/Graphics.hpp>
#include "Screens.h"
#include "Storage.h"
#include "FileHandler.h"
#include "Patient.h"
#include "Doctor.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "Validator.h"
#include <cstdio>
using namespace std;

static int nextApptId(Storage<Appointment>& apts) {
	int mx = 0;
	for (int i = 0; i < apts.size(); i++) {
		if (apts.getAll()[i].getId() > mx) {
			mx = apts.getAll()[i].getId();
		}
	}
	return mx + 1;
}

static int nextBillId(Storage<Bill>& bls) {
	int mx = 0;
	for (int i = 0; i < bls.size(); i++) {
		if (bls.getAll()[i].getId() > mx) {
			mx = bls.getAll()[i].getId();
		}
	}
	return mx + 1;
}

void bookAppt(sf::RenderWindow& win, sf::Font& font, Patient* p, Storage<Doctor>& drs, Storage<Appointment>& apts, Storage<Bill>& bls, FileHandler& fh) {
	char spec[100] = "";
	char didBuf[20] = "";
	char date[15] = "";
	char slot[10] = "";
	char msg[200] = "";
	int sLen = 0, dLen = 0, dtLen = 0, slLen = 0;
	int step = 0; // 0: spec, 1: doctor display, 2: doctor ID, 3: date, 4: date validation, 5: slots, 6: slot input
	int dateAttempts = 0;
	const char* slots[8] = { "09:00", "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00" };
	Doctor* selectedDoctor = nullptr;

	while (win.isOpen()) {
		sf::Event e;
		while (win.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				win.close();
			}

			if (step == 0) {
				typeChar(e, spec, 100, sLen);
			}
			if (step == 2) {
				typeChar(e, didBuf, 20, dLen);
			}
			if (step == 3) {
				typeChar(e, date, 15, dtLen);
			}
			if (step == 6) {
				typeChar(e, slot, 10, slLen);
			}

			if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Return) {
				if (step == 0 && Validator::myLen(spec) > 0) {
					step = 1;
				}
				else if (step == 2 && Validator::myLen(didBuf) > 0) {
					step = 3;
				}
				else if (step == 3 && Validator::myLen(date) > 0) {
					step = 4;
				}
				else if (step == 6 && Validator::myLen(slot) > 0) {
					step = 7;
				}
			}
			
			if (isClick(e, 250, 510, 180, 40)) {
				if (step == 0) {
					// Search doctors by specialization (case-insensitive)
					bool found = false;
					for (int i = 0; i < drs.size(); i++) {
						char lowerSpec[100], lowerInput[100];
						Validator::toLower(drs.getAll()[i].getSpec(), lowerSpec);
						Validator::toLower(spec, lowerInput);
						if (Validator::myEq(lowerSpec, lowerInput)) {
							found = true;
							break;
						}
					}
					if (!found) {
						Validator::myCopy(msg, "No doctors available for that specialization.", 200);
						step = 0;
					} else {
						step = 1;
					}
				} else if (step == 2) {
					int did = Validator::toInt(didBuf);
					selectedDoctor = drs.findById(did);
					if (selectedDoctor == nullptr) {
						Validator::myCopy(msg, "Doctor not found.", 200);
					} else {
						step = 3;
					}
				} else if (step == 4) {
					if (!Validator::chkDate(date)) {
						Validator::myCopy(msg, "Invalid date. Use format DD-MM-YYYY.", 200);
						dateAttempts++;
						if (dateAttempts >= 3) {
							step = 0;
							dateAttempts = 0;
						}
					} else {
						step = 5;
					}
				} else if (step == 6) {
					// Validate slot
					bool validSlot = false;
					for (int i = 0; i < 8; i++) {
						if (Validator::myEq(slot, slots[i])) {
							validSlot = true;
							break;
						}
					}
					if (!validSlot) {
						Validator::myCopy(msg, "Invalid time slot. Choose from available slots.", 200);
					} else {
						// Check if slot is available
						bool slotTaken = false;
						for (int i = 0; i < apts.size(); i++) {
							Appointment& a = apts.getAll()[i];
							if (a.getDid() == selectedDoctor->getId() && Validator::myEq(a.getDate(), date) && Validator::myEq(a.getSlot(), slot) && !Validator::myEq(a.getStat(), "cancelled")) {
								slotTaken = true;
								break;
							}
						}
						if (slotTaken) {
							Validator::myCopy(msg, "SlotUnavailableException: Slot already taken.", 200);
							step = 5; // Re-display available slots
						} else {
							// Check patient balance
							if (p->getBal() < selectedDoctor->getFee()) {
								Validator::myCopy(msg, "InsufficientFundsException: Insufficient balance for appointment fee.", 200);
								step = 0;
							} else {
								// Book appointment
								Appointment newAppt(nextApptId(apts), p->getId(), selectedDoctor->getId(), date, slot, "pending");
								apts.add(newAppt);
								fh.addAppt(newAppt);
								
								// Create bill
								Bill newBill(nextBillId(bls), p->getId(), newAppt.getId(), selectedDoctor->getFee(), "unpaid", date);
								bls.add(newBill);
								fh.addBill(newBill);
								
								// Update patient balance
								*p -= selectedDoctor->getFee();
								fh.updField("data/patients.txt", p->getId(), 6, "");
								char temp[50];
								Validator::floatToStr(p->getBal(), temp);
								fh.updField("data/patients.txt", p->getId(), 6, temp);
								
								char successMsg[200];
								snprintf(successMsg, sizeof(successMsg), "Appointment booked successfully. Appointment ID: %d.", newAppt.getId());
								Validator::myCopy(msg, successMsg, 200);
								step = 8; // Success state
							}
						}
					}
				}
			}
			if (isClick(e, 470, 510, 180, 40)) {
				return;
			}
		}

		win.clear(sf::Color(18, 32, 65));
		drawTxt(win, "Book Appointment", 315, 40, font, 28, sf::Color::White);
		
		if (step == 0) {
			drawTxt(win, "Enter specialization to search (e.g. Cardiology):", 130, 120, font, 14, sf::Color(180, 200, 255));
			drawBox(win, 130, 145, 620, 38, spec, font, true);
		} else if (step == 1) {
			// Display doctors with that specialization
			drawTxt(win, "Available doctors:", 130, 120, font, 14, sf::Color(180, 200, 255));
			int y = 145;
			for (int i = 0; i < drs.size(); i++) {
				char lowerSpec[100], lowerInput[100];
				Validator::toLower(drs.getAll()[i].getSpec(), lowerSpec);
				Validator::toLower(spec, lowerInput);
				if (Validator::myEq(lowerSpec, lowerInput)) {
					char doctorInfo[200];
					snprintf(doctorInfo, sizeof(doctorInfo), "ID:%d | Name:%s | Fee:%.2f", 
						drs.getAll()[i].getId(), drs.getAll()[i].getName(), drs.getAll()[i].getFee());
					drawTxt(win, doctorInfo, 130, (float)y, font, 14, sf::Color(210, 220, 240));
					y += 25;
				}
			}
			drawTxt(win, "Enter Doctor ID:", 130, y + 20, font, 14, sf::Color(180, 200, 255));
			drawBox(win, 130, y + 45, 620, 38, didBuf, font, true);
			step = 2;
		} else if (step == 3) {
			drawTxt(win, "Enter date (DD-MM-YYYY):", 130, 120, font, 14, sf::Color(180, 200, 255));
			drawBox(win, 130, 145, 620, 38, date, font, true);
		} else if (step == 5) {
			// Display available time slots
			drawTxt(win, "Available time slots:", 130, 120, font, 14, sf::Color(180, 200, 255));
			int y = 145;
			for (int i = 0; i < 8; i++) {
				bool slotAvailable = true;
				for (int j = 0; j < apts.size(); j++) {
					Appointment& a = apts.getAll()[j];
					if (a.getDid() == selectedDoctor->getId() && Validator::myEq(a.getDate(), date) && Validator::myEq(a.getSlot(), slots[i]) && !Validator::myEq(a.getStat(), "cancelled")) {
						slotAvailable = false;
						break;
					}
				}
				char slotInfo[100];
				if (slotAvailable) {
					snprintf(slotInfo, sizeof(slotInfo), "• %s", slots[i]);
				} else {
					snprintf(slotInfo, sizeof(slotInfo), "• %s (TAKEN)", slots[i]);
				}
				drawTxt(win, slotInfo, 130, (float)y, font, 14, sf::Color(210, 220, 240));
				y += 25;
			}
			drawTxt(win, "Enter time slot (e.g. 09:00):", 130, y + 20, font, 14, sf::Color(180, 200, 255));
			drawBox(win, 130, y + 45, 620, 38, slot, font, true);
		} else if (step == 8) {
			drawTxt(win, "Appointment booked successfully!", 130, 120, font, 14, sf::Color(180, 200, 255));
		}

		drawBtn(win, 250, 510, 180, 40, "Submit", font, sf::Color(30, 90, 170), sf::Color::White);
		drawBtn(win, 470, 510, 180, 40, "Back", font, sf::Color(70, 70, 90), sf::Color::White);
		if (msg[0] != '\0') {
			drawTxt(win, msg, 130, 565, font, 14, sf::Color(255, 180, 120));
		}
		win.display();
	}
}

void cancelAppt(sf::RenderWindow& win, sf::Font& font, Patient* p, Storage<Appointment>& apts, Storage<Doctor>& drs, Storage<Bill>& bls, FileHandler& fh) {
	char idBuf[20] = "";
	int len = 0;
	char msg[200] = "";
	bool displayPending = true;

	while (win.isOpen()) {
		sf::Event e;
		while (win.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				win.close();
			}
			if (displayPending) {
				// Display pending appointments for this patient
				bool hasPending = false;
				for (int i = 0; i < apts.size(); i++) {
					Appointment& a = apts.getAll()[i];
					if (a.getPid() == p->getId() && Validator::myEq(a.getStat(), "pending")) {
						hasPending = true;
						break;
					}
				}
				if (!hasPending) {
					Validator::myCopy(msg, "You have no pending appointments.", 200);
					displayPending = false;
				}
			} else {
				typeChar(e, idBuf, 20, len);
				if (isClick(e, 290, 300, 150, 40)) {
					int id = Validator::toInt(idBuf);
					bool found = false;
					for (int i = 0; i < apts.size(); i++) {
						Appointment& a = apts.getAll()[i];
						if (a.getId() == id && a.getPid() == p->getId() && Validator::myEq(a.getStat(), "pending")) {
							// Find doctor for fee refund
							Doctor* dr = drs.findById(a.getDid());
							if (dr != nullptr) {
								// Update appointment status to cancelled
								a.setStat("cancelled");
								fh.updField("data/appointments.txt", id, 5, "cancelled");
								
								// Refund fee to patient balance
								*p += dr->getFee();
								char temp[50];
								Validator::floatToStr(p->getBal(), temp);
								fh.updField("data/patients.txt", p->getId(), 6, temp);
								
								// Update corresponding bill status to cancelled
								for (int j = 0; j < bls.size(); j++) {
									Bill& b = bls.getAll()[j];
									if (b.getApid() == id) {
										b.setStat("cancelled");
										fh.updField("data/bills.txt", b.getId(), 4, "cancelled");
										break;
									}
								}
								
								char successMsg[200];
								snprintf(successMsg, sizeof(successMsg), "Appointment cancelled. PKR %.2f refunded to your balance.", dr->getFee());
								Validator::myCopy(msg, successMsg, 200);
							}
							found = true;
							break;
						}
					}
					if (!found) {
						Validator::myCopy(msg, "Invalid appointment ID.", 200);
					}
				}
			}
			if (isClick(e, 460, 300, 150, 40)) {
				return;
			}
		}
		
		win.clear(sf::Color(18, 32, 65));
		drawTxt(win, "Cancel Appointment", 300, 100, font, 28, sf::Color::White);
		
		if (displayPending) {
			drawTxt(win, "All pending appointments:", 130, 120, font, 14, sf::Color(180, 200, 255));
			int y = 145;
			for (int i = 0; i < apts.size(); i++) {
				Appointment& a = apts.getAll()[i];
				if (a.getPid() == p->getId() && Validator::myEq(a.getStat(), "pending")) {
					Doctor* dr = drs.findById(a.getDid());
					char aptInfo[200];
					snprintf(aptInfo, sizeof(aptInfo), "Appointment ID: %d | Doctor Name: %s | Date: %s | Time Slot: %s", 
						a.getId(), dr ? dr->getName() : "Unknown", a.getDate(), a.getSlot());
					drawTxt(win, aptInfo, 130, (float)y, font, 14, sf::Color(210, 220, 240));
					y += 25;
				}
			}
			drawTxt(win, "Enter Appointment ID to cancel:", 130, y + 20, font, 14, sf::Color(180, 200, 255));
			drawBox(win, 130, y + 45, 620, 38, idBuf, font, true);
		} else {
			drawTxt(win, "Enter Appointment ID to cancel:", 130, 120, font, 14, sf::Color(180, 200, 255));
			drawBox(win, 130, 145, 620, 38, idBuf, font, true);
		}
		
		drawBtn(win, 290, 300, 150, 40, "Cancel", font, sf::Color(120, 40, 40), sf::Color::White);
		drawBtn(win, 460, 300, 150, 40, "Back", font, sf::Color(60, 60, 80), sf::Color::White);
		if (msg[0] != '\0') {
			drawTxt(win, msg, 130, 370, font, 14, sf::Color(255, 180, 120));
		}
		win.display();
	}
}

void viewMyAppointments(sf::RenderWindow& win, sf::Font& font, Patient* p, Storage<Appointment>& apts, Storage<Doctor>& drs) {
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
		drawTxt(win, "My Appointments", 320, 30, font, 26, sf::Color::White);
		drawBtn(win, 20, 20, 100, 35, "Back", font, sf::Color(70, 70, 90), sf::Color::White);
		int y = 90 - offset;
		
		// Collect patient's appointments
		Appointment patientApts[100];
		int aptCount = 0;
		for (int i = 0; i < apts.size(); i++) {
			Appointment& a = apts.getAll()[i];
			if (a.getPid() == p->getId()) {
				patientApts[aptCount] = a;
				aptCount++;
			}
		}
		
		if (aptCount == 0) {
			drawTxt(win, "No appointments found.", 60, 90, font, 16, sf::Color(210, 220, 240));
		} else {
			// Sort by date ascending (manual bubble sort)
			for (int i = 0; i < aptCount - 1; i++) {
				for (int j = 0; j < aptCount - i - 1; j++) {
					if (Validator::myCmp(patientApts[j].getDate(), patientApts[j + 1].getDate()) > 0) {
						Appointment temp = patientApts[j];
						patientApts[j] = patientApts[j + 1];
						patientApts[j + 1] = temp;
					}
				}
			}
			
			// Display sorted appointments
			for (int i = 0; i < aptCount; i++) {
				Appointment& a = patientApts[i];
				Doctor* dr = drs.findById(a.getDid());
				char row[300];
				snprintf(row, sizeof(row), "ID: %d | Doctor Name: %s | Specialization: %s | Date: %s | Time Slot: %s | Status: %s", 
					a.getId(), dr ? dr->getName() : "Unknown", dr ? dr->getSpec() : "Unknown", a.getDate(), a.getSlot(), a.getStat());
				drawTxt(win, row, 45, (float)y, font, 14, sf::Color(210, 220, 240));
				y += 35;
			}
		}
		win.display();
	}
}

void viewMyMedicalRecords(sf::RenderWindow& win, sf::Font& font, Patient* p, Storage<Prescription>& prs, Storage<Doctor>& drs) {
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
		drawTxt(win, "My Medical Records", 280, 30, font, 26, sf::Color::White);
		drawBtn(win, 20, 20, 100, 35, "Back", font, sf::Color(70, 70, 90), sf::Color::White);
		int y = 90 - offset;
		
		// Collect patient's prescriptions
		Prescription patientPrescriptions[100];
		int prescCount = 0;
		for (int i = 0; i < prs.size(); i++) {
			Prescription& r = prs.getAll()[i];
			if (r.getPid() == p->getId()) {
				patientPrescriptions[prescCount] = r;
				prescCount++;
			}
		}
		
		if (prescCount == 0) {
			drawTxt(win, "No medical records found.", 60, 90, font, 16, sf::Color(210, 220, 240));
		} else {
			// Sort by date descending (manual bubble sort)
			for (int i = 0; i < prescCount - 1; i++) {
				for (int j = 0; j < prescCount - i - 1; j++) {
					if (Validator::myCmp(patientPrescriptions[j].getDate(), patientPrescriptions[j + 1].getDate()) < 0) {
						Prescription temp = patientPrescriptions[j];
						patientPrescriptions[j] = patientPrescriptions[j + 1];
						patientPrescriptions[j + 1] = temp;
					}
				}
			}
			
			// Display prescriptions grouped by appointment
			for (int i = 0; i < prescCount; i++) {
				Prescription& r = patientPrescriptions[i];
				Doctor* dr = drs.findById(r.getDid());
				char row[400];
				snprintf(row, sizeof(row), "Date: %s | Doctor Name: %s | Medicines: %s | Notes: %s", 
					r.getDate(), dr ? dr->getName() : "Unknown", r.getMeds(), r.getNotes());
				drawTxt(win, row, 45, (float)y, font, 14, sf::Color(210, 220, 240));
				y += 35;
			}
		}
		win.display();
	}
}

void viewMyBills(sf::RenderWindow& win, sf::Font& font, Patient* p, Storage<Bill>& bls) {
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
		drawTxt(win, "My Bills", 380, 30, font, 26, sf::Color::White);
		drawBtn(win, 20, 20, 100, 35, "Back", font, sf::Color(70, 70, 90), sf::Color::White);
		int y = 90 - offset;
		float totalOutstanding = 0.0f;
		bool hasBills = false;
		
		for (int i = 0; i < bls.size(); i++) {
			Bill& b = bls.getAll()[i];
			if (b.getPid() != p->getId()) {
				continue;
			}
			
			hasBills = true;
			char row[300];
			snprintf(row, sizeof(row), "Bill ID: %d | Appointment ID: %d | Amount (PKR): %.2f | Status: %s | Date: %s", 
				b.getId(), b.getApid(), b.getAmount(), b.getStat(), b.getDate());
			drawTxt(win, row, 45, (float)y, font, 14, sf::Color(210, 220, 240));
			y += 35;
			
			// Calculate total outstanding
			if (!Validator::myEq(b.getStat(), "paid")) {
				totalOutstanding += b.getAmount();
			}
		}
		
		if (!hasBills) {
			drawTxt(win, "No bills found.", 45, 90, font, 16, sf::Color(210, 220, 240));
		} else {
			// Display total outstanding
			char totalStr[200];
			snprintf(totalStr, sizeof(totalStr), "Total outstanding unpaid amount: PKR %.2f", totalOutstanding);
			drawTxt(win, totalStr, 45, (float)y + 20, font, 16, sf::Color(255, 200, 120));
		}
		win.display();
	}
}

void payBill(sf::RenderWindow& win, sf::Font& font, Patient* p, Storage<Bill>& bls, FileHandler& fh) {
	char idBuf[20] = "";
	int len = 0;
	char msg[200] = "";
	bool displayUnpaid = true;

	while (win.isOpen()) {
		sf::Event e;
		while (win.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				win.close();
			}
			
			if (displayUnpaid) {
				// Display unpaid bills for this patient
				bool hasUnpaid = false;
				for (int i = 0; i < bls.size(); i++) {
					Bill& b = bls.getAll()[i];
					if (b.getPid() == p->getId() && !Validator::myEq(b.getStat(), "paid")) {
						hasUnpaid = true;
						break;
					}
				}
				if (!hasUnpaid) {
					Validator::myCopy(msg, "No unpaid bills.", 200);
					displayUnpaid = false;
				}
			} else {
				typeChar(e, idBuf, 20, len);
				if (isClick(e, 290, 300, 150, 40)) {
					int bid = Validator::toInt(idBuf);
					bool found = false;
					for (int i = 0; i < bls.size(); i++) {
						Bill& b = bls.getAll()[i];
						if (b.getId() == bid && b.getPid() == p->getId() && !Validator::myEq(b.getStat(), "paid")) {
							// Check balance
							if (p->getBal() < b.getAmount()) {
								Validator::myCopy(msg, "InsufficientFundsException: Insufficient balance for bill payment.", 200);
							} else {
								// Pay bill
								*p -= b.getAmount();
								b.setStat("paid");
								fh.updField("data/bills.txt", b.getId(), 4, "paid");
								
								// Update patient balance
								char temp[50];
								Validator::floatToStr(p->getBal(), temp);
								fh.updField("data/patients.txt", p->getId(), 6, temp);
								
								char successMsg[200];
								snprintf(successMsg, sizeof(successMsg), "Bill paid successfully. Remaining balance: PKR %.2f", p->getBal());
								Validator::myCopy(msg, successMsg, 200);
							}
							found = true;
							break;
						}
					}
					if (!found) {
						Validator::myCopy(msg, "Bill not found or already paid.", 200);
					}
				}
			}
			if (isClick(e, 460, 300, 150, 40)) {
				return;
			}
		}

		win.clear(sf::Color(18, 32, 65));
		drawTxt(win, "Pay Bill", 390, 100, font, 28, sf::Color::White);
		
		if (displayUnpaid) {
			drawTxt(win, "Unpaid bills:", 180, 120, font, 14, sf::Color(180, 200, 255));
			int y = 145;
			for (int i = 0; i < bls.size(); i++) {
				Bill& b = bls.getAll()[i];
				if (b.getPid() == p->getId() && !Validator::myEq(b.getStat(), "paid")) {
					char billInfo[200];
					snprintf(billInfo, sizeof(billInfo), "Bill ID: %d | Amount (PKR): %.2f | Date: %s", 
						b.getId(), b.getAmount(), b.getDate());
					drawTxt(win, billInfo, 180, (float)y, font, 14, sf::Color(210, 220, 240));
					y += 25;
				}
			}
			drawTxt(win, "Enter Bill ID to pay:", 180, y + 20, font, 14, sf::Color(180, 200, 255));
			drawBox(win, 180, y + 45, 620, 38, idBuf, font, true);
		} else {
			drawTxt(win, "Enter Bill ID to pay:", 180, 120, font, 14, sf::Color(180, 200, 255));
			drawBox(win, 180, 145, 620, 38, idBuf, font, true);
		}
		
		drawBtn(win, 290, 300, 150, 40, "Pay", font, sf::Color(20, 120, 80), sf::Color::White);
		drawBtn(win, 460, 300, 150, 40, "Back", font, sf::Color(60, 60, 80), sf::Color::White);
		if (msg[0] != '\0') {
			drawTxt(win, msg, 130, 370, font, 14, sf::Color(255, 180, 120));
		}
		win.display();
	}
}

void topUpBalance(sf::RenderWindow& win, sf::Font& font, Patient* p, FileHandler& fh) {
	char amtBuf[30] = "";
	int len = 0;
	char msg[200] = "";
	int attempts = 0;

	while (win.isOpen()) {
		sf::Event e;
		while (win.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				win.close();
			}
			typeChar(e, amtBuf, 30, len);
			if (isClick(e, 290, 300, 150, 40)) {
				float amount = Validator::toFloat(amtBuf);
				if (amount <= 0.0f) {
					Validator::myCopy(msg, "InvalidInputException: Amount must be greater than 0.", 200);
					attempts++;
					if (attempts >= 3) {
						return; // Return to menu after 3 failed attempts
					}
				} else {
					// Add amount to patient balance
					*p += amount;
					
					// Update patient record in file
					char temp[50];
					Validator::floatToStr(p->getBal(), temp);
					fh.updField("data/patients.txt", p->getId(), 6, temp);
					
					char successMsg[200];
					snprintf(successMsg, sizeof(successMsg), "Balance updated. New balance: PKR %.2f", p->getBal());
					Validator::myCopy(msg, successMsg, 200);
					attempts = 0; // Reset attempts on success
				}
			}
			if (isClick(e, 460, 300, 150, 40)) {
				return;
			}
		}
		
		win.clear(sf::Color(18, 32, 65));
		drawTxt(win, "Top Up Balance", 335, 100, font, 28, sf::Color::White);
		drawTxt(win, "Enter amount to add (PKR):", 180, 200, font, 15, sf::Color(180, 200, 255));
		drawBox(win, 180, 225, 460, 40, amtBuf, font, true);
		drawBtn(win, 290, 300, 150, 40, "Top Up", font, sf::Color(20, 120, 80), sf::Color::White);
		drawBtn(win, 460, 300, 150, 40, "Back", font, sf::Color(60, 60, 80), sf::Color::White);
		if (msg[0] != '\0') {
			drawTxt(win, msg, 130, 370, font, 14, sf::Color(255, 180, 120));
		}
		win.display();
	}
}

void showPatientMenu(sf::RenderWindow& win, sf::Font& font, Patient* p, Storage<Doctor>& drs, Storage<Appointment>& apts, Storage<Bill>& bls, Storage<Prescription>& prs, FileHandler& fh) {
	while (win.isOpen()) {
		sf::Event e;
		while (win.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				win.close();
			}
			if (isClick(e, 300, 140, 300, 45)) {
				bookAppt(win, font, p, drs, apts, bls, fh);
			}
			if (isClick(e, 300, 200, 300, 45)) {
				cancelAppt(win, font, p, apts, drs, bls, fh);
			}
			if (isClick(e, 300, 260, 300, 45)) {
				viewMyAppointments(win, font, p, apts, drs);
			}
			if (isClick(e, 300, 320, 300, 45)) {
				viewMyMedicalRecords(win, font, p, prs, drs);
			}
			if (isClick(e, 300, 380, 300, 45)) {
				viewMyBills(win, font, p, bls);
			}
			if (isClick(e, 300, 440, 300, 45)) {
				payBill(win, font, p, bls, fh);
			}
			if (isClick(e, 300, 500, 300, 45)) {
				topUpBalance(win, font, p, fh);
			}
			if (isClick(e, 300, 560, 300, 40)) {
				return;
			}
		}
		win.clear(sf::Color(18, 32, 65));
		char hdr[200];
		snprintf(hdr, sizeof(hdr), "Welcome, %s\nBalance: PKR %.2f", p->getName(), p->getBal());
		drawTxt(win, hdr, 300, 60, font, 26, sf::Color::White);
		drawBtn(win, 300, 140, 300, 45, "1. Book Appointment", font, sf::Color(20, 70, 150), sf::Color::White);
		drawBtn(win, 300, 200, 300, 45, "2. Cancel Appointment", font, sf::Color(20, 100, 80), sf::Color::White);
		drawBtn(win, 300, 260, 300, 45, "3. View My Appointments", font, sf::Color(45, 75, 130), sf::Color::White);
		drawBtn(win, 300, 320, 300, 45, "4. View My Medical Records", font, sf::Color(45, 95, 130), sf::Color::White);
		drawBtn(win, 300, 380, 300, 45, "5. View My Bills", font, sf::Color(95, 75, 40), sf::Color::White);
		drawBtn(win, 300, 440, 300, 45, "6. Pay Bill", font, sf::Color(20, 120, 80), sf::Color::White);
		drawBtn(win, 300, 500, 300, 45, "7. Top Up Balance", font, sf::Color(20, 70, 150), sf::Color::White);
		drawBtn(win, 300, 560, 300, 40, "8. Logout", font, sf::Color(80, 20, 20), sf::Color::White);
		win.display();
	}
}
