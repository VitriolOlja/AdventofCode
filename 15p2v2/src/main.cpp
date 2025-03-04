#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <utility>

struct pairHash {
  std::size_t operator()(const std::pair<int, int>& pair) const {
    std::hash<int> intHash;
    std::size_t hash = 0;
    hash ^= intHash(pair.first) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    hash ^= intHash(pair.second) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    return hash;
  }
};


void print_map(const std::vector<std::vector<char>> &map) {
  for (int i = 0; i < map.size(); i++) {
    for( int j = 0; j < map[i].size(); j++) {
      std::cout << map[i][j];
    }
    std::cout << "\n";
  }
  std::cout << std::endl;
}

bool recursive_check(int x, int y, int dy, const std::vector<std::vector<char>> &map) {
  //base case is '.' w
  //check if '[ or ']

  int new_y = y + dy;

  if (map[new_y][x] == '.') {
    return true;
  }
  else if (map[new_y][x] == '#') {
    return false;
  }
  else if (map[new_y][x] == '[') {
    return recursive_check(x, new_y, dy, map) && recursive_check(x + 1, new_y, dy, map);
  }
  else if (map[new_y][x] == ']') {
    return recursive_check(x, new_y, dy, map) && recursive_check(x - 1, new_y, dy, map);
  }

  return false;
}

void recursive_move(int left, int right, int y, int dy, std::vector<std::vector<char>> &map) {
  if (map[y + dy][left] == '[') {
    recursive_move(left, right, y + dy, dy, map);
  }
  else {
    if (map[y + dy][left] == ']') {
      recursive_move(left - 1, left, y + dy, dy, map);
    }
    if (map[y + dy][right] == '[') {
      recursive_move(right, right + 1, y + dy, dy, map);
    }
  }
  char temp = map[y + dy][left]; 
  map[y + dy][left] = map[y][left];
  map[y][left] = '.';

  temp = map[y + dy][right]; 
  map[y + dy][right] = map[y][right];
  map[y][right] = '.';
}

void move_up_down(int x, int y, int dy, std::vector<std::vector<char>> &map) {
  if (map[y + dy][x] == '[') {
    recursive_move(x, x + 1, y + dy, dy, map);
  }
  else if (map[y + dy][x] == ']') {
    recursive_move(x - 1, x, y + dy, dy, map);
  }
  else if (map[y + dy][x] == '.') {
    map[y + dy][x] = map[y][x];
    map[y][x] = '.';
  }

  return;
}



char instruction(const std::pair<int, int> &n) {
    if (n == std::make_pair(-1, 0)) {
        return '<';  // Left
    } else if (n == std::make_pair(1, 0)) {
        return '>';  // Right
    } else if (n == std::make_pair(0, -1)) {
        return '^';  // Up
    } else if (n == std::make_pair(0, 1)) {
        return 'v';  // Down
    } else {
        return '?';  // Invalid direction
    }
}


void do_instruction(const std::pair<int, int> &instruction, std::pair<int, int> &robot, std::vector<std::vector<char>> &map) {
  if (instruction.second == 0) {
    //moving left and right. no recursion necessary.
    //check if the first that is not a [ or ] is a dot . in that case. move all. 

    int x = robot.first + instruction.first;

    while (map[robot.second][x] == '[' || map[robot.second][x] == ']') {
      x += instruction.first;
    }
    if (map[robot.second][x] == '#') {
      //do nothing
      return;
    }

    //else - move all this line.
    while (x - instruction.first != robot.first) {
      map[robot.second][x] = map[robot.second][x - instruction.first];
      //print_map(map);
      x -= instruction.first;
    }
    
    map[robot.second][robot.first] = '.';
    map[robot.second][robot.first + instruction.first] = '@';
    robot.first = robot.first + instruction.first;
  }
  else {
    //moving up and down. 
    int y = robot.second + instruction.second;
    
    bool should_move = recursive_check(robot.first, robot.second, instruction.second, map);
    
    if (should_move) {
      move_up_down(robot.first, robot.second, instruction.second, map);
      map[robot.second][robot.first] = '.';
      robot.second = robot.second + instruction.second;
      map[robot.second][robot.first] = '@';
    }
  }
}



int calculate_gps(const std::vector<std::vector<char>> &map) {
  int gps = 0;
  for (int i = 0; i < map.size(); i++) {
    for (int j = 0; j < map[i].size(); j++) {
      if (map[i][j] == '[') {
        gps += ((100 * i) + j);
      }
    }
  }
  return gps;
}

int main() {
  std::string line;
  std::ifstream input("C:/Users/Dwight/Desktop/c_dev/AoC/15p2v2/build/input.txt");

  std::vector<std::vector<char>> map;
  std::pair<int, int> robot;
  
  while (std::getline(input, line) && (line.find_first_of('#') == 0))
  {
    std::vector<char> row;
    for (int column = 0; column < line.find_last_of('#') + 1; column++) {
      if (line[column] == '#') {
        row.push_back('#');
        row.push_back('#');
      } else if (line[column] == 'O') {
        row.push_back('[');
        row.push_back(']');
      } else if(line[column] == '@') {
        robot.first = row.size();
        robot.second = map.size();
        row.push_back('@');
        row.push_back('.');
      }
      else {
        row.push_back('.');
        row.push_back('.');
      }
      
    }
    map.push_back(row);
  }

  print_map(map);

  std::vector<std::pair<int, int>> instructions;

  while(std::getline(input, line)) {
    for (const char &c : line) {
      if (c == '<') {
        instructions.push_back({-1, 0});
      }
      else if (c == '>') {
        instructions.push_back({1, 0});
      }
      else if (c == '^') {
        instructions.push_back({0, -1});
      }
      else if (c == 'v') {
        instructions.push_back({0, 1});
      }
    }
  }


  for (int i = 0; i < instructions.size(); i++) {
    do_instruction(instructions[i], robot, map);
    //std::cout << "\nInstruction " << i << "  " << instruction(instructions[i]) <<  " \n";
    //print_map(map);
  }


  std::cout << calculate_gps(map) << std::endl;

  std::cout << std::flush;

  

  return 0;
}