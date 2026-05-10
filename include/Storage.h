#pragma once
#include <iostream>
using namespace std;

// Holds up to 100 items of any type
template <typename T>
class Storage {
private:
	T* data;  // Array to store items
	int cnt;  // Number of items stored
public:
	Storage() {
		data = new T[100];
		cnt = 0;
	}

	~Storage() {
		delete[] data;
	}

	void add(const T& item) {
		if (cnt < 100) {
			data[cnt] = item;
			cnt++;
		}
		else {
			cout << "Storage full." << endl;
		}
	}

	T* findById(int id) {
		for (int i = 0; i < cnt; i++) {
			if (data[i].getId() == id) {
				return &data[i];
			}
		}
		return nullptr;
	}

	bool removeById(int id) {
		int pos = -1;
		for (int i = 0; i < cnt; i++) {
			if (data[i].getId() == id) {
				pos = i;
			}
		}
		if (pos == -1) {
			return false;
		}
		for (int j = pos; j < cnt - 1; j++) {
			data[j] = data[j + 1];
		}
		cnt--;
		return true;
	}

	T* getAll() {
		return data;
	}

	int size() {
		return cnt;
	}

	void clear() {
		cnt = 0;
	}
};
