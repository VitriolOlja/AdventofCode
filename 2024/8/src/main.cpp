#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <unordered_set>
#include <utility>

struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator () (std::pair<T1, T2> const &pair) const
    {
        std::size_t h1 = std::hash<T1>()(pair.first);
        std::size_t h2 = std::hash<T2>()(pair.second);

        // A standard hash combination formula
        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
};


int main() {
  std::string line;
  std::ifstream input("C:\\Users\\Dwight\\Desktop\\c_dev\\AoC\\8\\build\\input.txt");
  std::map<char, std::vector<std::pair<int, int>>> nodes_map;
  std::map<std::pair<int, int>, char> antennas;
  int row = 0;
  int columns;
  while(std::getline(input, line)) {
    for (int column = 0; column < line.size(); column++) {
      if (line[column] != '.') {
        nodes_map[line[column]].push_back(std::make_pair(column, row)); 
        antennas[std::make_pair(column, row)] = line[column];
      }
    }

    row++;
    columns = line.size();
  }
  std::vector<std::vector<std::pair<int, int>>> nodes;
  // loop over the map, and 
  int nodes_amount = 0;
  for (const auto& [key, value] : nodes_map) {
    nodes.push_back(value);
    nodes_amount += value.size();
  }

  std::unordered_set<std::pair<int, int>, pair_hash> anti_nodes;
  // loop over the antennas. 
  // 
 
  for(int node_type = 0; node_type < nodes.size(); node_type++) {
    const auto & n = nodes[node_type];
    //double loop over the nodes of one type. 
    for (int i = 0; i < n.size(); i++) {
      for (int j = 0; j < n.size(); j++) {
      //we get delta x and delta y between two nodes.
        if (j == i) {
          continue;
        }

        int delta_x = n[i].first - n[j].first;
        int delta_y = n[i].second - n[j].second;

        int first_anti_x = n[i].first - delta_x;
        int first_anti_y = n[i].second - delta_y;

        while (first_anti_x >= 0 && first_anti_x < columns &&  first_anti_y >= 0 && first_anti_y < row) {
          anti_nodes.insert(std::make_pair(first_anti_x, first_anti_y));

          first_anti_x -= delta_x;
          first_anti_y -= delta_y;
        }
      }
    }
  }


  //char m[row][columns] = {};

  //char A = 'A';

  for (int i = 0; i < row; i++) {
    for (int j = 0; j < columns; j++) {
      char symbol;
      
      if (antennas.count(std::make_pair(j, i)))
      {
        symbol = antennas[std::make_pair(j, i)];
      }
      else if (anti_nodes.count(std::make_pair(j, i))){
        symbol = '#';
      }
      else {
        symbol = '.';
      }
      std::cout << symbol;
    }
    std::cout << "\n";
  }

  std::cout << anti_nodes.size() << std::endl;


  return 0;
}