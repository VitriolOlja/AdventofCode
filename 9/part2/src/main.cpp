#include <vector>
#include <iostream>
#include <fstream>
#include <string>



void print_data(const std::vector<std::pair<int, int>> &disk_map) {
  for (const auto& pair : disk_map) {
    for (int i = 0; i < pair.second; i++) {
      if (pair.first == -1) {
        std::cout << '.';
      }
      else {
        std::cout << pair.first;
      }
    }
  }
}


int main() {
  std::string line;
  std::ifstream input("C:\\Users\\Dwight\\Desktop\\c_dev\\AoC\\9\\part2\\build\\input.txt");
  
  std::vector<std::pair<int, int>> disk_map; /* (id, how many of them! ) id = -1 for empty space. */
  int id = 0;
  bool data = true;
  while(std::getline(input, line)) {
    for (const char &c: line) {
      if (data) {
        disk_map.push_back(std::make_pair(id, static_cast<int>(c - '0')));
        id++;
      }
      else {
        disk_map.push_back(std::make_pair(-1, static_cast<int>(c - '0')));
      }

      data = !data;
    }
  }
  //print_data(disk_map);

  std::cout << std::endl;

  int last = disk_map.size() - 1;
  while (last >= 0) {
    if (disk_map[last].first == -1) {
      last--;
      continue;
    }
    for (int j = 0; j < last; j++) {
      if (disk_map[j].first == -1 && disk_map[j].second >= disk_map[last].second) {
        int rest = disk_map[j].second - disk_map[last].second;
        if (rest == 0) {
          disk_map[j].first = disk_map[last].first;
          disk_map[last].first = -1;
        }
        else { /* > 0 */
          disk_map[j].second = disk_map[last].second;
          disk_map[j].first = disk_map[last].first;
          disk_map[last].first = -1;
          // add a new free space entry.
          if (disk_map[j+1].second == -1) {
            disk_map[j+1].second += rest;
          }
          else {
            disk_map.insert(disk_map.begin() + j + 1, std::make_pair(-1, rest));  
          }
        }
        break;
      }
    }
    last--;
  
  }

  //print_data(disk_map);

  long long checksum = 0;
  long long i  = 0;
  for (const std::pair<int, int> &e : disk_map) {
    if (e.first == -1){
      i += e.second;
      continue;
    }

    for (int byte = 0; byte < e.second; byte++) {
      checksum += e.first * i;
      i++;
    }
  }
  

  std::cout << "\n" << checksum << std::endl;

  return 0;
}