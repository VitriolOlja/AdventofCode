#include <vector> 
#include <fstream>
#include <string>
#include <unordered_set>
#include <deque> 
#include <iostream>
#include <tuple>
#include <unordered_map>


/*
  Idea: find the shortest path and hash the path 
  simulate falling bytes til one destroys the path - then find a new path.
*/
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
  std::pair<int, int> parent;
  Coordinate() : cost(INT_MAX) {}
};


int main() {
  std::string line;
  std::ifstream input("C:/Users/Dwight/Desktop/c_dev/AoC/18/part2/build/input.txt");

  std::vector<std::pair<int, int>> bytes;
  while(std::getline(input, line)) {
    size_t splitindex = line.find_first_of(',');
    std::string first = line.substr(0, splitindex);
    std::string second = line.substr(splitindex + 1, line.size() - splitindex - 1);
    bytes.push_back(std::make_pair(std::stoi(first), std::stoi(second)));
  }

  const std::pair<int, int> start = {0, 0};
  const std::pair<int, int> end = {70, 70};
  const std::pair<int, int> boundary = {70, 70};

  std::unordered_set<std::pair<int, int>, pairHash> corrupted;
  size_t fallen_bytes = 1024;
  for(int falling = 0; falling < fallen_bytes; falling++) {
    corrupted.insert(bytes[falling]);
  }
  

  while(true) {
    std::unordered_map<std::pair<int, int>, Coordinate, pairHash> cost;
    std::deque<std::pair<int, int>> queue;
    
    std::unordered_set<std::pair<int, int>, pairHash> path;
    queue.push_back(start);
    cost[start].cost = 0;
    std::pair<int, int> direction = {1, 0};

    while(!queue.empty()) {
      const auto current = queue.front();
      queue.pop_front();
      int steps = cost[current].cost;

      for(int i = 0; i < 4; i++){
        std::pair<int, int> next = {current.first + direction.first, current.second + direction.second};
        rotate_right(direction);

        if (next.first > boundary.first || 
            next.first < 0 || 
            next.second > boundary.second || 
            next.second < 0) {
          continue;
        }
        if(corrupted.count(next)) continue;

        if (cost[next].cost <= steps + 1) continue;

        cost[next].cost = steps + 1;
        cost[next].parent = current;
        queue.push_back(next);
      }
    }
    //if this does not work we can check while backtracking if we 
    //ever come to start.
    int end_cost = cost[end].cost;
    if (cost[end].cost == INT_MAX){
      break;
    }

    //backtrack
    std::pair<int, int> track = end;
    path.clear();
    path.insert(end);
    while(track != start) {
      path.insert(cost[track].parent);
      track = cost[track].parent;
    }

    //insert until we destroys the current path.
    while(true) {
      corrupted.insert(bytes[fallen_bytes]);
      fallen_bytes++;
      if (path.count(bytes[fallen_bytes-1])) break;
    }
  }
  

  std::cout << fallen_bytes << " with value: (" << bytes[fallen_bytes - 1].first << ", " << bytes[fallen_bytes - 1].second << ") " << std::endl;
  return 0;
}