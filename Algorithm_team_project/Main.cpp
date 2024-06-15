//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <string>
//#include <sstream>
//#include <unordered_map>
//#include "Menu.h"
//#include "Process.h"
//
//using namespace std;
//
//void readMenus1(const string &filename, vector<Menu> &menus);
//void readProcesses1(const string &filename, unordered_map<int, Process*> &processes);
//
//
//int main() {
//	vector<Menu> menus;
//	unordered_map<int, Process*> processes;
//
//	// 파일 읽기
//	readMenus1("menu.txt", menus);
//	readProcesses1("process.txt", processes);
//
//	// 읽은 데이터 출력
//	for (const auto &menu : menus) {
//		cout << "Menu Num: " << menu.menu_num << ", Menu Name: " << menu.menu_name << ", Processes: ";
//		for (const auto &process : menu.processes) {
//			cout << "process" << process << " ";
//		}
//		cout << endl;
//	}
//
//	for (const auto &pair : processes) {
//		const auto &process = pair.second;
//		cout << "Process Num: " << process->process_num
//			<< ", Name: " << process->process_name
//			<< ", Time: " << process->time
//			<< ", Previous: " << (process->previous_process ? "process" + to_string(process->previous_process->process_num) : "null")
//			<< ", isMachine: " << process->isMachine << endl;
//	}
//
//	// 메모리 해제
//	for (const auto &pair : processes) {
//		delete pair.second;
//	}
//
//	return 0;
//}
//
//
//void readMenus1(const string &filename, vector<Menu> &menus) {
//	ifstream file(filename);
//	string line;
//	int menu_num;
//	string menu_name;
//	vector<int> processes;
//
//	while (getline(file, line)) {
//		if (line.find("menu_num") != string::npos) {
//			if (!processes.empty()) {
//				menus.emplace_back(menu_num, menu_name, processes);
//				processes.clear();
//			}
//			menu_num = stoi(line.substr(line.find('=') + 1));
//		}
//		else if (line.find("menu_name") != string::npos) {
//			menu_name = line.substr(line.find('=') + 1);
//		}
//		else if (line.find("processes") != string::npos) {
//			string process_list = line.substr(line.find('=') + 1);
//			stringstream ss(process_list);
//			string process;
//			while (getline(ss, process, ',')) {
//				if (!process.empty()) {
//					processes.push_back(stoi(process.substr(7))); // "process" 제거
//				}
//			}
//		}
//	}
//
//	if (!processes.empty()) {
//		menus.emplace_back(menu_num, menu_name, processes);
//	}
//
//	file.close();
//}
//
//
//void readProcesses1(const string &filename, unordered_map<int, Process*> &processes) {
//	ifstream file(filename);
//	string line;
//	int time;
//	string previous_process_name;
//	bool isMachine;
//	string process_name;
//	int process_num;
//
//	while (getline(file, line)) {
//		if (line.find("time") != string::npos) {
//			time = stoi(line.substr(line.find('=') + 1));
//		}
//		else if (line.find("previous_process") != string::npos) {
//			previous_process_name = line.substr(line.find('=') + 1);
//			if (previous_process_name == "null") {
//				previous_process_name = "";
//			}
//		}
//		else if (line.find("isMachine") != string::npos) {
//			isMachine = stoi(line.substr(line.find('=') + 1));
//		}
//		else if (line.find("process_name") != string::npos) {
//			process_name = line.substr(line.find('=') + 1);
//		}
//		else if (line.find("process_num") != string::npos) {
//			process_num = stoi(line.substr(line.find('=') + 1));
//			Process* previous_process = nullptr;
//			if (!previous_process_name.empty()) {
//				int previous_process_num = stoi(previous_process_name.substr(7)); // "process" 제거
//				previous_process = processes[previous_process_num];
//			}
//			processes[process_num] = new Process(time, previous_process, isMachine, process_name, process_num);
//		}
//	}
//
//	file.close();
//}