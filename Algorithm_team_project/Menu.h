#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>

using namespace std;

class Menu {
public:
	int menu_num;
	string menu_name;
	vector<int> processes;

	Menu(int menu_num, string menu_name, vector<int> processes);
};

#endif // MENU_H
