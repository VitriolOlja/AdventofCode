#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <list>
#include <unordered_set>

#define stone std::string
#define amount long long

void find_set(std::unordered_map<stone, amount> &stones, const stone &s, const amount &occ) {
  auto it = stones.find(s);
  if (it != stones.end())
  {
   it->second += occ;
  }
  else {
    stones.insert({s, occ});
  }
}

std::unordered_map<stone, amount> blink(const std::unordered_map<stone, amount> &in_stones)
{
  std::unordered_map<stone, std::vector<stone>> cache;

  std::unordered_map<stone, amount> out_stones;

  for (auto & s : in_stones)
  {
    amount occurences = s.second;
    if (occurences == 0) {
      continue;
    }

    auto it = cache.find(s.first);
    if (it != cache.end())
    {
      for (stone &st : it->second)
      {
        find_set(out_stones, st, occurences);
      }
      continue;
    }

    if (s.first.size() == 1 && s.first == "0")
    {
      find_set(out_stones, "1", occurences);
    }
    else if (s.first.size() % 2 == 0)
    {
      size_t size = s.first.size();
      stone first = std::to_string(std::stoll(s.first.substr(0, size / 2)));
      find_set(out_stones, first, occurences);

      stone second = std::to_string(std::stoll(s.first.substr(size / 2, size / 2)));
      find_set(out_stones, second, occurences);
      cache.insert(std::make_pair(s.first, std::vector<stone>{first, second}));
    }
    else
    {
      long long number = std::stoll(s.first);
      number *= 2024;
      std::string str = std::to_string(number);
      find_set(out_stones, str, occurences);
      cache.insert({s.first, std::vector<stone>{str}});
    }
  }

  return out_stones;
}

int main(int argc, char* argv[]) {
  size_t value;
  // Check if an argument is provided
  if (argc < 2) {
    std::cerr << "Usage: ./program <integer_value>" << std::endl;
    return 1; // Exit with error code
  }

  try {
    value = std::stoi(argv[1]);
  } catch (const std::invalid_argument& e) {
      std::cerr << "Error: The argument must be a valid integer!" << std::endl;
      return 1; // Exit with error code
  } catch (const std::out_of_range& e) {
      std::cerr << "Error: The argument is out of range for an integer!" << std::endl;
      return 1; // Exit with error code
  }

  
  std::string input = "0 5601550 3914 852 50706 68 6 645371";
  //std::string input = "125 17";

  std::unordered_map<stone, amount> stones;
  size_t first = 0;
  size_t last = input.find(' ');
  while (last != std::string::npos)
  {
    // stones.push_back(std::stoi(input.substr(first, last - first)));
    std::string s = input.substr(first, last - first);

    find_set(stones, s, 1);

    first = last + 1;
    last = input.find(' ', first);
  }
  std::string s = input.substr(first, last - first);
  find_set(stones, s, 1);

  size_t num_of_blinks = value;
  for (int i = 0; i < num_of_blinks; i++)
  {
    stones = blink(stones);
  }

  long long total = 0;
  for (auto s : stones) {
    total += s.second;
  } 

  std::cout << total << "\n";
  std::cout << std::endl;

  return 0;
}
