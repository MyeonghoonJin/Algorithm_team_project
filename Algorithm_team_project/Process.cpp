#include "Process.h"

Process::Process(int time, Process* previous_process, bool isMachine, string process_name, int process_num)
	: time(time), previous_process(previous_process), isMachine(isMachine), process_name(process_name), process_num(process_num) {}
