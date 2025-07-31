#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>


long long concatNumbers(const long long &a, const long long &b) {
  int digits = std::to_string(b).length();
  long long rest = b;

  long long concat = a * static_cast<long long>(std::pow(10, digits)) + b;
  return concat;
}


bool calibrate(const long long &result, const std::vector<long long> &numbers) {
  std::vector<long long> sums;
  sums.push_back(numbers[0]); 
  int depth = 1;
  while (!sums.empty() && depth < numbers.size()) {
    std::vector<long long> level;
    long long next_number = numbers[depth];
    for (int i = 0; i < sums.size(); i++) {
      long long s = sums[i];
      long long mul_result = sums[i] * numbers[depth];
      if (mul_result <= result) {
        level.push_back(mul_result);
      }

      long long add_result = sums[i] + numbers[depth];
      if (add_result <= result) {
        level.push_back(add_result);
      }
      
      long long concat_result = concatNumbers(sums[i], numbers[depth]);
      if (concat_result <= result) {
        level.push_back(concat_result);
      }

      /* if (add_result == result || mul_result == result || concat_result == result) {
        return true;
      } */
    }

    depth +=1;
    sums = level;
    /* sums.clear();
    for (const auto &n : level) {
      sums.push_back(n);
    } */
  }

   for (const auto &n : sums) {
    if (n == result) {
      return true;
    }
   }
   return false;
}




int main() {
  std::string line;
  std::ifstream input("C:\\Users\\Dwight\\Desktop\\c_dev\\AoC\\7\\build\\input.txt");

  long long sum = 0;
  while(std::getline(input, line)) {
    size_t colon = line.find(':');
    std::string first = line.substr(0, colon);
    //std::cout << "\n" << first <<" " << first.length() << std::endl;
    long long result = std::stoll(first);


    std::vector<long long> numbers;

    size_t start = colon + 2;
    size_t end = line.find(' ', start);
    do {
      numbers.push_back(std::stoll(line.substr(start, end - start)));
      start = end + 1;
      end = line.find(' ', start);
    } while(end != std::string::npos);

    // Add the last number after the last space
    if (start < line.length()) {
      numbers.push_back(std::stoll(line.substr(start)));
    }


    if (calibrate(result, numbers)) {
      sum += result;
    }
  }

  std::cout << "calibration sum = " << sum << std::endl;

  return 0;
}