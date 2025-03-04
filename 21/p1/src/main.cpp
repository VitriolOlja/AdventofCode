#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <deque>
#include <array>

/**
 * This one seems very hard. I've been waiting a long time. And solved day 22 before.
 * I am thinking first -> that for each of the keyboards I will want to have a hashmap node and hashmap
 *
 * So that for each node i can find the cost (amount of steps) to get to any other node.
 * hash the starting node to a map -> i get the hashmap for that node ->
 * hash the end node to that map -> i get the cost to get from start to end
 *
 * std::unordered_map<std::pair<x, y>, std::unordered_map<std::pair<x, y>, int>>
 *
 * one of these will have to be constructed
 */

struct pairHash
{
  std::size_t operator()(const std::pair<int, int> &pair) const
  {
    std::hash<int> intHash;
    std::size_t hash = 0;
    hash ^= intHash(pair.first) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    hash ^= intHash(pair.second) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    return hash;
  }
};

template <size_t WIDTH, size_t HEIGHT>
void full_dijkstra(std::unordered_map<char, std::unordered_map<char, int>> &keypad_cost,
                   const std::array<std::array<char, WIDTH>, HEIGHT> &keypad)
{
  for (int y = 0; y < HEIGHT; y++)
  {
    for (int x = 0; x < WIDTH; x++)
    {
      const char &start = keypad[y][x];
      if (start == '#')
      {
        continue;
      }

      std::unordered_map<char, int> dijkstra;
      dijkstra.insert({keypad[y][x], 0});

      std::deque<std::tuple<std::pair<int, int>, int>> queue;
      queue.push_back({{x, y}, 0});

      while (!queue.empty())
      {
        const auto [position, cost] = queue.front();
        queue.pop_front();
        const char &current = keypad[position.second][position.first];

        auto it = dijkstra.find(current);
        if (it != dijkstra.end())
        {
          if (cost > it->second)
          {
            continue;
          }
        }

        dijkstra.insert({current, cost});

        if (position.first + 1 < width)
        {
          queue.push_back({{position.first + 1, position.second}, cost + 1});
        }
        if (position.first - 1 >= 0)
        {
          queue.push_back({{position.first - 1, position.second}, cost + 1});
        }
        if (position.second + 1 < height)
        {
          queue.push_back({{position.first, position.second + 1}, cost + 1});
        }
        if (position.second - 1 >= 0)
        {
          queue.push_back({{position.first, position.second - 1}, cost + 1});
        }
      }

      keypad_cost.insert({start, dijkstra});
    }
  }
}

int main()
{
  constexpr size_t numeric_keypad_width = 3;
  constexpr size_t numeric_keypad_height = 4;
  constexpr std::array<std::array<char, numeric_keypad_width>, numeric_keypad_height> numeric_keypad = {{
    {'7', '8', '9'},
    {'4', '5', '6'},
    {'1', '2', '3'},
    {'#', '0', 'A'}
  }};

  constexpr size_t directional_keypad_width = 3;
  constexpr size_t directional_keypad_height = 2;

  constexpr std::array<std::array<char, directional_keypad_width>, directional_keypad_height> directional_keypad = {{
    {'#', '^', 'A'},
    {'<', 'v', '>'}
  }};

  std::unordered_map<char, std::unordered_map<char, int>> numeric_keypad_cost;
  full_dijkstra(numeric_keypad_cost, numeric_keypad);

  std::unordered_map<char, std::unordered_map<char, int>> directional_keypad_cost;

  full_dijkstra(directional_keypad_cost, directional_keypad);
  

  int what = numeric_keypad_cost['A']['1'];

  std::cout << what << std::endl;
  return 0;
}