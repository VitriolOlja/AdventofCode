#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <deque>
#include <algorithm>

struct CantorHash
{
  std::size_t operator()(const std::pair<int, int> &p) const
  {
    return (p.first + p.second) * (p.first + p.second + 1) / 2 + p.second;
  }
};

void print_map_and_cheat(const std::vector<std::vector<char>> &map, const std::pair<int, int> &a, const std::pair<int, int> &b) {
  for(int i = 0; i < map.size(); i++) {
    for (int j = 0; j < map[i].size(); j++) {
      if (i == a.second && j == a.first) {
        std::cout << "X";
      }
      else if (i == b.second && j == b.first) {
        std::cout << "Y";
      }
      else {
        std::cout << map[i][j];
      }
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

void checkCheat(const std::pair<int, int> a, const std::pair<int, int> b,
                int length_of_cheat,
                const std::vector<std::vector<char>> &map,
                const std::unordered_map<std::pair<int, int>, int, CantorHash> &cost_start_to,
                const std::unordered_map<std::pair<int, int>, int, CantorHash> &cost_end_to,
                std::unordered_map<int, int> &number_of_cheats,
                const std::pair<int, int> &end)
{

  // check if the second position is valid. and if it is measure the cheat.
  if (map[b.second][b.first] == '.' || map[b.second][b.first] == 'E')
  {
    int cost = cost_start_to.at(a) + length_of_cheat + cost_end_to.at(b);
    int cheat = cost_start_to.at(end) - cost;
    if (cheat > 0)
    {
      //print_map_and_cheat(map, a, b);
      if (!number_of_cheats.count(cheat))
      {
        number_of_cheats[cheat] = 1;
      }
      else
      {
        number_of_cheats[cheat] += 1;
      }
    }
  }
}

void checkkAllWithinRadius(
    const std::pair<int, int> &center,
    int radius,
    const std::vector<std::vector<char>> &map,
    const std::unordered_map<std::pair<int, int>, int, CantorHash> &cost_start_to,
    const std::unordered_map<std::pair<int, int>, int, CantorHash> &cost_end_to,
    std::unordered_map<int, int> &number_of_cheats,
    const std::pair<int, int> &end)
{

  for (int r = 2; r <= radius; r++)
  {
    for (int d_x = -r; d_x < r; d_x++)
    {
      int d_y = r - std::abs(d_x);
      if(d_y == 0) {
        continue;
      } 

      if (0 <= center.first + d_x && center.first + d_x < map[0].size() &&
          0 <= center.second + d_y && center.second + d_y < map.size())
      {
        checkCheat(center, {center.first + d_x, center.second + d_y}, r, map, cost_start_to, cost_end_to, number_of_cheats, end);
      }
      if (0 <= center.first + d_x && center.first + d_x < map[0].size() &&
          0 <= center.second - d_y && center.second - d_y < map.size())
      {
        checkCheat(center, {center.first + d_x, center.second - d_y}, r, map, cost_start_to, cost_end_to, number_of_cheats, end);
      }
    }
    if (0 <= center.first + r && center.first + r < map[0].size())
    {
      checkCheat(center, {center.first + r, center.second}, r, map, cost_start_to, cost_end_to, number_of_cheats, end);
    }
    if (0 <= center.first - r && center.first - r < map[0].size())
    {
      checkCheat(center, {center.first - r, center.second}, r, map, cost_start_to, cost_end_to, number_of_cheats, end);
    }
  }
}

void check_and_add(const std::pair<int, int> &pos,
                   const std::pair<int, int> &delta,
                   const int cost,
                   const std::vector<std::vector<char>> &map,
                   std::deque<std::tuple<std::pair<int, int>, int>> &stack,
                   const std::unordered_map<std::pair<int, int>, int, CantorHash> &cost_map)
{
  const int &x = pos.first + delta.first;
  const int &y = pos.second + delta.second;

  if (map[y][x] != '#')
  {
    // check if it is in cost_map, if not go there, else compare cost.
    const auto got = cost_map.find({x, y});

    if (got == cost_map.end())
    {
      stack.push_front({{x, y}, cost + 1});
    }
    else
    {
      int c = got->second;
      if (c > cost + 1)
      {
        // if the cost on the other node is higher than this node + 1 update it.
        stack.push_front({{x, y}, cost + 1});
      }
    }
  }
}

void printCheats(const std::unordered_map<int, int> &number_of_cheats)
{
  // Transfer to vector of pairs
  std::vector<std::pair<int, int>> cheats(number_of_cheats.begin(), number_of_cheats.end());

  // Sort by the saving value (the key) in ascending order.
  std::sort(cheats.begin(), cheats.end(),
            [](const std::pair<int, int> &a, const std::pair<int, int> &b)
            {
              return a.first < b.first;
            });

  int count_saving_hundred = 0;
  // Print them in the desired format:
  for (const auto &cheat : cheats)
  {
    int saving = cheat.first;
    int count = cheat.second;
    if (count == 1)
    {
      if (saving >= 100)
      {
        count_saving_hundred += 1;
      }
      std::cout << "There is one cheat that saves " << saving << " picoseconds." << std::endl;
    }
    else
    {
      if (saving >= 100)
      {
        count_saving_hundred += count;
      }
      std::cout << "There are " << count << " cheats that save " << saving << " picoseconds." << std::endl;
    }
  }

  std::cout << "There are " << count_saving_hundred << " cheats that saved more than 100 seconds";
}

/**
 * General idea of solution:
 * 1. 2d vector of map
 * 2. find start and end
 * 3. Make a dijkstra analysis of the map,
 *  3.1 first from start to each node
 *  3.2 then from end to each node
 * 4. Iterate through each node in the map.
 * 5. the cost to get to that node from startnode is what is in the costmap
 * 6. check for each node within a certain radius ->
 * 7. extract a potential cheat from node a to node b
 * if we can get to that node cheaper -> and cost(end) - cost(endofCheat)
 * total cost = cost_start_to(a) + cheatlength + cost_end_to(b)
 */

void dijkstra(const std::pair<int, int> &start,
              const std::vector<std::vector<char>> &map,
              std::unordered_map<std::pair<int, int>, int, CantorHash> &cost_map)
{
  std::deque<std::tuple<std::pair<int, int>, int>> stack;
  stack.push_front({start, 0});
  cost_map.insert({start, 0});

  while (!stack.empty())
  {
    auto [current, cost] = stack.front();
    stack.pop_front();

    cost_map.insert({current, cost});

    // left
    check_and_add(current, {-1, 0}, cost, map, stack, cost_map);
    // up
    check_and_add(current, {0, -1}, cost, map, stack, cost_map);
    // right
    check_and_add(current, {1, 0}, cost, map, stack, cost_map);
    // down
    check_and_add(current, {0, 1}, cost, map, stack, cost_map);
  }
}

int main()
{
  std::ifstream file("C:\\Users\\Dwight\\Desktop\\c_dev\\AoC\\20\\p2\\build\\input.txt");

  std::string line;

  std::vector<std::vector<char>> map;

  std::pair<int, int> start;
  std::pair<int, int> end;

  while (std::getline(file, line))
  {
    std::vector<char> row;
    for (const auto &c : line)
    {
      if (c == 'S')
      {
        start = std::make_pair(row.size(), map.size());
      }
      else if (c == 'E')
      {
        end = std::make_pair(row.size(), map.size());
      }
      row.push_back(c);
    }
    map.push_back(row);
  }

  std::cout << "Start: (" << start.first << ", " << start.second << ") End: (" << end.first << ", " << end.second << ") " << std::endl;

  // position, cost, cheated
  std::unordered_map<std::pair<int, int>, int, CantorHash> cost_start_to;
  std::unordered_map<std::pair<int, int>, int, CantorHash> cost_end_to;
  // std::unordered_map<std::pair<int, int>, std::pair<int, int>, CantorHash> cheat_map;

  dijkstra(start, map, cost_start_to);
  dijkstra(end, map, cost_end_to);

  // std::cout << "Cost at end is :" << cost_map[end] << std::endl;

  std::unordered_map<int, int> number_of_cheats;

  for (int row = 0; row < map.size(); row++)
  {
    for (int column = 0; column < map[row].size(); column++)
    {
      if (map[row][column] == '.' || map[row][column] == 'S')
      {
        checkkAllWithinRadius(std::make_pair(column, row), 20, map, cost_start_to, cost_end_to, number_of_cheats, end);
      }
    }
  }

  printCheats(number_of_cheats);

  return 0;
}