#ifndef PROCESS_H
#define PROCESS_H

#include <string>

using namespace std;

class Process {
public:
	int time;
	Process* previous_process;
	bool isMachine;
	string process_name;
	int process_num;

	Process(int time, Process* previous_process, bool isMachine, string process_name, int process_num);
};

#endif // PROCESS_H