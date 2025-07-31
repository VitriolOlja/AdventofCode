#include <string>
#include <vector>
#include <fstream>
#include <iostream>



void print_data(const std::vector<int> &data) {

  if (data.empty()) {
    std::cout << "[]" << std::endl;
    return;
  }
  std::cout << "[";
  for (int i = 0; i < data.size() - 1; i++) {
    std::cout << data[i] << ", ";
  }
  
  std::cout << data[data.size() - 1] << "]";

  std::cout << std::endl;

}


int main() {
  std::string line;
  std::ifstream input("C:\\Users\\Dwight\\Desktop\\c_dev\\AoC\\9\\build\\input.txt");

  std::vector<int> inp;
  while(std::getline(input, line)) {
    for (const char &c : line) {
      if (c >= '0' && c <= '9') {
        inp.push_back(static_cast<int>(c - '0'));
      } else {
          std::cerr << "Error: Invalid character '" << c << "' in input." << std::endl;
      }
    }
  }



  std::vector<int> data;


  bool is_data = 1;
  int id = 0;
  for (const int &i: inp) {
    if (is_data) {
      for (int j = 0; j < i; j++) {
        data.push_back(id);
      }
      id += 1;
    }
    else {
      for (int j = 0; j < i; j++) {
        data.push_back(-1);
      }
    }

    is_data = !is_data;
  }


  //compact

  int last = data.size() - 1;
  int first = 0;
  
  //print_data(data);
  //std::cout << data << std::endl;
  while (first < last) {
    //find a free slot. 
    while (first < last && data[first] != -1) {
      first++;
    }

    //find the last piece of data. 
    while (first < last && data[last] == -1) {
      last--;
      //data.pop_back();
    }

    if (first >= last) {
      break;
    }
    

    data[first] = data[last];
    data[last] = -1;
    //data.pop_back();
    //last--;

    //print_data(data);
  }
  //print_data(data);

  long long checksum = 0;

  for (int i = 0; i < data.size(); i++) {
    if (data[i] == -1) {
      continue;
    }
    checksum += data[i] * i;
  }

  std::cout << checksum << std::endl;




  return 0;
}