#include "Validator.h"
using namespace std;

int Validator::myLen(const char* s) {
    int i = 0;
    while (s[i] != '\0') {
        i++;
    }
    return i;
}

void Validator::myCopy(char* dest, const char* src, int max) {
    int i = 0;
    while (src[i] != '\0' && i < max - 1) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

bool Validator::myEq(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) {
            return false;
        }
        i++;
    }
    return (a[i] == b[i]);
}

char Validator::toLow(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + 32;
    }
    return c;
}

bool Validator::myEqCI(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (toLow(a[i]) != toLow(b[i])) {
            return false;
        }
        i++;
    }
    return (a[i] == b[i]);
}

bool Validator::isDig(char c) {
    return (c >= '0' && c <= '9');
}

int Validator::toInt(const char* s) {
    int r = 0;
    int i = 0;
    while (s[i] != '\0' && isDig(s[i])) {
        r = r * 10 + (s[i] - '0');
        i++;
    }
    return r;
}

float Validator::toFloat(const char* s) {
    float r = 0.0f;
    int i = 0;

    while (s[i] != '\0' && s[i] != '.' && isDig(s[i])) {
        r = r * 10 + (s[i] - '0');
        i++;
    }

    if (s[i] == '.') {
        i++;
        float d = 0.1f;
        while (s[i] != '\0' && isDig(s[i])) {
            r += (s[i] - '0') * d;
            d *= 0.1f;
            i++;
        }
    }

    return r;
}

void Validator::intToStr(int n, char* buf) {
    if (n == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }

    char tmp[30];
    int i = 0;

    while (n > 0) {
        tmp[i] = '0' + (n % 10);
        n /= 10;
        i++;
    }

    int j = 0;
    while (i > 0) {
        i--;
        buf[j] = tmp[i];
        j++;
    }

    buf[j] = '\0';
}

void Validator::floatToStr(float f, char* buf) {
    int whole = (int)f;
    int dec = (int)((f - whole) * 100);

    if (dec < 0) {
        dec = -dec;
    }

    char tmp[20];
    intToStr(whole, tmp);

    int i = 0;
    while (tmp[i] != '\0') {
        buf[i] = tmp[i];
        i++;
    }

    buf[i] = '.';
    i++;

    if (dec < 10) {
        buf[i] = '0';
        i++;
    }

    char tmp2[10];
    intToStr(dec, tmp2);

    int j = 0;
    while (tmp2[j] != '\0') {
        buf[i] = tmp2[j];
        i++;
        j++;
    }

    buf[i] = '\0';
}
bool Validator::chkDate(const char* d) {
    if (myLen(d) != 10) {
        return false;
    }
    if (d[2] != '-' || d[5] != '-') {
        return false;
    }

    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) {
            continue;
        }
        if (!isDig(d[i])) {
            return false;
        }
    }

    int day = (d[0] - '0') * 10 + (d[1] - '0');
    int mon = (d[3] - '0') * 10 + (d[4] - '0');
    int yr = toInt(d + 6);

    if (mon < 1 || mon > 12) {
        return false;
    }
    if (yr < 1900 || yr > 9999) {
        return false;
    }

    int daysInMonth[12] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    bool leap = false;
    if ((yr % 4 == 0 && yr % 100 != 0) || (yr % 400 == 0)) {
        leap = true;
    }
    if (leap) {
        daysInMonth[1] = 29;
    }

    if (day < 1 || day > daysInMonth[mon - 1]) {
        return false;
    }

    return true;
}

bool Validator::chkSlot(const char* s) {
    const char* v[8] = {
        "09:00","10:00","11:00","12:00",
        "13:00","14:00","15:00","16:00"
    };
    for (int i = 0; i < 8; i++) {
        if (myEq(s, v[i])) {
            return true;
        }
    }
    return false;
}

bool Validator::chkContact(const char* c) {
    if (myLen(c) != 11) {
        return false;
    }
    for (int i = 0; i < 11; i++) {
        if (!isDig(c[i])) {
            return false;
        }
    }
    return true;
}

bool Validator::chkPass(const char* p) {
    return (myLen(p) >= 6);
}

bool Validator::chkFloat(const char* v) {
    return (toFloat(v) > 0.0f);
}

bool Validator::chkChoice(int c, int mn, int mx) {
    return (c >= mn && c <= mx);
}