#include "Screens.h"
using namespace std;

void drawTxt(sf::RenderWindow& w, const char* t, float x, float y, const sf::Font& f, int sz, sf::Color c) {
    sf::Text tx;
    tx.setFont(f);
    tx.setString(t);
    tx.setCharacterSize(sz);
    tx.setFillColor(c);
    tx.setPosition(x, y);
    w.draw(tx);
}

void drawBtn(sf::RenderWindow& w, float x, float y, float bw, float bh, const char* lbl, const sf::Font& f, sf::Color bg, sf::Color tc) {
    sf::Vector2i mp = sf::Mouse::getPosition(w);
    bool hov = (mp.x >= x && mp.x <= x + bw && mp.y >= y && mp.y <= y + bh);
    sf::RectangleShape r(sf::Vector2f(bw, bh));
    r.setPosition(x, y);
    if (hov) {
        r.setFillColor(sf::Color(180, 200, 230));
    }
    else {
        r.setFillColor(bg);
    }
    r.setOutlineColor(sf::Color(100, 140, 200));
    r.setOutlineThickness(1.5f);
    w.draw(r);
    sf::Text tx;
    tx.setFont(f);
    tx.setString(lbl);
    tx.setCharacterSize(16);
    tx.setFillColor(tc);
    sf::FloatRect tb = tx.getLocalBounds();
    tx.setPosition(x + (bw - tb.width) / 2 - tb.left, y + (bh - tb.height) / 2 - tb.top);
    w.draw(tx);
}

void drawBox(sf::RenderWindow& w, float x, float y, float bw, float bh, const char* txt, const sf::Font& f, bool focused) {
    sf::RectangleShape r(sf::Vector2f(bw, bh));
    r.setPosition(x, y);
    r.setFillColor(sf::Color(240, 245, 255));
    if (focused) {
        r.setOutlineColor(sf::Color(30, 100, 200));
        r.setOutlineThickness(2.0f);
    }
    else {
        r.setOutlineColor(sf::Color(140, 160, 190));
        r.setOutlineThickness(1.0f);
    }
    w.draw(r);
    sf::Text tx;
    tx.setFont(f);
    tx.setString(txt);
    tx.setCharacterSize(15);
    tx.setFillColor(sf::Color(20, 20, 20));
    tx.setPosition(x + 8, y + (bh - 18) / 2);
    w.draw(tx);
}

bool isHover(sf::RenderWindow& w, float x, float y, float bw, float bh) {
    sf::Vector2i mp = sf::Mouse::getPosition(w);
    if (mp.x >= x && mp.x <= x + bw && mp.y >= y && mp.y <= y + bh) {
        return true;
    }
    else {
        return false;
    }
}

bool isClick(sf::Event& e, float x, float y, float bw, float bh) {
    if (e.type != sf::Event::MouseButtonReleased) {
        return false;
    }
    if (e.mouseButton.button != sf::Mouse::Left) {
        return false;
    }
    float mx = (float)e.mouseButton.x;
    float my = (float)e.mouseButton.y;
    if (mx >= x && mx <= x + bw && my >= y && my <= y + bh) {
        return true;
    }
    else {
        return false;
    }
}

void typeChar(sf::Event& e, char* buf, int max, int& len) {
    if (e.type != sf::Event::TextEntered) {
        return;
    }
    char c = (char)e.text.unicode;
    if (c == '\b') {
        if (len > 0) {
            len--;
            buf[len] = '\0';
        }
    }
    else if (c >= ' ' && len < max - 1) {
        buf[len] = c;
        len++;
        buf[len] = '\0';
    }
}
