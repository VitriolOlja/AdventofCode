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
  int left = 0;
  int right = values.size() - 2;

  while(left < (values.size() - 1) && comparator(values[left], values[left + 1])) {
    left += 1;
  }

  if (left >= (values.size() - 1)) {
      for(const int &i : values) {
        std::cout << i << " - ";
      }
      std::cout << "Is safe without removing any level - " << std::endl;
      return true;
  }
  else if (left >= (values.size() - 2)) {
    for(const int &i : values) {
        std::cout << i << " - ";
      }
      std::cout << "\nIs safe when removing - " << values[left + 1] << std::endl;
      return true;
  }

  //std::cout << "left pointer stopped at index " << left << std::endl;

  while(right > 0 && comparator(values[right], values[right + 1])) {
    right -= 1;
  }

  //std::cout << "right pointer stopped at index " << right << std::endl;

  if (right == left || right == (left + 1)){
    bool some = comparator(values[right - 1], values[right + 1]);
    bool some2 = comparator(values[right], values[right + 2]);

    if (some) {
      for(const int &i : values) {
        std::cout << i << " - ";
      }
      std::cout << "\n is safe when removing " << values[right] << std::endl;
      return true;
    }
    if (some2) {
      for(const int &i : values) {
        std::cout << i << " - ";
      }
      std::cout << "\n is safe when removing " << values[right + 1] << std::endl;
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
  std::ifstream input("input.txt");

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