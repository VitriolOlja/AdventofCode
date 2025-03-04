#include <vector> 
#include <fstream>
#include <string>
#include <unordered_set>
#include <deque> 
#include <iostream>
#include <tuple>
#include <unordered_map>


struct pairHash {
  std::size_t operator()(const std::pair<int, int>& pair) const {
    std::hash<int> intHash;
    std::size_t hash = 0;
    hash ^= intHash(pair.first) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    hash ^= intHash(pair.second) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    return hash;
  }
};

void rotate_left(std::pair<int, int> &dir) {
  int temp = dir.first;
  dir.first = dir.second;
  dir.second = -temp;
}

void rotate_right(std::pair<int, int> &dir) {
  int temp = dir.first;
  dir.first = -dir.second;
  dir.second = temp;
}

struct Coordinate {
  int cost; 
  Coordinate() : cost(INT_MAX) {}
};


int main() {
  std::string line;
  std::ifstream input("C:/Users/Dwight/Desktop/c_dev/AoC/18/build/input.txt");

  std::vector<std::pair<int, int>> bytes;
  while(std::getline(input, line)) {
    size_t splitindex = line.find_first_of(',');
    std::string first = line.substr(0, splitindex);
    std::string second = line.substr(splitindex + 1, line.size() - splitindex - 1);
    bytes.push_back(std::make_pair(std::stoi(first), std::stoi(second)));
  }

  std::pair<int, int> start = {0, 0};
  std::pair<int, int> end = {70, 70};

  std::unordered_set<std::pair<int, int>, pairHash> corrupted;
  size_t fallen_bytes = 1024;
  for(int falling = 0; falling < fallen_bytes; falling++) {
    corrupted.insert(bytes[falling]);
  }

  std::unordered_map<std::pair<int, int>, Coordinate, pairHash> cost;
  std::deque<std::tuple<std::pair<int, int>, int>> queue;
  
  const std::pair<int, int> boundary = {70, 70};

  queue.push_back({start, 0});
  std::pair<int, int> direction = {1, 0};

  while(!queue.empty()) {
    const auto  [current, steps] = queue.front();
    queue.pop_front();
    
    if(corrupted.count(current)) continue;
    if (current.first > boundary.first || current.first < 0 || current.second > boundary.second || current.second < 0) continue;


    if (steps < cost[current].cost) {
      cost[current].cost = steps;
      for(int i = 0; i < 4; i++){
        queue.push_back({{current.first + direction.first, current.second + direction.second}, steps + 1});
        rotate_right(direction);
      }
    }
  }

  std::cout << cost[end].cost << std::endl;
}