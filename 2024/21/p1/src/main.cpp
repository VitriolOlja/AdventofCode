#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <deque>
#include <array>

/**
 * 1.
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
 *
 *
 * 2.
 * Having an integer cost to calculate the movements is maybe not possible.
 *
 * I changed the hashmap now to return a char vector of movements.
 * lets see where thay leads us.
 *
 * 3. For the order of movements. I think we always want to get left last.
 * And always right before we go down or up. Otherwise we might have to go around the free space
 * (where the robot will panic unrecoverably)
 * which i denote here by '#'.
 *
 * 3.
 * I think my first idea about the order of movements is not correct. > ^ v < does not seem to work optimally for
 * 379A
 *
 * I think there is a shortest solution expect when handling the empty space.
 *
 * We know we want to always have the movements together as much as possible meaning ^ > ^ > is less optimal than
 * ^^>>,
 * So when doing the dijkstra analysis
 *  we can split it maybe into two parts, a delta_x and a delta_y part
 * the empty space is always left of the...
 *
 * Interestingly, I think we always want to go left first, except when the movement becomes
 *
 * Maybe we make the dijkstra analysis in two steps.
 * We judge the number of steps aswell as the number of turns.
 *
 * We can calculate the turn in two ways -> either we keep a count of turns in the queue,
 * or we count the amount of turns after by looping through the list of directions.
 * If we would have a long array of directions, looping through it would probably be slow, but
 * for this it will only be like 3 long, and it will not be done that often,
 * so lets just loop through it.
 * 
 * hmm. this seems to not work either. 
 * When taking left first, <v will have the same cost as v< 
 * which is what we want, but we want v<< instead of <v< but it will never reach and attempt it
 * because the algorithm will reach <v before it reaches v< and so the algorithm will terminate 
 * before it has any chance to check 
 * 
 * okay I will be honest. 
 * I saw a pattern and made the if statement at line 149 a strict less than, 
 * meaning it will override if it another comes along with the same cost and equal turns.
 * 
 * and then i flipped the directions around to the same order i hade before -
 * which means the < will override because it will always be last. 
 * This magically turned out to give the right answer. Was just a pure intuition that seemed to 
 * work but I don't 100% understand why. 
 * 
 * Lets see on if it works on part2. 
 *
 * */

int count_turns(const std::vector<char> &moves)
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

/**
 * This function runs a dijkstra search to find the optimal path from any key to any other key in on the keypads.
 */
template <size_t WIDTH, size_t HEIGHT>
void full_dijkstra(std::unordered_map<char, std::unordered_map<char, std::pair<int, std::vector<char>>>> &keypad_cost,
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

      std::unordered_map<char, std::pair<int, std::vector<char>>> dijkstra;
      dijkstra.insert({keypad[y][x], std::make_pair(0, std::vector<char>())});

      // position, cost to get here, movements made / button presses to get to this position from the previous.
      std::deque<std::tuple<std::pair<int, int>, int, std::vector<char>>> queue;
      queue.push_back({{x, y}, 0, std::vector<char>()});

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

  std::unordered_map<char, std::unordered_map<char, std::pair<int, std::vector<char>>>> numeric_keypad_cost;
  full_dijkstra(numeric_keypad_cost, numeric_keypad);

  std::unordered_map<char, std::unordered_map<char, std::pair<int, std::vector<char>>>> directional_keypad_cost;

  full_dijkstra(directional_keypad_cost, directional_keypad);

  /* for (auto &c : directional_keypad_cost['A']['<'].second) {
    std::cout << c;
  }
  std::cout << std::endl;
  

  return 0; */
  // std::vector<char> code = {'0', '2', '9', 'A'};
  // std::vector<char> code = {'0'};

  long long complexity = 0;

  // pointer to the robots current position on this keyboard. it starts on A

  for (const auto &[code, numeric_part] : codes)
  {
    std::vector<char> numeric_movements;
    std::vector<char> first_directional_movements;
    std::vector<char> second_directional_movements;
    char current_position_numeric = 'A';

    for (const char &c : code)
    {

      // the movements we need to reach the first key in the code
      // is saved in the hashmap, we get there by going from the current key to the key in the code.
      const auto &numeric_movements_here = numeric_keypad_cost[current_position_numeric][c].second;

      // to go to that position, we also have to move on our first directional keyboard.
      char current_position_first_directional = 'A';
      for (const char &nmc : numeric_movements_here)
      {
        // for every movement we make on our numeric keyboard there are movements on the first directional
        const auto &first_directional_movements_here = directional_keypad_cost[current_position_first_directional][nmc].second;

        char current_position_second_directional = 'A';
        // for every movement on the first directional keyboard there are several movements on the second directional keyboard.
        for (const char &first_dir : first_directional_movements_here)
        {

          const auto &second_directional_movements_here = directional_keypad_cost[current_position_second_directional][first_dir].second;
          // insert those movements in the second directional keypresses.
          second_directional_movements.insert(second_directional_movements.end(), second_directional_movements_here.begin(), second_directional_movements_here.end());
          second_directional_movements.push_back('A'); // Press the button
          current_position_second_directional = first_dir;
        }

        // add the first directional movements.
        first_directional_movements.insert(first_directional_movements.end(), first_directional_movements_here.begin(), first_directional_movements_here.end()); // copy into movements
        first_directional_movements.push_back('A');                                                                                                              // Press the button

        const auto &second_directional_movements_here = directional_keypad_cost[current_position_second_directional]['A'].second;
        second_directional_movements.insert(second_directional_movements.end(), second_directional_movements_here.begin(), second_directional_movements_here.end()); // copy into movements
        second_directional_movements.push_back('A');                                                                                                                 // Press the button

        current_position_first_directional = nmc;
      }

      numeric_movements.insert(numeric_movements.end(), numeric_movements_here.begin(), numeric_movements_here.end()); // copy into movements

      numeric_movements.push_back('A'); // Press the button
      // move back to A with the first directional keypad.
      const auto &first_directional_movements_here = directional_keypad_cost[current_position_first_directional]['A'].second;
      first_directional_movements.insert(first_directional_movements.end(), first_directional_movements_here.begin(), first_directional_movements_here.end()); // copy into movements

      // we of course need to use the second directional keypad to move the first.
      char current_position_second_directional = 'A';
      for (const auto &c : first_directional_movements_here)
      {
        const auto &second_directional_movements_here = directional_keypad_cost[current_position_second_directional][c].second;
        second_directional_movements.insert(second_directional_movements.end(), second_directional_movements_here.begin(), second_directional_movements_here.end()); // copy into movements
        second_directional_movements.push_back('A');                                                                                                                 // Press the button
        current_position_second_directional = c;
      }

      first_directional_movements.push_back('A'); // Press the button

      const auto &second_directional_movements_here = directional_keypad_cost[current_position_second_directional]['A'].second;
      second_directional_movements.insert(second_directional_movements.end(), second_directional_movements_here.begin(), second_directional_movements_here.end()); // copy into movements
      second_directional_movements.push_back('A');                                                                                                                 // Press the button

      current_position_numeric = c; // change the current position.
    }

    for (const auto &c : second_directional_movements)
    {
      std::cout << c << "";
    }
    std::cout << " " << second_directional_movements.size() << " " << numeric_part << "\n";

    complexity += second_directional_movements.size() * numeric_part;
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