#include <vector>
#include <iostream>
#include <algorithm>


/**
 * return a vector of pairs, given a center and radius 
 * where the pairs are the edge of a discrete circle
 * where the each point is radius manhattan distance from center
 */
std::vector<std::pair<int, int>> getDiscreteCircle(const std::pair<int, int>& center, int radius) {
  std::vector<std::pair<int, int>> list;

  for (int d_x = -radius; d_x < radius; d_x++) {
    int d_y = radius - std::abs(d_x);

    list.push_back({center.first + d_x, center.second + d_y});
    list.push_back({center.first + d_x, center.second - d_y});
  }
  list.push_back({center.first + radius, center.second});

  return list;
}


int main() {

  
  for (int r = 1; r < 10; r++) 
  {
    std::vector<std::vector<char>> map;
    int width = 20;
    int height = 20;
    for (int i = 0; i < height; i++) {
      std::vector<char> row;
      for (int j = 0; j < width; j++) {
        row.push_back('.');
      }
      map.push_back(row);
    }

    std::vector<std::pair<int, int>> edge_points = getDiscreteCircle({10, 10}, r);

    for(const auto &e : edge_points) {
      map[e.second][e.first] = '#';
    }

    map[10][10] = 'X';

    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        std::cout << map[i][j];
      }
      std::cout << std::endl;
    }

    std::cout << std::endl;

  }
  return 0;
}



