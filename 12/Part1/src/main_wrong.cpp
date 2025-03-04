#include <vector>
#include <string>
#include <iostream>
#include <fstream>

/**
 * When you dont read the task properly. 
 * I thought i could not do a graph search flood fill. 
 * but the regions are separate of course.  
 */

int main() {
  std::string line;
  std::ifstream input("C:/Users/Dwight/Desktop/c_dev/AoC/12/build/input.txt");

  int region['Z' - 'A' + 1][2] = {}; // pair (area, perimeter)

  std::vector<std::vector<int>> map;
  size_t y = 0;  
  while(std::getline(input, line)) {
    std::vector<int> row;
    
    int index;
    for (int i = 0; i < line.size(); i++) {
      const char &c = line[i];
      index = c - 'A';
      
      region[index][0] += 1;  //add to area. 

      int perimeter = 0;
      //top perimeter?
      if (y == 0) {
        perimeter += 1;
      }
      else if (map.back()[i] != index) {
        perimeter += 1; // not the same block above it. 
        region[map.back()[i]][1] += 1;  //also add it to perimeter above.
      }
      //left perimeter
      if (row.empty()) {  //first element. of row. 
        perimeter += 1;
      }
      else if(row.back() != index) {
        //if the last element is not the same as this
        //we have a left perimeter. 
        perimeter += 1;
        region[row.back()][1] += 1;
      }
      row.push_back(index);
      region[index][1] += perimeter;
    }
    region[index][1] += 1;  //add perimeter to left hand side. 
    map.push_back(row);
    y += 1;
  }
  //another loop through the last row.
  for (const int &c : map.back()) {
    region[c][1] += 1;
  }

  std::cout << "\n\n";
  long long sum = 0;
  for (int i = 0; i < sizeof(region) / sizeof(region[0]); i++) {
    std::cout << char('A' + i) << " : (Area, Perimeter) : (" << region[i][0] << ", " << region[i][1] << ") \n";
    sum += region[i][0] * region[i][1];
  }


  std::cout << sum << std::endl;


  return 0;
}