#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>


std::vector<int> getRow(const std::string &line) {
  std::vector<int> values;

  std::string value;
  for (const char &c : line) {
    if (std::isdigit(c)) {
      value.push_back(c);
    }
    else if (!value.empty()){
      values.push_back(std::stoi(value));
      value.clear();
    }
  }

  if (!value.empty()){
      values.push_back(std::stoi(value));
      value.clear();
  }

  return values;
}

bool is_safe(const std::vector<int> &values, bool (*comparator)(const int&, const int&), bool skipped) {
  for (int i = 0; i < values.size() - 1; i++) {
      if (!comparator(values[i], values[i + 1] )) {
        if (!skipped){
          std::vector<int> copy;
          std::vector<int> copy2;
          
          for(int j = 0; j < values.size(); j++) {
            if (j != i) {
              copy.push_back(values[j]);
            }
            if (j != i+1) {
              copy2.push_back(values[j]);
            }
          }
          return is_safe(copy, comparator, true) || is_safe(copy2, comparator, true);
        }
        else{
          return false;
        }
      }
  }
  return true;
}


bool gt(const int& a, const int& b) {
  return a > b  && std::abs(a - b) <= 3;
}
bool lt(const int& a, const int& b) {
  return a < b  && std::abs(a - b) <= 3;
}

int main() {
  std::ifstream input("C:/Users/Dwight/Desktop/c_dev/AoC/2/build/input.txt");

  std::string line;
  int safe_reports = 0;
  while(getline(input, line)) {
    
    std::vector<int> values = getRow(line); 

    bool safe = is_safe(values, gt, false) || is_safe(values, lt, false);
    
    if (safe) {
      safe_reports += 1;
    }

  }


  std::cout << safe_reports << std::endl;


  return 0;
}