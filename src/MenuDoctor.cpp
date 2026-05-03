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
#include <cstdio>
using namespace std;

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
            if (e.type == sf::Event::Closed) win.close();
            if (e.type == sf::Event::MouseWheelScrolled) { offset -= (int)e.mouseWheelScroll.delta * 30; }
            if (isClick(e, 20, 20, 100, 35)) return;
        }
        if (offset < 0) offset = 0;
        win.clear(sf::Color(18, 32, 65));
        drawTxt(win, "Today's Appointments", 270, 30, font, 26, sf::Color::White);
        drawBtn(win, 20, 20, 100, 35, "Back", font, sf::Color(70, 70, 90), sf::Color::White);
        int y = 90 - offset;
        for (int i = 0; i < apts.size(); i++) {
            Appointment& a = apts.getAll()[i];
            if (a.getDid() == d->getId() && Validator::myEq(a.getDate(), today)) {
                char row[300];
                snprintf(row, sizeof(row), "ID:%d  Patient:%d  %s  Status:%s", a.getId(), a.getPid(), a.getSlot(), a.getStat());
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
            if (e.type == sf::Event::Closed) win.close();
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
            if (isClick(e, 460, 300, 150, 40)) return;
        }
        win.clear(sf::Color(18, 32, 65));
        drawTxt(win, "Mark Done", 375, 100, font, 28, sf::Color::White);
        drawBox(win, 180, 225, 460, 40, idBuf, font, true);
        drawBtn(win, 290, 300, 150, 40, "Done", font, sf::Color(20, 120, 80), sf::Color::White);
        drawBtn(win, 460, 300, 150, 40, "Back", font, sf::Color(60, 60, 80), sf::Color::White);
        if (msg[0] != '\0') drawTxt(win, msg, 180, 370, font, 14, sf::Color(255, 180, 120));
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
            if (e.type == sf::Event::Closed) win.close();
            typeChar(e, idBuf, 20, len);
            if (isClick(e, 290, 300, 150, 40)) {
                int id = Validator::toInt(idBuf);
                bool ok = false;
                for (int i = 0; i < apts.size(); i++) {
                    if (apts.getAll()[i].getId() == id && apts.getAll()[i].getDid() == d->getId()) {
                        apts.getAll()[i].setStat("no-show");
                        fh.updField("data/appointments.txt", id, 5, "no-show");
                        Validator::myCopy(msg, "Marked as no-show.", 120);
                        ok = true;
                    }
                }
                if (!ok) Validator::myCopy(msg, "Appointment not found.", 120);
            }
            if (isClick(e, 460, 300, 150, 40)) return;
        }
        win.clear(sf::Color(18, 32, 65));
        drawTxt(win, "Mark No-Show", 340, 100, font, 28, sf::Color::White);
        drawBox(win, 180, 225, 460, 40, idBuf, font, true);
        drawBtn(win, 290, 300, 150, 40, "No-Show", font, sf::Color(120, 80, 20), sf::Color::White);
        drawBtn(win, 460, 300, 150, 40, "Back", font, sf::Color(60, 60, 80), sf::Color::White);
        if (msg[0] != '\0') drawTxt(win, msg, 180, 370, font, 14, sf::Color(255, 180, 120));
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
            if (e.type == sf::Event::Closed) win.close();
            if (foc == 0) typeChar(e, apidBuf, 20, aLen);
            if (foc == 1) typeChar(e, meds, 500, mLen);
            if (foc == 2) typeChar(e, notes, 300, nLen);
            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Tab) foc = (foc + 1) % 3;
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
                    char dt[15];
                    getToday(dt);
                    Prescription p(nextPrescId(prs), a->getId(), a->getPid(), d->getId(), dt, meds, notes);
                    prs.add(p);
                    fh.addPresc(p);
                    Validator::myCopy(msg, "Prescription saved.", 120);
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
        if (msg[0] != '\0') drawTxt(win, msg, 130, 565, font, 14, sf::Color(255, 180, 120));
        win.display();
    }
}

void viewHist(sf::RenderWindow& win, sf::Font& font, Doctor* d, Storage<Appointment>& apts) {
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
        drawTxt(win, "Appointment History", 275, 30, font, 26, sf::Color::White);
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

void showDoctorMenu(sf::RenderWindow& win, sf::Font& font, Doctor* d, Storage<Patient>& pts, Storage<Appointment>& apts, Storage<Prescription>& prs, FileHandler& fh) {
    (void)pts;
    while (win.isOpen()) {
        sf::Event e;
        while (win.pollEvent(e)) {
            if (e.type == sf::Event::Closed) win.close();
            if (isClick(e, 300, 190, 300, 45)) viewToday(win, font, d, apts);
            if (isClick(e, 300, 250, 300, 45)) markDone(win, font, d, apts, fh);
            if (isClick(e, 300, 310, 300, 45)) markNoShow(win, font, d, apts, fh);
            if (isClick(e, 300, 370, 300, 45)) writePresc(win, font, d, apts, prs, fh);
            if (isClick(e, 300, 430, 300, 45)) viewHist(win, font, d, apts);
            if (isClick(e, 300, 490, 300, 40)) return;
        }
        win.clear(sf::Color(18, 32, 65));
        char hdr[180];
        snprintf(hdr, sizeof(hdr), "Doctor Menu - Dr. %s", d->getName());
        drawTxt(win, hdr, 250, 90, font, 26, sf::Color::White);
        drawBtn(win, 300, 190, 300, 45, "View Today", font, sf::Color(20, 70, 150), sf::Color::White);
        drawBtn(win, 300, 250, 300, 45, "Mark Done", font, sf::Color(20, 120, 80), sf::Color::White);
        drawBtn(win, 300, 310, 300, 45, "Mark No-Show", font, sf::Color(120, 80, 20), sf::Color::White);
        drawBtn(win, 300, 370, 300, 45, "Write Prescription", font, sf::Color(45, 95, 130), sf::Color::White);
        drawBtn(win, 300, 430, 300, 45, "View History", font, sf::Color(70, 70, 120), sf::Color::White);
        drawBtn(win, 300, 490, 300, 40, "Logout", font, sf::Color(80, 20, 20), sf::Color::White);
        win.display();
    }
}
