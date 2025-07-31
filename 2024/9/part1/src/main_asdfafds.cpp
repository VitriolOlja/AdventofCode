#include <vector>
#include <iostream>
#include <string>
#include <fstream>


int main() {
  std::string line;
  std::ifstream input("C:\\Users\\Dwight\\Desktop\\c_dev\\AoC\\9\\build\\input.txt");

  std::vector<int> data;

  
  while(std::getline(input, line)) {
    for (const char &c : line) {
      data.push_back(c - '0');
    }
  }

  int last = data.size() - 1;
  int first = 0;
  bool is_data = true;
  long first_id = 0;
  long last_id = (data.size() - 1) / 2;
  int i = 0;

  long checksum = 0;
  while(first < data.size()) {
    if (first_id > last_id) {
      break;
    }
    if (is_data) {
      for (int j = 0; j < data[first]; j++) {
        checksum += first_id * i;
        i++;
      }
      first++;
      first_id++;
    }
    else { /* it is not data. */
      for (int j = 0; j < data[first]; j++) {
        //while we have free space here - remove from back

        //first if the back is also empty - 
        // we need to move the back ptr to the front.
        while(data[last] <= 0) {
          last -= 2;
          last_id--;
        }

        checksum += last_id * i;
        data[last]--;
        i++;
      }
      first++;
    }

    is_data = !is_data;
  } 

  std::cout << checksum << std::endl;
  


  return 0;
}