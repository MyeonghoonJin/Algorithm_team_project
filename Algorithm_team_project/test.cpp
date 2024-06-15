#include <iostream>
#include <vector>

int main() {
    std::vector<int> vec;
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);
    vec.push_back(40);
    vec.push_back(50);

    if (!vec.empty()) {
        // 벡터의 맨 앞 요소를 저장합니다.
        int firstElement = vec.front();

        // 벡터의 맨 앞 요소를 제거합니다.
        vec.erase(vec.begin());

        // 저장된 맨 앞 요소를 출력합니다.
        std::cout << "제거된 첫 번째 요소: " << firstElement << std::endl;

        // 벡터의 현재 상태를 출력합니다.
        std::cout << "현재 벡터: ";
        for (const auto& elem : vec) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "벡터가 비어 있습니다." << std::endl;
    }

    return 0;
}
