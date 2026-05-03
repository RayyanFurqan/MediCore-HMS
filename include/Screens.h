#pragma once
#include <SFML/Graphics.hpp>
#include "Validator.h"

//draw helpers used across all screens
void drawTxt(sf::RenderWindow& w, const char* t, float x, float y, const sf::Font& f, int sz, sf::Color c);

void drawBtn(sf::RenderWindow& w, float x, float y, float bw, float bh, const char* lbl, const sf::Font& f, sf::Color bg, sf::Color tc);

void drawBox(sf::RenderWindow& w, float x, float y, float bw, float bh, const char* txt, const sf::Font& f, bool focused);

bool isHover(sf::RenderWindow& w, float x, float y, float bw, float bh);

bool isClick(sf::Event& e, float x, float y, float bw, float bh);

void typeChar(sf::Event& e, char* buf, int max, int& len);
