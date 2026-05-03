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
    char msg[160] = "";
    int sLen = 0, dLen = 0, dtLen = 0, slLen = 0;
    int foc = 0;
    const char* slots[8] = { "09:00", "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00" };

    while (win.isOpen()) {
        sf::Event e;
        while (win.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                win.close();
            }
            if (foc == 0) typeChar(e, spec, 100, sLen);
            if (foc == 1) typeChar(e, didBuf, 20, dLen);
            if (foc == 2) typeChar(e, date, 15, dtLen);
            if (foc == 3) typeChar(e, slot, 10, slLen);
            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Tab) {
                foc = (foc + 1) % 4;
            }
            if (isClick(e, 250, 510, 180, 40)) {
                bool specOk = false;
                for (int i = 0; i < drs.size(); i++) {
                    if (Validator::myEqCI(drs.getAll()[i].getSpec(), spec)) {
                        specOk = true;
                    }
                }
                if (!specOk) {
                    Validator::myCopy(msg, "No doctor in this specialization.", 160);
                    continue;
                }
                if (!Validator::chkDate(date)) {
                    Validator::myCopy(msg, "Invalid date format DD-MM-YYYY.", 160);
                    continue;
                }
                if (!Validator::chkSlot(slot)) {
                    Validator::myCopy(msg, "Invalid slot. Use 09:00 to 16:00.", 160);
                    continue;
                }
                int did = Validator::toInt(didBuf);
                Doctor* dr = drs.findById(did);
                if (dr == nullptr || !Validator::myEqCI(dr->getSpec(), spec)) {
                    Validator::myCopy(msg, "Doctor ID mismatch for specialization.", 160);
                    continue;
                }

                Appointment candidate(nextApptId(apts), p->getId(), did, date, slot, "pending");
                bool conflict = false;
                for (int i = 0; i < apts.size(); i++) {
                    if (candidate == apts.getAll()[i]) {
                        conflict = true;
                    }
                }
                if (conflict) {
                    Validator::myCopy(msg, "Slot unavailable for this doctor/date.", 160);
                    continue;
                }

                if (p->getBal() < dr->getFee()) {
                    Validator::myCopy(msg, "Insufficient balance. Top up first.", 160);
                    continue;
                }

                *p -= dr->getFee();
                apts.add(candidate);
                fh.addAppt(candidate);
                Bill b(nextBillId(bls), p->getId(), candidate.getId(), dr->getFee(), "paid", date);
                bls.add(b);
                fh.addBill(b);
                Validator::myCopy(msg, "Appointment booked successfully.", 160);
            }
            if (isClick(e, 470, 510, 180, 40)) {
                return;
            }
        }

        win.clear(sf::Color(18, 32, 65));
        drawTxt(win, "Book Appointment", 315, 40, font, 28, sf::Color::White);
        drawTxt(win, "Specialization", 130, 120, font, 14, sf::Color(180, 200, 255));
        drawBox(win, 130, 145, 620, 38, spec, font, foc == 0);
        drawTxt(win, "Doctor ID", 130, 200, font, 14, sf::Color(180, 200, 255));
        drawBox(win, 130, 225, 620, 38, didBuf, font, foc == 1);
        drawTxt(win, "Date (DD-MM-YYYY)", 130, 280, font, 14, sf::Color(180, 200, 255));
        drawBox(win, 130, 305, 620, 38, date, font, foc == 2);
        drawTxt(win, "Time Slot", 130, 360, font, 14, sf::Color(180, 200, 255));
        drawBox(win, 130, 385, 620, 38, slot, font, foc == 3);

        char slotMsg[200] = "Available slots:";
        for (int i = 0; i < 8; i++) {
            char temp[20];
            Validator::myCopy(temp, " ", 20);
            Validator::myCopy(temp, slots[i], 20);
        }
        drawTxt(win, "Available slots: 09:00 10:00 11:00 12:00 13:00 14:00 15:00 16:00", 130, 440, font, 13, sf::Color(180, 210, 220));

        drawBtn(win, 250, 510, 180, 40, "Submit", font, sf::Color(30, 90, 170), sf::Color::White);
        drawBtn(win, 470, 510, 180, 40, "Back", font, sf::Color(70, 70, 90), sf::Color::White);
        if (msg[0] != '\0') {
            drawTxt(win, msg, 130, 565, font, 14, sf::Color(255, 180, 120));
        }
        win.display();
    }
}

void cancelAppt(sf::RenderWindow& win, sf::Font& font, Patient* p, Storage<Appointment>& apts, FileHandler& fh) {
    char idBuf[20] = "";
    int len = 0;
    char msg[120] = "";
    while (win.isOpen()) {
        sf::Event e;
        while (win.pollEvent(e)) {
            if (e.type == sf::Event::Closed) win.close();
            typeChar(e, idBuf, 20, len);
            if (isClick(e, 290, 300, 150, 40)) {
                int id = Validator::toInt(idBuf);
                bool done = false;
                for (int i = 0; i < apts.size(); i++) {
                    Appointment& a = apts.getAll()[i];
                    if (a.getId() == id && a.getPid() == p->getId()) {
                        a.setStat("cancelled");
                        fh.updField("data/appointments.txt", id, 5, "cancelled");
                        Validator::myCopy(msg, "Appointment cancelled.", 120);
                        done = true;
                    }
                }
                if (!done) Validator::myCopy(msg, "Appointment not found.", 120);
            }
            if (isClick(e, 460, 300, 150, 40)) return;
        }
        win.clear(sf::Color(18, 32, 65));
        drawTxt(win, "Cancel Appointment", 300, 100, font, 28, sf::Color::White);
        drawTxt(win, "Appointment ID", 180, 200, font, 15, sf::Color(180, 200, 255));
        drawBox(win, 180, 225, 460, 40, idBuf, font, true);
        drawBtn(win, 290, 300, 150, 40, "Cancel", font, sf::Color(120, 40, 40), sf::Color::White);
        drawBtn(win, 460, 300, 150, 40, "Back", font, sf::Color(60, 60, 80), sf::Color::White);
        if (msg[0] != '\0') drawTxt(win, msg, 180, 370, font, 14, sf::Color(255, 170, 120));
        win.display();
    }
}

void viewApts(sf::RenderWindow& win, sf::Font& font, Patient* p, Storage<Appointment>& apts) {
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
        drawTxt(win, "My Appointments", 320, 30, font, 26, sf::Color::White);
        drawBtn(win, 20, 20, 100, 35, "Back", font, sf::Color(70, 70, 90), sf::Color::White);
        int y = 90 - offset;
        for (int i = 0; i < apts.size(); i++) {
            Appointment& a = apts.getAll()[i];
            if (a.getPid() != p->getId()) continue;
            char row[300];
            snprintf(row, sizeof(row), "ID:%d  Doctor:%d  %s %s  [%s]", a.getId(), a.getDid(), a.getDate(), a.getSlot(), a.getStat());
            drawTxt(win, row, 60, (float)y, font, 16, sf::Color(210, 220, 240));
            y += 35;
        }
        win.display();
    }
}

void viewRecords(sf::RenderWindow& win, sf::Font& font, Patient* p, Storage<Prescription>& prs) {
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
        drawTxt(win, "My Medical Records", 280, 30, font, 26, sf::Color::White);
        drawBtn(win, 20, 20, 100, 35, "Back", font, sf::Color(70, 70, 90), sf::Color::White);
        int y = 90 - offset;
        for (int i = 0; i < prs.size(); i++) {
            Prescription& r = prs.getAll()[i];
            if (r.getPid() != p->getId()) continue;
            char row[900];
            snprintf(row, sizeof(row), "Presc:%d Date:%s Doc:%d  Meds:%s  Notes:%s", r.getId(), r.getDate(), r.getDid(), r.getMeds(), r.getNotes());
            drawTxt(win, row, 30, (float)y, font, 14, sf::Color(210, 220, 240));
            y += 35;
        }
        win.display();
    }
}

void viewBills(sf::RenderWindow& win, sf::Font& font, Patient* p, Storage<Bill>& bls) {
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
        drawTxt(win, "My Bills", 380, 30, font, 26, sf::Color::White);
        drawBtn(win, 20, 20, 100, 35, "Back", font, sf::Color(70, 70, 90), sf::Color::White);
        int y = 90 - offset;
        for (int i = 0; i < bls.size(); i++) {
            Bill& b = bls.getAll()[i];
            if (b.getPid() != p->getId()) continue;
            char row[300];
            snprintf(row, sizeof(row), "Bill:%d  Appt:%d  Amount:%.2f  Status:%s  Date:%s", b.getId(), b.getApid(), b.getAmount(), b.getStat(), b.getDate());
            drawTxt(win, row, 45, (float)y, font, 16, sf::Color(210, 220, 240));
            y += 35;
        }
        win.display();
    }
}

void payBill(sf::RenderWindow& win, sf::Font& font, Patient* p, Storage<Bill>& bls, FileHandler& fh) {
    char idBuf[20] = "";
    int len = 0;
    char msg[130] = "";
    while (win.isOpen()) {
        sf::Event e;
        while (win.pollEvent(e)) {
            if (e.type == sf::Event::Closed) win.close();
            typeChar(e, idBuf, 20, len);
            if (isClick(e, 290, 300, 150, 40)) {
                int bid = Validator::toInt(idBuf);
                bool ok = false;
                for (int i = 0; i < bls.size(); i++) {
                    Bill& b = bls.getAll()[i];
                    if (b.getId() == bid && b.getPid() == p->getId()) {
                        if (Validator::myEq(b.getStat(), "paid")) {
                            Validator::myCopy(msg, "Bill already paid.", 130);
                            ok = true;
                            break;
                        }
                        if (p->getBal() < b.getAmount()) {
                            Validator::myCopy(msg, "Insufficient balance.", 130);
                            ok = true;
                            break;
                        }
                        *p -= b.getAmount();
                        b.setStat("paid");
                        fh.updField("data/bills.txt", b.getId(), 4, "paid");
                        Validator::myCopy(msg, "Bill paid successfully.", 130);
                        ok = true;
                    }
                }
                if (!ok) Validator::myCopy(msg, "Bill not found.", 130);
            }
            if (isClick(e, 460, 300, 150, 40)) return;
        }
        win.clear(sf::Color(18, 32, 65));
        drawTxt(win, "Pay Bill", 390, 100, font, 28, sf::Color::White);
        drawTxt(win, "Bill ID", 180, 200, font, 15, sf::Color(180, 200, 255));
        drawBox(win, 180, 225, 460, 40, idBuf, font, true);
        drawBtn(win, 290, 300, 150, 40, "Pay", font, sf::Color(20, 120, 80), sf::Color::White);
        drawBtn(win, 460, 300, 150, 40, "Back", font, sf::Color(60, 60, 80), sf::Color::White);
        if (msg[0] != '\0') drawTxt(win, msg, 180, 370, font, 14, sf::Color(255, 170, 120));
        win.display();
    }
}

void topUp(sf::RenderWindow& win, sf::Font& font, Patient* p) {
    char amtBuf[30] = "";
    int len = 0;
    char msg[120] = "";
    while (win.isOpen()) {
        sf::Event e;
        while (win.pollEvent(e)) {
            if (e.type == sf::Event::Closed) win.close();
            typeChar(e, amtBuf, 30, len);
            if (isClick(e, 290, 300, 150, 40)) {
                float v = Validator::toFloat(amtBuf);
                if (v > 0.0f) {
                    *p += v;
                    Validator::myCopy(msg, "Balance topped up.", 120);
                }
                else {
                    Validator::myCopy(msg, "Enter valid amount.", 120);
                }
            }
            if (isClick(e, 460, 300, 150, 40)) return;
        }
        win.clear(sf::Color(18, 32, 65));
        drawTxt(win, "Top Up Balance", 335, 100, font, 28, sf::Color::White);
        drawTxt(win, "Amount", 180, 200, font, 15, sf::Color(180, 200, 255));
        drawBox(win, 180, 225, 460, 40, amtBuf, font, true);
        drawBtn(win, 290, 300, 150, 40, "Top Up", font, sf::Color(20, 120, 80), sf::Color::White);
        drawBtn(win, 460, 300, 150, 40, "Back", font, sf::Color(60, 60, 80), sf::Color::White);
        if (msg[0] != '\0') drawTxt(win, msg, 180, 370, font, 14, sf::Color(255, 170, 120));
        win.display();
    }
}

void showPatientMenu(sf::RenderWindow& win, sf::Font& font, Patient* p, Storage<Doctor>& drs, Storage<Appointment>& apts, Storage<Bill>& bls, Storage<Prescription>& prs, FileHandler& fh) {
    while (win.isOpen()) {
        sf::Event e;
        while (win.pollEvent(e)) {
            if (e.type == sf::Event::Closed) win.close();
            if (isClick(e, 120, 180, 300, 45)) bookAppt(win, font, p, drs, apts, bls, fh);
            if (isClick(e, 120, 240, 300, 45)) cancelAppt(win, font, p, apts, fh);
            if (isClick(e, 120, 300, 300, 45)) viewApts(win, font, p, apts);
            if (isClick(e, 120, 360, 300, 45)) viewRecords(win, font, p, prs);
            if (isClick(e, 470, 180, 300, 45)) viewBills(win, font, p, bls);
            if (isClick(e, 470, 240, 300, 45)) payBill(win, font, p, bls, fh);
            if (isClick(e, 470, 300, 300, 45)) topUp(win, font, p);
            if (isClick(e, 470, 360, 300, 45)) return;
        }
        win.clear(sf::Color(18, 32, 65));
        char hdr[180];
        snprintf(hdr, sizeof(hdr), "Patient Menu - %s (Balance: %.2f)", p->getName(), p->getBal());
        drawTxt(win, hdr, 145, 80, font, 24, sf::Color::White);
        drawBtn(win, 120, 180, 300, 45, "Book Appointment", font, sf::Color(20, 70, 150), sf::Color::White);
        drawBtn(win, 120, 240, 300, 45, "Cancel Appointment", font, sf::Color(90, 50, 50), sf::Color::White);
        drawBtn(win, 120, 300, 300, 45, "View Appointments", font, sf::Color(45, 75, 130), sf::Color::White);
        drawBtn(win, 120, 360, 300, 45, "View Records", font, sf::Color(45, 95, 130), sf::Color::White);
        drawBtn(win, 470, 180, 300, 45, "View Bills", font, sf::Color(95, 75, 40), sf::Color::White);
        drawBtn(win, 470, 240, 300, 45, "Pay Bill", font, sf::Color(20, 100, 80), sf::Color::White);
        drawBtn(win, 470, 300, 300, 45, "Top Up Balance", font, sf::Color(35, 120, 90), sf::Color::White);
        drawBtn(win, 470, 360, 300, 45, "Logout", font, sf::Color(80, 20, 20), sf::Color::White);
        win.display();
    }
}
