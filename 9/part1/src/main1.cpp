#include <string>
#include <vector>
#include <fstream>
#include <iostream>


int main() {
  std::string line;
  std::ifstream input("C:\\Users\\Dwight\\Desktop\\c_dev\\AoC\\9\\build\\input.txt");

  std::vector<int> inp;
  while(std::getline(input, line)) {
    for (const char &c : line) {
      inp.push_back(c - '0');
    }
  }

  long long checksum = 0;

  std::vector<int> final;

  bool is_data = 1;
  int first_id = 0;
  int last_id = (inp.size() - 1) / 2;
  int last = inp.size() - 1;
  int i = 0;

  for (const int &disk_map_element: inp) {
    if (first_id > last_id) {
      break;
    }
    if (is_data) {
      for (int j = 0; j < disk_map_element; j++) {
        //we want to calculate the sum as usual.
        checksum += first_id * i;
        final.push_back(first_id);
        i++;
      }
      first_id += 1;
    }
    else {
      //its empty - meaning we want to remove from the back and add checksum here. 

      for (int j = 0; j < disk_map_element; j++) {
        //while we have free space here - remove from back
        while(inp[last] <= 0) {
          last -= 2;
          last_id--;
        }

        checksum += last_id * i;
        final.push_back(last_id);
        inp[last]--;
        i++;
      }
    }

    is_data = !is_data; //every other indicates data/free space. 
  }

  std::cout << "\n" << checksum << std::endl;

 /*  std::cout << "[";
  for (int i = 0; i < final.size() - 1; i++) {
    std::cout << final[i] << ", ";
  }
  

  std::cout << final[final.size() - 1] << "]";

  std::cout << std::endl; 
 */
  return 0;
}