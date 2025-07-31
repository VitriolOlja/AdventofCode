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

int some_function(int* arr)
{
    int a = arr[0];

    if(arr != nullptr)
    {
        return a + arr[1];
    }

    return 0;
}

struct Region {
  int id;   //gotten by the char. 
  int area; 
  int perimeter;
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
        Region region = {map[row][column], 0, 0};
        
        
        while(!queue.empty()) {
          std::pair<int, int> plant = queue.front();
          queue.pop_front();


          //check the value of this plant.
          if(map[plant.second][plant.first] != region.id) {
            //this is not same as last one -> so add perimeter? 
            region.perimeter += 1;
            continue;
          }
          if (visited.count({plant.first, plant.second})){
            continue;
          }

          visited.insert({plant.first, plant.second});
          region.area += 1;

          //must check previous so it does not go back and forth? 
          //but that should be fixed by checking visited maybe? 
          if (0 <= plant.second - 1) {
            queue.push_back({plant.first, plant.second - 1});
          }
          else {
            region.perimeter += 1;
          }
          if (plant.second + 1 < map.size()) {
            queue.push_back({plant.first, plant.second + 1});
          }
          else {
            region.perimeter += 1;
          }
          if (0 <= plant.first - 1) {
            queue.push_back({plant.first - 1, plant.second});
          }
          else {
            region.perimeter += 1;
          }
          if (plant.first + 1 < map[row].size()) {
            queue.push_back({plant.first + 1, plant.second});
          }
          else {
            region.perimeter += 1;
          }
        }

        regions.push_back(region);
      }
    }
  }

  std::cout << "\n\n";
  long long sum = 0; 

  for (const auto& [region, area, perimeter] : regions) {
    sum += area * perimeter;
    std::cout << "Region: " << char(region + 'A') << ", Area: " << area << ", Perimeter: " << perimeter << "\n";
  }

  std::cout << std::endl;
  //for (int i = 0; i < sizeof(region) / sizeof(region[0]); i++) {
   //std::cout << char('A' + i) << " : (Area, Perimeter) : (" << region[i][0] << ", " << region[i][1] << ") \n";
    //sum += region[i][0] * region[i][1];
  //}


  std::cout << sum << std::endl;


  return 0;
}