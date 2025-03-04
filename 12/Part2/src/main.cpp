#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_set>
#include <deque>

struct pairHash {
  std::size_t operator()(const std::pair<int, int>& pair) const {
    std::hash<int> intHash;
    std::size_t hash = 0;
    hash ^= intHash(pair.first) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    hash ^= intHash(pair.second) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    return hash;
  }
};

struct Region {
  int id;   //gotten by the char. 
  int area; 
  int perimeter;
  int corners;
};

int main() {
  std::string line;
  std::ifstream input("C:/Users/Dwight/Desktop/c_dev/AoC/12/build/input.txt");

  //int region['Z' - 'A' + 1][2] = {}; // pair (area, perimeter)

  std::vector<std::vector<int>> map;
  size_t y = 0;  
  while(std::getline(input, line)) {
    std::vector<int> row;
    
    int index;
    for (int i = 0; i < line.size(); i++) {
      const char &c = line[i];
      row.push_back(c - 'A');
    }
    map.push_back(row);
  }

  std::unordered_set<std::pair<int, int>, pairHash> visited;
  std::vector<Region> regions; //char, area, perimeter
  for (int row = 0; row < map.size(); row++ ) {
    for (int column = 0; column < map[row].size(); column++) {
      if (!visited.count(std::make_pair(column, row))) {
        //we have not been here before - do a bfs search
        std::deque<std::pair<int, int>> queue = {{column, row}};
        Region region = {map[row][column], 0, 0, 0};
        
        
        while(!queue.empty()) {
          std::pair<int, int> plant = queue.front();
          queue.pop_front();

          if (visited.count({plant.first, plant.second})){
            continue;
          }

          visited.insert({plant.first, plant.second});
          region.area += 1;

          //must check previous so it does not go back and forth? 
          //but that should be fixed by checking visited maybe? 
          bool up = false, down = false, left = false, right = false;
          if (0 <= plant.second - 1 && map[plant.second -1][plant.first] == region.id) {
            queue.push_back({plant.first, plant.second - 1});
          }
          else {
            region.perimeter += 1;
            up = true;
          }
          if (plant.second + 1 < map.size() && map[plant.second + 1][plant.first] == region.id) {
            queue.push_back({plant.first, plant.second + 1});
          }
          else {
            region.perimeter += 1;
            down = true;
          }
          if (0 <= plant.first - 1 && map[plant.second][plant.first - 1] == region.id) {
            queue.push_back({plant.first - 1, plant.second});
          }
          else {
            region.perimeter += 1;
            left = true;
          }
          if (plant.first + 1 < map[row].size() && map[plant.second][plant.first + 1] == region.id) {
            queue.push_back({plant.first + 1, plant.second});
          }
          else {
            region.perimeter += 1;
            right = true;
          }

          if (left && up) region.corners += 1;
          else if (!left && !up && map[plant.second - 1][plant.first - 1] != region.id) region.corners += 1;
          if (up && right) region.corners += 1;
          else if (!up && !right && map[plant.second - 1][plant.first + 1] != region.id) region.corners += 1;
          if (right && down) region.corners += 1;
          else if (!right && !down && map[plant.second + 1][plant.first + 1] != region.id) region.corners += 1;
          if (down && left) region.corners += 1;
          else if (!down && !left && map[plant.second + 1][plant.first - 1] != region.id) region.corners += 1;

        }
        

        regions.push_back(region);
      }
    }
  }

  std::cout << "\n\n";
  long long sum = 0; 

  for (const auto& [region, area, perimeter, corners] : regions) {
    sum += area * corners;
    std::cout << "Region: " << char(region + 'A') << ", Area: " << area << ", Perimeter: " << perimeter << " Corners: " << corners << "\n";
  }

  std::cout << std::endl;
  //for (int i = 0; i < sizeof(region) / sizeof(region[0]); i++) {
   //std::cout << char('A' + i) << " : (Area, Perimeter) : (" << region[i][0] << ", " << region[i][1] << ") \n";
    //sum += region[i][0] * region[i][1];
  //}


  std::cout << sum << std::endl;


  return 0;
}