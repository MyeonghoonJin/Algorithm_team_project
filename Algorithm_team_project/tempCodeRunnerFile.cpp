#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <unordered_map>
#include <queue>
#include <set>


using namespace std;

/*--------------------------------------------------------------추가한 코드_명훈--------------------------------------------------------------*/
class Process {
public:
	int time;
	Process* previous_process;
	bool isMachine;
	string process_name;
	int process_num;

	Process(int time, Process* previous_process, bool isMachine, string process_name, int process_num)
	: time(time), previous_process(previous_process), isMachine(isMachine), process_name(process_name), process_num(process_num) {}
};

class Menu {
public:
	int menu_num;
	string menu_name;
	vector<int> processes;

	Menu(int menu_num, string menu_name, vector<int> processes)
	: menu_num(menu_num), menu_name(menu_name), processes(processes) {}
};
// 구조체를 사용하여 조합의 합과 인덱스를 저장
struct Subset {
    int sum;
    vector<int> indices;
};
// 배열의 모든 부분 집합을 찾는 함수
void findSubsets(vector<Process*>& all_tasks, int target, Subset& bestSubset) {
    int n = all_tasks.size();
    int maxSubsetCount = 1 << n; // 2^n 모든 부분 집합의 개수

    for (int i = 0; i < maxSubsetCount; ++i) {
        int currentSum = 0;
        vector<int> currentIndices;

        for (int j = 0; j < n; ++j) {
            if (i & (1 << j)) {
				if(!(all_tasks[j]->isMachine)){
					currentSum += all_tasks[j]->time;
					currentIndices.push_back(j);
				}
            }
        }

        if (currentSum <= target && currentSum > bestSubset.sum) {
            bestSubset.sum = currentSum;
            bestSubset.indices = currentIndices;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------*/
void readMenus(const string &filename, vector<Menu> &menus) {
	ifstream file(filename);
	string line;
	int menu_num;
	string menu_name;
	vector<int> processes;

	while (getline(file, line)) {
		if (line.find("menu_num") != string::npos) {
			if (!processes.empty()) {
				
				// Menu 객체를 생성하고 push_back을 사용하여 벡터에 추가합니다.
                menus.push_back(Menu(menu_num, menu_name, processes));
				processes.clear();
			}
			menu_num = stoi(line.substr(line.find('=') + 1));
		}
		else if (line.find("menu_name") != string::npos) {
			menu_name = line.substr(line.find('=') + 1);
		}
		else if (line.find("processes") != string::npos) {
			string process_list = line.substr(line.find('=') + 1);
			stringstream ss(process_list);
			string process;
			while (getline(ss, process, ',')) {
				if (!process.empty()) {
					processes.push_back(stoi(process.substr(7))); // "process" 제거
				}
			}
		}
	}

	if (!processes.empty()) {
		// Menu 객체를 생성하고 push_back을 사용하여 벡터에 추가합니다.
        menus.push_back(Menu(menu_num, menu_name, processes));
	}

	file.close();
}

void readProcesses(const string &filename, unordered_map<int, Process*> &processes) {
	ifstream file(filename);
	string line;
	int time;
	string previous_process_name;
	bool isMachine;
	string process_name;
	int process_num;

	while (getline(file, line)) {
		if (line.find("time") != string::npos) {
			time = stoi(line.substr(line.find('=') + 1));
		}
		else if (line.find("previous_process") != string::npos) {
			previous_process_name = line.substr(line.find('=') + 1);
			if (previous_process_name == "null") {
				previous_process_name = "";
			}
		}
		else if (line.find("isMachine") != string::npos) {
			isMachine = stoi(line.substr(line.find('=') + 1));
		}
		else if (line.find("process_name") != string::npos) {
			process_name = line.substr(line.find('=') + 1);
		}
		else if (line.find("process_num") != string::npos) {
			process_num = stoi(line.substr(line.find('=') + 1));
			Process* previous_process = nullptr;
			if (!previous_process_name.empty()) {
				int previous_process_num = stoi(previous_process_name.substr(7)); // "process" 제거
				previous_process = processes[previous_process_num];
			}
			processes[process_num] = new Process(time, previous_process, isMachine, process_name, process_num);
		}
	}

	file.close();
}

int main() {
	vector<Menu> menus;
	unordered_map<int, Process*> processes;
	vector<int> processes_vec;
	menus.push_back(Menu(0,"",processes_vec));
	// 파일 읽기
	readMenus("menu.txt", menus);
	readProcesses("process.txt", processes);

	// 사용자가 입력한 메뉴와 개수
	vector<pair<int, int> > menu_orders;
	pair<int,int>a;
	a.first = 1;
	a.second = 2;
	pair<int,int>b;
	b.first = 2;
	b.second = 2;

	menu_orders.push_back(a);
	menu_orders.push_back(b); // 예시 입력값: Menu1 2개, Menu2 2개 

	// 모든 작업을 저장할 벡터
	vector<Process*> all_tasks;

	// 입력된 메뉴와 개수를 바탕으로 프로세스 생성
	for (const auto& order : menu_orders) {
		int menu_num = order.first;
		int quantity = order.second;
		for (const auto& menu : menus) {
			if (menu.menu_num == menu_num) {
				for (int i = 0; i < quantity; ++i) {
					for (int process_num : menu.processes) {
						all_tasks.push_back(new Process(*processes[process_num]));
					}
				}
				break;
			}
		}
	}

	// 작업을 우선순위 큐에 넣기 위해 비교함수 정의
	// auto cmp = [](Process* left, Process* right) { return left->time > right->time; };
	// priority_queue<Process*, vector<Process*>, decltype(cmp)> task_queue(cmp);

	// 선행 프로세스를 완료했는지 확인하기 위한 집합
	set<int> completed_tasks;

	// 기계와 인간의 현재 작업 시간
	int machine_time = 0;
	int human_time = 0;
	int total_time = 0;

	// 모든 작업을 큐에 넣음
	// for (auto* task : all_tasks) {
	// 	task_queue.push(task);
	// }
/*------------------------------------------------------------ 그리디 알고리즘 ------------------------------------------------------------*/
	//가장 근접한 조합을 나타내는 변수 {최적합,{인덱스 집합}}
	Subset bestSubset;
	bestSubset.sum = 0;

	//작업을 처리
	while (!all_tasks.empty()){
		Process* current_task = all_tasks.front();

		//IsMachine확인 => 기계동작
		if(current_task->isMachine){
			//기계 동작 시간 추가
			machine_time += current_task->time;
			total_time += current_task->time;
			
			//기계 동작 수행
			completed_tasks.insert(current_task->process_num);
			cout << "Completed Process: " << current_task->process_name << " (Num: " << current_task->process_num << ")" << endl;
			//맨 앞 원소 제거 및 한칸 씩 이동
			all_tasks.erase(all_tasks.begin());

			//current_task->time 에 근접한 조합 찾기
			findSubsets(all_tasks,current_task->time,bestSubset);

			//조합에 해당되는 원소들 all_task,all_tesk_times에서 앞으로 가져오기
			for(auto& indexToMove : bestSubset.indices){
				if(indexToMove  >= 0 && indexToMove < all_tasks.size()){

					Process* elementToMove2 = all_tasks[indexToMove];

					//조합에 해당되는 원소를 all_task에서 맨 앞으로 가져오기
					all_tasks.erase(all_tasks.begin() + indexToMove);
					all_tasks.insert(all_tasks.begin(),elementToMove2);

					//맨 앞으로 가져온 동작 수행
					current_task = all_tasks.front();

					completed_tasks.insert(current_task->process_num);
					cout << "Completed Process: " << current_task->process_name << " (Num: " << current_task->process_num << ")" << endl;

					//맨 앞 원소 제거 및 한칸 씩 이동
					all_tasks.erase(all_tasks.begin());
				}
			}
			//기계 시간동안 한 사람의 동작 시간 추가
			human_time += bestSubset.sum;
		}
		//비기계 동작일 경우
		completed_tasks.insert(current_task->process_num);
		total_time += current_task->time;
		human_time += current_task->time;
		cout << "Completed Process: " << current_task->process_name << " (Num: " << current_task->process_num << ")" << endl;
		//맨 앞 원소 제거 및 한칸 씩 이동
		all_tasks.erase(all_tasks.begin());

	}
	//최종 시간 출력
	cout << "Total time: " << total_time << " seconds" << endl;     //총 소요시간
	cout << "Human time: " << human_time << " seconds" << endl;     //인력 소요 시간
	cout << "Machine time: " << machine_time << " seconds" << endl; //기계 소요 시간

/*------------------------------------------------------------ 그리디 알고리즘 -----------------------------------------------------------*/
	



/*-------------------------------------------------------- 기존 코드 내용(수정 전) ---------------------------------------------------------*/
	// 작업을 처리
	// while (!task_queue.empty()) {
	// 	Process* current_task = task_queue.top();
	// 	task_queue.pop();

	// 	// 선행 프로세스가 완료되었는지 확인
	// 	if (current_task->previous_process && completed_tasks.find(current_task->previous_process->process_num) == completed_tasks.end()) {
	// 		task_queue.push(current_task); // 선행 프로세스가 완료되지 않았으면 다시 큐에 넣음
	// 		continue;
	// 	}

	// 	// 기계 작업인지 인간 작업인지에 따라 처리
	// 	if (current_task->isMachine) {
	// 		machine_time += current_task->time;
	// 	}
	// 	else {
	// 		if (machine_time > human_time) {
	// 			human_time = machine_time;
	// 		}
	// 		human_time += current_task->time;
	// 	}

	// 	// 현재 작업 완료 표시
	// 	completed_tasks.insert(current_task->process_num);

	// 	cout << "Completed Process: " << current_task->process_name << " (Num: " << current_task->process_num << ")" << endl;
	// }

	// // 최종 소요 시간 출력
	// int total_time = max(machine_time, human_time);
	// cout << "Total time: " << total_time << " seconds" << endl;
/*--------------------------------------------------------- 기존 코드 내용(수정 전) ---------------------------------------------------------*/
	// 메모리 해제
	for (auto* task : all_tasks) {
		delete task;
	}

	return 0;
}
