#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <deque>
#include <array>
#include <string>

/**
 * So first i just constructued a few loops, better than the way i solved it in p1. 
 * But this does not lend itself well to memoization. 
 * We need to calculate the the movements 
 * 
 * I am thinking, that for each movement, ^ < v > I calculate the corresponding cost, for 25 arms. 
 * this is only 4 different variations, although it loops 
 * 
 * 
 * Nevermind. 
 * I hash the sequence which will be a sequence of <> and then ending with an A. 
 * e.g. v>A or ^<A etc. and then i count how many of that sequence i have. we can the amount of that sequence with whatever we 
 * get from the parts. 
 *
 * */

int count_turns(const std::string &moves)
{
  if (moves.empty())
    return 0; // No turns in an empty path

  int turns = 0;
  char prev_direction = moves[0];

  for (size_t i = 1; i < moves.size(); i++)
  {
    if (moves[i] != prev_direction)
    {
      turns++;
      prev_direction = moves[i];
    }
  }

  return turns;
}

/**
 * This function runs a dijkstra search to find the optimal path from any key to any other key in on the keypads.
 */
template <size_t WIDTH, size_t HEIGHT>
void full_dijkstra(std::unordered_map<char, std::unordered_map<char, std::pair<int, std::string>>> &keypad_cost,
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

      std::unordered_map<char, std::pair<int, std::string>> dijkstra;
      dijkstra.insert({keypad[y][x], std::make_pair(0, "")});

      // position, cost to get here, movements made / button presses to get to this position from the previous.
      std::deque<std::tuple<std::pair<int, int>, int, std::string>> queue;
      queue.push_back({{x, y}, 0, ""});

      while (!queue.empty())
      {
        auto [position, cost, movements] = queue.front();
        queue.pop_front();
        const char &current = keypad[position.second][position.first];

        // check if we added this before.
        auto it = dijkstra.find(current);
        if (it != dijkstra.end())
        {
          // if we have compare the cost.
          if (cost > it->second.first)
          {
            continue;
          }

          if (it->second.first > 1 && cost == it->second.first)
          {
            // if the cost is the same, also compare the amount of turns, if we have more turns, continue.

            int previous_turns = count_turns(it->second.second);
            int this_turns = count_turns(movements);

             
            // if the one previous saved has less or equal amount of turns to this, but same cost. We skip this.
            // we do not want to save one that turns more. that is bad for the algorithm.
            if (previous_turns < this_turns)
            {
              continue;
            }
          }
        }

        dijkstra[current] = std::make_pair(cost, movements);

        if (position.first + 1 < WIDTH && keypad[position.second][position.first + 1] != '#')
        {
          auto new_movements = movements;
          new_movements.push_back('>');
          queue.push_back({{position.first + 1, position.second}, cost + 1, new_movements});
        }
        if (position.second - 1 >= 0 && keypad[position.second - 1][position.first] != '#')
        {
          auto new_movements = movements;
          new_movements.push_back('^');
          queue.push_back({{position.first, position.second - 1}, cost + 1, new_movements});
        }
        if (position.second + 1 < HEIGHT && keypad[position.second + 1][position.first] != '#')
        {
          auto new_movements = movements;
          new_movements.push_back('v');
          queue.push_back({{position.first, position.second + 1}, cost + 1, new_movements});
        }
        if (position.first - 1 >= 0 && keypad[position.second][position.first - 1] != '#')
        {
          auto new_movements = movements;
          new_movements.push_back('<');
          queue.push_back({{position.first - 1, position.second}, cost + 1, new_movements});
        }
        
      }

      keypad_cost.emplace(start, dijkstra);
    }
  }
}

int main()
{
  std::ifstream input("C:/Users/Dwight/Desktop/c_dev/AoC/21/p1/build/input.txt");
  std::string line;

  std::vector<std::pair<std::string, int>> codes;
  while (std::getline(input, line))
  {
    codes.push_back(std::make_pair(line, std::stoi(line.substr(0, 3))));
  }

  constexpr size_t numeric_keypad_width = 3;
  constexpr size_t numeric_keypad_height = 4;
  constexpr std::array<std::array<char, numeric_keypad_width>, numeric_keypad_height> numeric_keypad = {{{'7', '8', '9'},
                                                                                                         {'4', '5', '6'},
                                                                                                         {'1', '2', '3'},
                                                                                                         {'#', '0', 'A'}}};

  constexpr size_t directional_keypad_width = 3;
  constexpr size_t directional_keypad_height = 2;

  constexpr std::array<std::array<char, directional_keypad_width>, directional_keypad_height> directional_keypad = {{{'#', '^', 'A'},
                                                                                                                     {'<', 'v', '>'}}};

  std::unordered_map<char, std::unordered_map<char, std::pair<int, std::string>>> numeric_keypad_cost;
  full_dijkstra(numeric_keypad_cost, numeric_keypad);

  std::unordered_map<char, std::unordered_map<char, std::pair<int, std::string>>> directional_keypad_cost;

  full_dijkstra(directional_keypad_cost, directional_keypad);

  /* for (auto &c : directional_keypad_cost['A']['<'].second) {
    std::cout << c;
  }
  std::cout << std::endl;
  

  return 0; */
  // std::vector<char> code = {'0', '2', '9', 'A'};
  // std::vector<char> code = {'0'};

  int number_of_robots = 25;


  std::unordered_map<char, std::string> final_cost;
  

  long long complexity = 0;

  // pointer to the robots current position on this keyboard. it starts on A
  
  for (const auto &[code, numeric_part] : codes)
  {

    char current_position_numeric = 'A';

    std::unordered_map<std::string, long long> occurences;

    for (const char &c : code)
    {
      // the movements we need to reach the first key in the code
      // is saved in the hashmap, we get there by going from the current key to the key in the code.
      auto numeric_movements_here = numeric_keypad_cost[current_position_numeric][c].second;
      numeric_movements_here.push_back('A'); //the intersting thing about going to A, is that we are back at the beginning.
      current_position_numeric = c;

      if (!occurences.count(numeric_movements_here)) {
        occurences[numeric_movements_here] = 1;
      }
      else {
        occurences[numeric_movements_here] += 1; 
      }
    }


    //for every robot.
    for (int i = 0; i < number_of_robots; i++) {
      //copy of hashmap.
      std::unordered_map<std::string, long long> occ; //this is a bad name here but we need some 
      //intermediate map to save values to. 

      //for every sequence/pattern in the hashmap, we loop over it.
      //we make the subsequence for each sequence, and then we add it to the hashmap, 
      //with the amount of times that it occurs.  
      for (const auto &[sequence, number_of_times] : occurences) {

        char current_position = 'A';

        for (const char &m : sequence)
        {
          
          auto movements_here = directional_keypad_cost[current_position][m].second;
          movements_here.push_back('A');

          //now because there are number of times occurences of that pattern, we multiply by it here. 
          if (!occ.count(movements_here)) {
            occ[movements_here] = number_of_times;
          }
          else {
            occ[movements_here] += (1 * number_of_times); 
          }
          
          current_position = m;
        }
      }

      //when we are done on this level, we can set occurences to the occurences at this level, to move to next robot.
      occurences = occ;
    }

    //now we need to find the total length by summing the values in the map

    long long total = 0;
    for (const auto &[pattern, number_of_times] : occurences) {
      //the total is the pattern.size() * number_of_times. 
      total += pattern.size() * number_of_times;
    }

    complexity += total * numeric_part;
  }

  std::cout << "Complexity: " << complexity << std::endl;

  std::cout << "\n";

  /**
  for (const auto &c : first_directional_movements) {
    std::cout << c << " ";
  }

  std::cout << "\n";


  for (const auto &c : numeric_movements) {
    std::cout << c << " ";
  }


  std::cout << std::endl;
 */
  return 0;
}