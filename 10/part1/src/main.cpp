#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <unordered_set>
#include <deque>
#include <map>

struct pair_hash
{
  template <class T1, class T2>
  std::size_t operator()(std::pair<T1, T2> const &pair) const
  {
    std::size_t h1 = std::hash<T1>()(pair.first);
    std::size_t h2 = std::hash<T2>()(pair.second);

    // A standard hash combination formula
    return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
  }
};

/** Trailhead - any position with start 0 height.
 * trailhead score - number of 9 height positions reachable from the head
 *
 *
 *
 */

std::string pair_to_string(const std::pair<int, int> &pair) {
    return "(" + std::to_string(pair.first) + ", " + std::to_string(pair.second) + ")";
}

void print_path(const std::vector<std::pair<int, int>> &current_path, const std::vector<std::vector<int>> &map) {
  // Initialize char_map to have the same dimensions as map
  std::vector<std::vector<char>> char_map(map.size(), std::vector<char>(map[0].size(), '.'));

  // Mark the path on char_map
  for (auto &p : current_path) {
    char_map[p.second][p.first] = map[p.second][p.first] + '0'; // Convert int to char
  }

  // Print the char_map
  for (int i = 0; i < char_map.size(); i++) {
    for (int j = 0; j < char_map[i].size(); j++) {
      std::cout << char_map[i][j];
    }
    std::cout << "\n";
  }

  std::cout << std::endl;
}

int all_paths_to_rome(const std::pair<int, int> &position,
                      const std::vector<std::vector<int>> &map,
                      const std::map<std::pair<int, int>, std::vector<std::pair<int, int>>> &parents,
                      std::vector<std::pair<int, int>> current_path) {
  
  
  current_path.push_back(position);

  //recursive the end is when we arrived at a zero.
  //print path: //or when we have no parents? 
  int height = map[position.second][position.first];
  if(height == 0) {
    //print_path(current_path, map);
    return 1;
  }
  //std::cout << std::endl;

  int sum = 0;

  // Check if 'position' exists in the 'parents' map
  auto it = parents.find(position);
  if (it != parents.end()) {
    // If it exists, iterate over its parents
    for (const std::pair<int, int> &parent : it->second) {
      sum += all_paths_to_rome(parent, map, parents, current_path);
    }
  }

  return sum;
}

void visit(const std::pair<int, int> &current_position,
           const std::pair<int, int> &new_position,
           std::map<std::pair<int, int>, std::vector<std::pair<int, int>>> &parents,
           const std::vector<std::vector<int>> &map,
           std::deque<std::pair<int, int>> &queue,
           std::unordered_set<std::pair<int, int>, pair_hash> &mountain_tops)
{
  if (0 <= new_position.first && new_position.first < map[0].size() && 0 <= new_position.second && new_position.second < map.size())
  {
    //std::cout << "Checking position: (" << new_position.first << ", " << new_position.second << ")" << std::endl;

    for (const std::pair<int, int> &parent : parents[new_position])
    {
      if (current_position == parent)
      {
        return; 
      }
    }
    //check elevation difference of new position before going there. 

    const int &elevation_difference = map[new_position.second][new_position.first] - map[current_position.second][current_position.first];
    if (elevation_difference == 1)
    {
      parents[new_position].push_back(current_position);
      if (map[new_position.second][new_position.first] == 9) {
        //we reached the top of a mountain.
        //now lets count how many ways there are to get here? 
        //std::cout << "found a 9 at : " << pair_to_string(new_position) << "coming from " << pair_to_string(current_position) << std::endl;
        //std::vector<std::pair<int, int>> path;
        mountain_tops.insert(new_position);
        //all_paths_to_rome(new_position, map, parents, path);
      }
      else {
        queue.push_back(new_position);
      } 
    }
  }
  return;
}

void process_position(const std::pair<int, int> &current_position,
                      std::map<std::pair<int, int>, std::vector<std::pair<int, int>>> &parents,
                      const std::vector<std::vector<int>> &map,
                      std::deque<std::pair<int, int>> &queue,
                      std::unordered_set<std::pair<int, int>, pair_hash> &mountain_tops)
{
  //directions 
  static const std::pair<int, int> directions[] = {
      {-1, 0}, // Left
      {1, 0},  // Right
      {0, -1}, // Up
      {0, 1}   // Down
  };
  for (const auto &[dx, dy] : directions)
  {
    const std::pair<int, int> new_position = {current_position.first + dx, current_position.second + dy};
    visit(current_position, new_position, parents, map, queue, mountain_tops);
  }
}

int main()
{
  std::string line;

  std::ifstream input("C:\\Users\\Dwight\\Desktop\\c_dev\\AoC\\10\\part1\\build\\input.txt");

  std::vector<std::vector<int>> map;
  std::vector<std::pair<int, int>> trailheads;
  while (std::getline(input, line))
  {
    std::vector<int> row;
    for (int i = 0; i < line.size(); i++)
    {
      int elevation = static_cast<int>(line[i] - '0');
      row.push_back(elevation);
      if (elevation == 0)
      {
        // trailhead.
        trailheads.push_back(std::make_pair(i, map.size()));
      }
    }

    map.push_back(row);
  }

  std::cout << "\n" << std::endl;

  int sum = 0;
  for (const auto &trailhead : trailheads)
  {
    //std::unordered_set<std::pair<int, int>, pair_hash> visited;
    std::map<std::pair<int, int>, std::vector<std::pair<int, int>>> parents; // maybe its enough with parents? if parents is empty it is not yet visited?

    std::deque<std::pair<int, int>> queue;
    queue.push_back(trailhead);
    std::unordered_set<std::pair<int, int>, pair_hash> mountain_tops;
    // check left.
    while (!queue.empty())
    {
      std::pair<int, int> current_position = queue.front();
      queue.pop_front();

      process_position(current_position, parents, map, queue, mountain_tops);

      //std::cout << sum << std::endl;
    }

    
    for (auto &top : mountain_tops){
      std::vector<std::pair<int, int>> path;
      //std::cout << pair_to_string(top) << std::endl;
      sum += all_paths_to_rome(top, map, parents, path);
    } 
    //sum += mountain_tops.size();
    
    //std::cout << sum << std::endl;

    //std::cout << "finished processing trailhead (" << trailhead.first << ", " << trailhead.second << ") " << std::endl;
  }

  std::cout << sum << std::endl;

  return 0;
}
