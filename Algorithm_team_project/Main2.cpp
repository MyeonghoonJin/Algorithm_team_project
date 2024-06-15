#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <unordered_map>
#include <queue>
#include <set>
#include <windows.h>
#include "Menu.h"
#include "Process.h"
#define _CRT_SECURE_NO_WARNINGS

using namespace std;

/*--------------------------------------------------------------추가한 코드_명훈--------------------------------------------------------------*/
// 구조체를 사용하여 조합의 합과 인덱스를 저장
struct Subset {
    int sum;
    vector<int> indices;
};
// 배열의 모든 부분 집합을 찾는 함수
// 부분집합을 찾는 함수 구현
void findClosestSubset(vector<Process*>& processes, int target, Subset& closestSubset) {
	int n = processes.size();
	int closestSum = INT_MAX;
	vector<int> closestIndices;

	// 비트 마스크를 사용한 모든 부분집합 탐색
	for (int mask = 0; mask < (1 << n); ++mask) {
		int currentSum = 0;
		vector<int> currentIndices;

		for (int i = 0; i < n; ++i) {
			if (mask & (1 << i)) {  // 현재 원소가 부분집합에 포함된 경우
				if (!processes[i]->isMachine) {  // isMachine이 false인 경우에만 고려
					currentSum += processes[i]->time;
					currentIndices.push_back(i);
				}
			}
		}

		// 현재 부분집합의 합이 target을 초과하지 않고, 현재까지 탐색한 최소 합보다 작을 때 업데이트
		if (currentSum <= target && abs(currentSum - target) < abs(closestSum - target)) {
			closestSum = currentSum;
			closestIndices = currentIndices;
		}
	}

	// closestSubset 구조체에 저장
	closestSubset.sum = closestSum;
	closestSubset.indices = closestIndices;
}


/*----------------------------------------------------------------------------------------------------------------------------------------*/
void readMenus(const string &filename, vector<Menu> &menus) {
	ifstream file(filename);
	if (!file) {
		cerr << "Error opening file: " << filename << endl;
		return;
	}
	string line;
	int menu_num;
	string menu_name;
	vector<int> processes;
	
	while (getline(file, line)) {
		if (line.find("menu_num") != string::npos) {
			if (!processes.empty()) {

                menus.emplace_back(menu_num, menu_name, processes);
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
        menus.emplace_back(menu_num, menu_name, processes);
	}

	file.close();
}

void readProcesses(const string& filename, unordered_map<int, Process*>& processes) {
	ifstream file(filename);
	if (!file) {
		cerr << "Error opening file: " << filename << endl;
		return;
	}
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
	SetConsoleOutputCP(CP_UTF8);
	// 파일 읽기
	string m = "menu.txt";
	string p = "process.txt";
	readMenus(m, menus);
	readProcesses(p, processes);

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
	 auto cmp = [](Process* left, Process* right) { return left->time > right->time; };
	 priority_queue<Process*, vector<Process*>, decltype(cmp)> task_queue(cmp);

	// 선행 프로세스를 완료했는지 확인하기 위한 집합
	set<int> completed_tasks;

	// 기계와 인간의 현재 작업 시간
	int machine_time = 0;
	int human_time = 0;
	int total_time = 0;

	 //모든 작업을 큐에 넣음
	 for (auto* task : all_tasks) {
	 	task_queue.push(task);
	 }

/*------------------------------------------------------------ 그리디 알고리즘 ------------------------------------------------------------*/
	//가장 근접한 조합을 나타내는 변수 {최적합,{인덱스 집합}}
	 Subset bestSubset;
	 bestSubset.sum = 0;
	//작업을 처리
	while (!all_tasks.empty()){
		Process* current_task = all_tasks.front();

		//IsMachine확인 => 기계동작
		if (current_task->isMachine) {

			//기계 동작 시간 추가
			machine_time += current_task->time;
			total_time += current_task->time;

			//기계 동작 수행
			completed_tasks.insert(current_task->process_num);
			cout << "Completed Process: " << current_task->process_name << " (Num: " << current_task->process_num << ")" << endl;
			//맨 앞 원소 제거 및 한칸 씩 이동
			all_tasks.erase(all_tasks.begin());


			//current_task->time 에 근접한 조합 찾기
			findClosestSubset(all_tasks, current_task->time, bestSubset);
			//조합에 해당되는 원소들 all_task,all_tesk_times에서 앞으로 가져오기

			vector<Process*>p_vec;
			for (int i = 0; i < bestSubset.indices.size(); i++) {
				int indexToMove = bestSubset.indices[i];
				if (indexToMove >= 0 && indexToMove < all_tasks.size()) {
					p_vec.push_back(all_tasks[indexToMove]);
				}
			}
			for (int i = bestSubset.indices.size() - 1; i >= 0; i--) {
				all_tasks.erase(all_tasks.begin() + bestSubset.indices[i]);
			}
			for (int i = 0; i < bestSubset.indices.size(); i++) {
				all_tasks.insert(all_tasks.begin(), p_vec[i]);
			}
			//프로세스 실행
			for (int i = 0; i < bestSubset.indices.size(); i++) {
				current_task = all_tasks.front();
				completed_tasks.insert(current_task->process_num);
				all_tasks.erase(all_tasks.begin());
				cout << "Completed Process: " << current_task->process_name << " (Num: " << current_task->process_num << ")" << endl;
			}
			p_vec.clear();
			//기계 시간동안 한 사람의 동작 시간 추가
			human_time += bestSubset.sum;
			//bestSubset초기화
			bestSubset.sum = 0;
			bestSubset.indices.clear();
		}
		else {
			//비기계 동작일 경우
			completed_tasks.insert(current_task->process_num);
			total_time += current_task->time;
			human_time += current_task->time;
			cout << "Completed Process: " << current_task->process_name << " (Num: " << current_task->process_num << ")" << endl;
			//맨 앞 원소 제거 및 한칸 씩 이동
			all_tasks.erase(all_tasks.begin());
		}
	}
	//최종 시간 출력
	cout << "Total time: " << total_time << " seconds" << endl;     //총 소요시간
	cout << "Human time: " << human_time << " seconds" << endl;     //인력 소요 시간
	cout << "Machine time: " << machine_time << " seconds" << endl; //기계 소요 시간

/*------------------------------------------------------------ 그리디 알고리즘 -----------------------------------------------------------*/
	
	// 메모리 해제
	for (auto* task : all_tasks) {
		delete task;
	}

	return 0;
}
