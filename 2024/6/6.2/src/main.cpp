#include <vector>
#include <iostream>
#include <fstream>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <functional> // For std::hash

/**
 * general idea: 
 * The only way we can change the directory is if we put a obstacle
 * on some part of the path that we already took. 
 * so run the simulation once without interference
 * save all locations. 
 * run a loop over these potential obstacles. 
 * run the simulation where we add the potential obstacle
 * if we hit the same obstacle we did 4 obstacles ago. we are in a loop.
 * This does work for some. but not all of them. for example 3,8 is
 * a more complex infinity loop.
 *   
 * or if we hit the same obstacle with the same velocity. 
 */

template <>
struct std::hash<std::pair<int, int>> {
    size_t operator()(const std::pair<int, int>& U) const {
        size_t h1 = std::hash<int>{}(std::get<0>(U));
        size_t h2 = std::hash<int>{}(std::get<1>(U));

        // Combine the individual hashes
        return (((h1 ^ (h2 << 1)) >> 1));
    }
};

std::unordered_set<std::pair<int, int>> simulate(int guard_x, int guard_y, int vel_x, int vel_y, const std::vector<std::vector<char>> &map) {
  std::unordered_set<std::pair<int, int>> locations;
  while (guard_x + vel_x >= 0 && guard_x + vel_x < map[0].size() && guard_y + vel_y >= 0 && guard_y + vel_y < map.size()) {
    if (map[guard_y + vel_y][guard_x + vel_x] == '.' ) {
      locations.insert(std::make_pair(guard_x, guard_y));
      //map[guard_y][guard_x] = 'X';
      guard_x += vel_x;
      guard_y += vel_y;
      
    }
    else if (map[guard_y + vel_y][guard_x + vel_x] == '#') {
      //spin around.
      //we want to spin around clockwise. 
      //multiply by rotation vector
      //[[0, 1],
      // [-1, 0]]
      int temp = vel_x;
      vel_x = -vel_y;
      vel_y = temp;
    }
    else if (map[guard_y + vel_y][guard_x + vel_x] == 'X' ) {
      //map[guard_y][guard_x] = 'X';
      guard_x += vel_x;
      guard_y += vel_y;
    }
  }

  locations.insert(std::make_pair(guard_x, guard_y));
  //map[guard_y][guard_x] = 'X';

  return locations;
}

template <>
struct std::hash<std::tuple<int, int, int, int>> {
    size_t operator()(const std::tuple<int, int, int, int>& U) const {
        size_t h1 = std::hash<int>{}(std::get<0>(U));
        size_t h2 = std::hash<int>{}(std::get<1>(U));
        size_t h3 = std::hash<int>{}(std::get<2>(U));
        size_t h4 = std::hash<int>{}(std::get<3>(U));

        // Combine the individual hashes
        return (((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1)) ^ (h4 << 1);
    }
};


bool infinity(int guard_x, int guard_y, int vel_x, int vel_y, const std::vector<std::vector<char>> &map) {
    // Use a set to track visited states (position + velocity)
    std::unordered_set<std::tuple<int,int,int,int>> visited;

    while (guard_x + vel_x >= 0 && guard_x + vel_x < map[0].size() &&
           guard_y + vel_y >= 0 && guard_y + vel_y < map.size()) {
        
        if (visited.count(std::make_tuple(guard_x, guard_y, vel_x, vel_y)) > 0) {
          return true; //same state
        }

        visited.insert(std::make_tuple(guard_x, guard_y, vel_x, vel_y));

        if (map[guard_y + vel_y][guard_x + vel_x] == '#') {
            
            int temp = vel_x;
            vel_x = -vel_y;
            vel_y = temp;
            
        }
        else {
          guard_x += vel_x;
          guard_y += vel_y;
        }
    }

    return false; // No infinite loop detected
}


int main()
{

  std::ifstream input("C:\\Users\\Dwight\\Desktop\\c_dev\\AoC\\6\\6.2\\build\\input.txt");

  std::string line;
  int guard_x;
  int guard_y;
  int vel_x;
  int vel_y;

  std::vector<std::vector<char>> map;
  int row = 0;

  //load map.
  while (std::getline(input, line))
  {
    std::vector<char> row_vector;
    for (int i = 0; i < line.size(); i++)
    {
      if (line[i] == '^' || line[i] == 'v' || line[i] == '<' || line[i] == '>')
      {
        guard_x = i;
        guard_y = row;
        row_vector.push_back('.');

        switch (line[i])
        {
        case '^':
          vel_y = -1; // Up
          vel_x = 0;
          break;
        case '>':
          vel_y = 0;
          vel_x = 1; // Right
          break;
        case 'v':
          vel_y = 1; // Down
          vel_x = 0;
          break;
        case '<':
          vel_y = 0;
          vel_x = -1; // Left
          break;
        }
      }
      else if (line[i] == '#') {
        //obstacles.push_back(std::make_pair(i,row));
        row_vector.push_back(line[i]);
      }
      else{
        row_vector.push_back(line[i]);
      }

      
    }
    map.push_back(row_vector);
    row += 1;
  }


  //run the simulation
  std::cout << "read map" << std::endl;

  std::unordered_set<std::pair<int, int>> locations = simulate(guard_x, guard_y, vel_x, vel_y, map);
  //now we have all locations. 

  std::cout << "starting multisim" << std::endl;
  int inf = 0;
  for (const std::pair<int,int> &location : locations) {
    std::vector<std::vector<char>> new_map(map);
    new_map[location.second][location.first] = '#';
    //new_map[8][3] = '#';
    if (infinity(guard_x, guard_y, vel_x, vel_y, new_map)) {
      std::cout << "found inf: (" << location.first << "," << location.second << ")\n";
      inf += 1;
    }
    else {
      std::cout << "no inf: (" << location.first << "," << location.second << ")\n";
    }
  }

  std::cout << "\n" << inf << std::endl;

  return 0;
}