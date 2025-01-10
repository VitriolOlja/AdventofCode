#include <string>
#include <vector>
#include <fstream>
#include <iostream>

int main() {
  std::string line;
  std::ifstream input("input.txt");

  std::vector<int> inp;
  while(std::getline(input, line)) {
    for (const char &c : line) {
      inp.push_back(c - '0');
    }
  }

  std::string data;
  bool is_data = 1;
  int id = 0;
  for (const int &i: inp) {
    if (is_data) {
      for (int j = 0; j < i; j++) {
        data += id + '0';
      }
      id += 1;
    }
    else {
      for (int j = 0; j < i; j++) {
        data += '.';
      }
    }

    is_data = !is_data;
  }
  
  int last = data.size() - 1;
  int first = 0;
  
  //std::cout << data << std::endl;
  while (first < last - 1 && first < data.size()) {
    //find a free slot. 
    while (first < last && data[first] != '.') {
      first++;
    }

    //find the last piece of data. 
    while (last >= first && data[last] == '.') {
      last--;
      data.pop_back();
    }
    

    data[first] = data[last];
    data.pop_back();
    last--;

    //std::cout << data << std::endl;
  }

  long long checksum = 0 ;
  //calc checksum
  for (int i = 0; i < data.size(); i++) {
    if (data[i] == '.') {
      break;
    }
    checksum += i * (data[i] - '0');
  }

  std::cout << checksum << std::endl;


  

  return 0;
}