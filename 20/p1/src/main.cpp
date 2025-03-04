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
    if (count == 1) {
      if (saving >= 100){
        count_saving_hundred += 1; 
      }
      std::cout << "There is one cheat that saves " << saving << " picoseconds." << std::endl;
    }
    else {
      if (saving >= 100){
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

  std::ifstream file("C:\\Users\\Dwight\\Desktop\\c_dev\\AoC\\20\\p1\\build\\input.txt");

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
      { // no point of cheating on end.
        // check each node within radius.
        // left
        if (column - 1 > 0 && map[row][column - 1] == '#')
        {
          // see if we earn antyhing frm going here
          if (map[row][column - 2] == '.' || map[row][column - 2] == 'E')
          {
            int total_cost = cost_start_to[std::make_pair(column, row)] + 2 + cost_end_to[std::make_pair(column - 2, row)];
            int saved = cost_start_to[end] - total_cost;
            if (saved > 0)
            {
              if (!number_of_cheats.count(saved))
              {
                number_of_cheats[saved] = 1;
              }
              else
              {
                number_of_cheats[saved] += 1;
              }
            }
          }
        }
        // up
        if (row - 1 > 0 && map[row - 1][column] == '#')
        {
          if (map[row - 2][column] == '.' || map[row - 2][column] == 'E')
          {
            int total_cost = cost_start_to[std::make_pair(column, row)] + 2 + cost_end_to[std::make_pair(column, row - 2)];
            int saved = cost_start_to[end] - total_cost;
            if (saved > 0)
            {
              if (!number_of_cheats.count(saved))
              {
                number_of_cheats[saved] = 1;
              }
              else
              {
                number_of_cheats[saved] += 1;
              }
            }
          }
        }
        // right
        if (column + 2 < map[row].size() && map[row][column + 1] == '#')
        {
          if (map[row][column + 2] == '.' || map[row][column + 2] == 'E')
          {
            int total_cost = cost_start_to[std::make_pair(column, row)] + 2 + cost_end_to[std::make_pair(column + 2, row)];
            int saved = cost_start_to[end] - total_cost;
            if (saved > 0)
            {
              if (!number_of_cheats.count(saved))
              {
                number_of_cheats[saved] = 1;
              }
              else
              {
                number_of_cheats[saved] += 1;
              }
            }
          }
        }
        // down
        if (row + 2 < map.size() && map[row + 1][column] == '#')
        {
          if (map[row + 2][column] == '.' || map[row + 2][column] == 'E')
          {
            int total_cost = cost_start_to[std::make_pair(column, row)] + 2 + cost_end_to[std::make_pair(column, row + 2)];
            int saved = cost_start_to[end] - total_cost;
            if (saved > 0)
            {
              if (!number_of_cheats.count(saved))
              {
                number_of_cheats[saved] = 1;
              }
              else
              {
                number_of_cheats[saved] += 1;
              }
            }
          }
        }
      }
    }
  }

  printCheats(number_of_cheats);

  return 0;
}