#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include <queue>
#include <cmath>
#include <set>
#include <algorithm>




#define INPUT "input.txt"

int main()
{
  std::ifstream inputFile(INPUT);

  if (!inputFile)
  {
    std::cerr << "Error: could not open file!" << std::endl;
    return 1;
  }

  std::vector<int> left, right;

  std::string line;
  while (std::getline(inputFile, line)) {
    size_t pos = line.find_first_of(' ');
    left.push_back(std::stoi(line.substr(0, pos)));
    right.push_back(std::stoi(line.substr(pos + 3, line.length())));
  }

  std::sort(right.begin(), right.end());

  int sum = 0;

  for (const int &num : left) {
    int low = 0;
    int high = right.size() - 1;

    //binary search right side.
    int mid;
    do {
      mid = low + (high - low) / 2;

      if (right[mid] > num) {
        high = mid - 1;
      }
      else if(right[mid] < num) {
        low = mid + 1;
      }
    } while(right[mid] != num && low <= high);

    if(low > high) {
      continue;
    }

    low = mid;
    while (right[low - 1] == num) {
      low -= 1;
    }

    high = mid;
    while(right[high + 1] == num) {
      high += 1;
    }

    sum += (high - low + 1) * num;

  }



  std::cout << sum << std::endl;

  return 0;
}