#pragma once

class Validator {
public:
	static int myLen(const char* s);
	static void myCopy(char* dest, const char* src, int max);
	static bool myEq(const char* a, const char* b);
	static bool myEqCI(const char* a, const char* b);
	static char toLow(char c);
	static bool isDig(char c);
	static int toInt(const char* s);
	static float toFloat(const char* s);
	static void intToStr(int n, char* buf);
	static void floatToStr(float f, char* buf);
	static bool chkDate(const char* d);
	static bool chkSlot(const char* s);
	static bool chkContact(const char* c);
	static bool chkPass(const char* p);
	static bool chkFloat(const char* v);
	static bool chkChoice(int c, int mn, int mx);
	static int myCmp(const char* s1, const char* s2);
	static void toLower(const char* src, char* dest);
};