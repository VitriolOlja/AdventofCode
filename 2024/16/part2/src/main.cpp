#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <deque>
#include <unordered_map>
#include <utility>
#include <array>
#include <queue>

/**
 * this works to find the shortest path, but it can not find all shortest path
 * Maybe one problem is i am not tracking the cost of teh current path - 
 * im getting the cost by accessing the node i am on - this is potentialy problematic?
 * 
 * The problem is, I can come to a node with a worse score - yet face a better direction. 
 *  
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

struct Node {
  int shortest_distance;
  std::vector<
      std::pair<
          std::pair<int, int>, std::pair<int, int>
      >
  > parents;

  Node() : shortest_distance(INT_MAX) {}
  //Node(int x, int y, int cost) : x(x), y(y), cost(cost) {}
};

enum class ANGLE {
  UP,
  DOWN,
  LEFT,
  RIGHT
};


void print_map_and_pos(std::pair<int, int> pos, const std::vector<std::vector<char>> &map) {
  for (int i = 0; i < map.size(); i++) {
    for  (int j = 0; j < map[i].size(); j++) {
      if (i == pos.second && j == pos.first) {
        std::cout << 'X';
      }
      else {
        std::cout << map[i][j];
      }
    }
    std::cout << "\n";
  }
  std::cout << std::endl;
}

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

class Compare {
    public:
       bool operator()(const std::tuple<std::pair<int, int>, std::pair<int, int>, int> &a, 
       const std::tuple<std::pair<int, int>, std::pair<int, int>, int> &b){
          auto [a_current, a_angle, a_cost] = a;
          auto [b_current, b_angle, b_cost] = b;
           return a_cost > b_cost;
      }
};

struct doublePairHash {
  std::size_t operator()(const std::pair<std::pair<int, int>, std::pair<int, int>> &doublePair) const {
    std::hash<int> intHash;
    std::size_t hash = 0;
    hash ^= intHash(doublePair.first.first) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    hash ^= intHash(doublePair.first.second) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    hash ^= intHash(doublePair.second.first) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    hash ^= intHash(doublePair.second.second) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    return hash;
  }
};



void calculate_all_distances(std::pair<int, int> start_node, std::pair<int, int> start_angle, 
  const std::vector<std::vector<char>> &map, 
  std::unordered_map<std::pair<std::pair<int, int>, std::pair<int, int>>, Node, doublePairHash> &nodes) {
  
  std::priority_queue<
    std::tuple<std::pair<int, int>, std::pair<int, int>, int>,
    std::vector<std::tuple<std::pair<int, int>, std::pair<int, int>, int>>,
    Compare>
  queue;

  queue.push({start_node, start_angle, nodes[{start_node, start_angle}].shortest_distance});

  //hash of angle and 
  //std::unordered_set<std::pair<std::pair<int, int>, std::pair<int, int>>, doublePairHash> visited;
  //visited.insert({start_node, start_angle});

  
  while (!queue.empty()) {
    auto [current, angle, cost] = queue.top();
    queue.pop();

    if (map[current.second][current.first] == 'E') {
      continue;
    }

    std::pair<int, int> new_angle = angle;
    rotate_right(new_angle);
    for (int i = 0; i < 3; i++) {
      std::pair<int, int> new_pos;
      new_pos.first = current.first + new_angle.first;
      new_pos.second = current.second + new_angle.second;

      if (map[new_pos.second][new_pos.first] == '#') {
        rotate_left(new_angle);
        continue;
      }

      int new_cost = cost + 1;
      if (new_angle != angle) {
        new_cost += 1000;
      }

      //print_map_and_pos(new_pos, map);
      //std::cout << new_cost << std::endl;

      if (nodes[{new_pos, new_angle}].shortest_distance > new_cost) {
        nodes[{new_pos, new_angle}].parents.clear();
        nodes[{new_pos, new_angle}].parents.push_back({current, angle});
        nodes[{new_pos, new_angle}].shortest_distance = new_cost;
        queue.push({new_pos, new_angle, new_cost});
      }
      else if (nodes[{new_pos, new_angle}].shortest_distance == new_cost) {
        nodes[{new_pos, new_angle}].parents.push_back({current, angle});
      }


      rotate_left(new_angle);
    }
  }
}

void push_parents(std::unordered_map<std::pair<std::pair<int, int>, std::pair<int, int>>, Node, doublePairHash> &nodes, 
                  const std::pair<int, int> &node, 
                  const std::pair<int, int> &angle, 
                  std::deque<std::pair<std::pair<int, int>, std::pair<int, int>>> &queue) {

    auto it = nodes.find(std::make_pair(node, angle));
    if (it != nodes.end()) { 
        for (auto p : it->second.parents) {
            queue.push_back(p);
        }
    }
}

int main() {
  std::string line;

  std::ifstream input("C:/Users/Dwight/Desktop/c_dev/AoC/16/part2/build/input.txt");

  std::pair<int, int> start;
  std::pair<int, int> end;
  std::vector<std::vector<char>> map;
  std::unordered_map<std::pair<std::pair<int, int>, std::pair<int, int>>, Node, doublePairHash> nodes;
  while(std::getline(input, line)) {
    std::vector<char> row;
    for (int i = 0; i < line.size(); i++) {
      const char& c = line[i];
      if (c == 'S') {
        start = {i, map.size()};
      } else if(c == 'E') {
        end = {i, map.size()};
      }
      
      row.push_back(c);
    }
    map.push_back(row);
  }

  //std::cout << std::endl;
  //print_map_and_pos(start, map);
  std::pair<int, int> start_angle = {1,0};
  nodes[{start, start_angle}].shortest_distance = 0;
  calculate_all_distances(start, start_angle, map, nodes);

  std::pair<int, int> node_of_interest = {5, 7};
  auto p1 = nodes[{node_of_interest, {0, -1}}]; // up
  auto p2 = nodes[{node_of_interest, {1, 0}}];  //right
  auto p3 = nodes[{node_of_interest, {0, 1}}];  //down  
  auto p4 = nodes[{node_of_interest, {-1, 0}}];//left



  int tiles = 1;

  std::deque<std::pair<std::pair<int, int>, std::pair<int, int>>> queue; 


  push_parents(nodes, end, {0, -1}, queue);
  std::unordered_set<std::pair<std::pair<int, int>, std::pair<int, int>>, doublePairHash> visited;
  std::unordered_set<std::pair<int, int>, pairHash> nodeVisited;

  auto list = nodes[{{6, 7},{1, 0}}].parents;


  while(!queue.empty()) {
    auto [current, direction] = queue.front();
    queue.pop_front();

    if (visited.count({current, direction})) continue;

    visited.insert({current,direction});

    if(!nodeVisited.count(current)){
      tiles +=1;
      nodeVisited.insert(current);
    }
    
    map[current.second][current.first] = 'O';
    //print_map_and_pos(current, map);

    push_parents(nodes, current, direction, queue);
  }



  std::cout << "\n\n";

  for (int i = 0; i < map.size(); i++) {
    for (int j = 0; j < map[i].size(); j++) {
      std::cout << map[i][j];
    }
    std::cout << "\n";
  }
  //std::cout << "\n" << nodes[end].shortest_distance << std::endl;
  std::cout << "\n" << tiles << std::endl;
  
  return 0;
}