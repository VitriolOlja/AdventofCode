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


//a left and right pointer. 
//left pointer stays when it reaches a value that is unsorted, 
//right pointer keeps going . 
//return the compare without the element we stopped on if left ptr = right ptr;
//if its increasing vs decreasing? how to know if that is the problem? 


bool is_safe(const std::vector<int> &values, bool (*comparator)(const int&, const int&)) {
    // Check if the array is already safe
    bool is_already_safe = true;
    for (int i = 0; i < values.size() - 2; i++) {
        if (!comparator(values[i], values[i + 1])) {
            is_already_safe = false;
            break;
        }
    }
    if (is_already_safe) return true;


    for (int skip = 0; skip < values.size(); skip++) {
        bool is_safe_with_skip = true;

        for (int i = 0; i < values.size() - 1; i++) {

            if (i == skip) {
                if (i == 0) {
                  continue;
                }
                else if (i == values.size() - 1) {
                  return true;
                }
                if (!comparator(values[i-1], values[i+1])) {
                  is_safe_with_skip = false;
                  break;
                }
                i += 1;
            }
            if (!comparator(values[i], values[i + 1])) {
                is_safe_with_skip = false;
                break;
            }
        }

        if (is_safe_with_skip) {
            return true;
        }
    }
    return false;
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

    bool safe = is_safe(values, gt) || is_safe(values, lt);
    
    if (safe) {
      safe_reports += 1;
    }

  }


  std::cout << safe_reports << std::endl;


  return 0;
}