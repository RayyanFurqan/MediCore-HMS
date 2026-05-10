// UI helper functions for SFML graphics
#include "Screens.h"
using namespace std;

// Draw text on screen
void drawTxt(sf::RenderWindow& w, const char* t, float x, float y, const sf::Font& f, int sz, sf::Color c) {
	sf::Text tx;
	tx.setFont(f);
	tx.setString(t);
	tx.setCharacterSize(sz);
	tx.setFillColor(c);
	tx.setPosition(x, y);
	w.draw(tx);
}

// Draw button with hover effect
void drawBtn(sf::RenderWindow& w, float x, float y, float bw, float bh, const char* lbl, const sf::Font& f, sf::Color bg, sf::Color tc) {
	int mp_x = sf::Mouse::getPosition(w).x;
	int mp_y = sf::Mouse::getPosition(w).y;
	bool hov = (mp_x >= x && mp_x <= x + bw && mp_y >= y && mp_y <= y + bh);
	sf::RectangleShape r;
	r.setSize(sf::Vector2f(bw, bh));
	r.setPosition(x, y);
	r.setFillColor(hov ? sf::Color(180, 200, 230) : bg);
	r.setOutlineColor(sf::Color(100, 140, 200));
	r.setOutlineThickness(1.5f);
	w.draw(r);
	sf::Text tx;
	tx.setFont(f);
	tx.setString(lbl);
	tx.setCharacterSize(16);
	tx.setFillColor(tc);
	tx.setPosition(x + 10, y + 10);
	w.draw(tx);
}

// Draw input box
void drawBox(sf::RenderWindow& w, float x, float y, float bw, float bh, const char* txt, const sf::Font& f, bool focused) {
	sf::RectangleShape r;
	r.setSize(sf::Vector2f(bw, bh));
	r.setPosition(x, y);
	r.setFillColor(sf::Color(0, 255, 255));
	r.setOutlineColor(focused ? sf::Color(30, 100, 200) : sf::Color(140, 160, 190));
	r.setOutlineThickness(focused ? 2.0f : 1.0f);
	w.draw(r);
	sf::Text tx;
	tx.setFont(f);
	tx.setString(txt);
	tx.setCharacterSize(15);
	tx.setFillColor(sf::Color(20, 20, 20));
	tx.setPosition(x + 8, y + 8);
	w.draw(tx);
}

// Check if mouse is over something
bool isHover(sf::RenderWindow& w, float x, float y, float bw, float bh) {
	int mp_x = sf::Mouse::getPosition(w).x;
	int mp_y = sf::Mouse::getPosition(w).y;
	return (mp_x >= x && mp_x <= x + bw && mp_y >= y && mp_y <= y + bh);
}

// Check if mouse clicked
bool isClick(sf::Event& e, float x, float y, float bw, float bh) {
	if (e.type != sf::Event::MouseButtonReleased || e.mouseButton.button != sf::Mouse::Left) {
		return false;
	}
	float mx = (float)e.mouseButton.x;
	float my = (float)e.mouseButton.y;
	return (mx >= x && mx <= x + bw && my >= y && my <= y + bh);
}

// Handle keyboard typing
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