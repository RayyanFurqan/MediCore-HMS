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

void dailyReport(sf::RenderWindow& win, sf::Font& font, Storage<Patient>& pts, Storage<Doctor>& drs, Storage<Appointment>& apts, Storage<Bill>& bls, Storage<Prescription>& prs) {
    while (win.isOpen()) {
        sf::Event e;
        while (win.pollEvent(e)) {
            if (e.type == sf::Event::Closed) win.close();
            if (isClick(e, 350, 520, 180, 40)) return;
        }
        int pending = 0;
        int unpaid = 0;
        for (int i = 0; i < apts.size(); i++) if (Validator::myEq(apts.getAll()[i].getStat(), "pending")) pending++;
        for (int i = 0; i < bls.size(); i++) if (!Validator::myEq(bls.getAll()[i].getStat(), "paid")) unpaid++;

        char r1[80], r2[80], r3[80], r4[80], r5[80], r6[80];
        snprintf(r1, sizeof(r1), "Patients: %d", pts.size());
        snprintf(r2, sizeof(r2), "Doctors: %d", drs.size());
        snprintf(r3, sizeof(r3), "Appointments: %d", apts.size());
        snprintf(r4, sizeof(r4), "Pending Appointments: %d", pending);
        snprintf(r5, sizeof(r5), "Bills Unpaid: %d", unpaid);
        snprintf(r6, sizeof(r6), "Prescriptions: %d", prs.size());

        win.clear(sf::Color(18, 32, 65));
        drawTxt(win, "Daily Report", 340, 80, font, 30, sf::Color::White);
        drawTxt(win, r1, 250, 180, font, 20, sf::Color(210, 220, 240));
        drawTxt(win, r2, 250, 220, font, 20, sf::Color(210, 220, 240));
        drawTxt(win, r3, 250, 260, font, 20, sf::Color(210, 220, 240));
        drawTxt(win, r4, 250, 300, font, 20, sf::Color(210, 220, 240));
        drawTxt(win, r5, 250, 340, font, 20, sf::Color(210, 220, 240));
        drawTxt(win, r6, 250, 380, font, 20, sf::Color(210, 220, 240));
        drawBtn(win, 350, 520, 180, 40, "Back", font, sf::Color(70, 70, 90), sf::Color::White);
        win.display();
    }
}

void showAdminMenu(sf::RenderWindow& win, sf::Font& font, Admin* a, Storage<Patient>& pts, Storage<Doctor>& drs, Storage<Appointment>& apts, Storage<Bill>& bls, Storage<Prescription>& prs, FileHandler& fh) {
    while (win.isOpen()) {
        sf::Event e;
        while (win.pollEvent(e)) {
            if (e.type == sf::Event::Closed) win.close();
            if (isClick(e, 90, 180, 230, 45)) addPatientAdmin(win, font, pts, fh);
            if (isClick(e, 90, 240, 230, 45)) addDoctorAdmin(win, font, drs, fh);
            if (isClick(e, 90, 300, 230, 45)) removePatientAdmin(win, font, pts, fh);
            if (isClick(e, 90, 360, 230, 45)) removeDoctorAdmin(win, font, drs, fh);
            if (isClick(e, 335, 180, 230, 45)) viewAllApts(win, font, apts);
            if (isClick(e, 335, 240, 230, 45)) viewAllBills(win, font, bls);
            if (isClick(e, 335, 300, 230, 45)) dischargePatientAdmin(win, font, pts, apts, bls, fh);
            if (isClick(e, 335, 360, 230, 45)) dailyReport(win, font, pts, drs, apts, bls, prs);
            if (isClick(e, 580, 180, 230, 45)) return;
        }
        win.clear(sf::Color(18, 32, 65));
        char hdr[180];
        snprintf(hdr, sizeof(hdr), "Admin Menu - %s", a->getName());
        drawTxt(win, hdr, 300, 90, font, 26, sf::Color::White);
        drawBtn(win, 90, 180, 230, 45, "Add Patient", font, sf::Color(20, 70, 150), sf::Color::White);
        drawBtn(win, 90, 240, 230, 45, "Add Doctor", font, sf::Color(20, 100, 80), sf::Color::White);
        drawBtn(win, 90, 300, 230, 45, "Remove Patient", font, sf::Color(120, 70, 20), sf::Color::White);
        drawBtn(win, 90, 360, 230, 45, "Remove Doctor", font, sf::Color(120, 70, 20), sf::Color::White);
        drawBtn(win, 335, 180, 230, 45, "View Appointments", font, sf::Color(45, 75, 130), sf::Color::White);
        drawBtn(win, 335, 240, 230, 45, "View Bills", font, sf::Color(95, 75, 40), sf::Color::White);
        drawBtn(win, 335, 300, 230, 45, "Discharge Patient", font, sf::Color(120, 80, 20), sf::Color::White);
        drawBtn(win, 335, 360, 230, 45, "Daily Report", font, sf::Color(45, 95, 130), sf::Color::White);
        drawBtn(win, 580, 180, 230, 45, "Logout", font, sf::Color(80, 20, 20), sf::Color::White);
        win.display();
    }
}
