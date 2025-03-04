/**
 * 16777216 = 2^24
 */

#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <array>

inline uint64_t getNext(uint64_t secret)
{
  constexpr unsigned int mask = (1u << 24) - 1;
  // multiply by 64 2^6
  uint64_t res = secret << 6;
  secret = secret ^ res; // mix
  secret &= mask;

  res = secret >> 5;
  secret = secret ^ res; // mix
  secret &= mask;

  res = secret << 11;
  secret = secret ^ res; // mix
  secret &= mask;

  return secret;
}
struct fourIntTupleHash
{
    std::size_t operator()(const std::array<int, 4> &tuple) const
    {
        std::hash<int> intHash;
        std::size_t hash = 0;
        for (const auto &val : tuple)
        {
            hash ^= intHash(val) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};


/**
 * first hash maps 4 numbers to amount of bananas and how many secretes.
 * For example
 * For 1 secret we may reach the same sequence again, and then we dont want to increase
 * so we hash if it already exist a value we check the second value which is the last index
 * of secret, if that last index is lower than the current loop index for our secrets,
 * then we add the bananas and increment it, else we do nothing.
 * We could also have a visited -> but this way we do not need multiple hashmaps
 * which in case of collisions could lead to double time.
 *
 */
void tt(std::vector<uint64_t> secrets, std::unordered_map<std::array<int, 4>, std::pair<int, int>, fourIntTupleHash> &map)
{
  for (int i = 0; i < secrets.size(); i++)
  {

    uint64_t secret = secrets[i];
    int last_digit_a = static_cast<int>(secret % 10); // last digit of secret

    std::array<int, 4> buffer = {};
    // do it for 3 numbers first, and fill the buffer.
    for (int j = 1; j < 4; j++)
    {
      secret = getNext(secret);
      int last_digit_b = static_cast<int>(secret % 10); // last digit of second number

      int diff = last_digit_b - last_digit_a; // new minus old = difference
      buffer[j] = diff;
      last_digit_a = last_digit_b;
    }


    for (int u = 0; u < 1997; u++)
    {
      secret = getNext(secret);
      int last_digit_b = static_cast<int>(secret % 10); // last digit of second number
      // which is also the amount of bananas sold by the monkey.

      int diff = last_digit_b - last_digit_a; // new minus old = difference
      //move buffer to left, and add diff at buffer[4]
      std::rotate(buffer.begin(), buffer.begin() + 1, buffer.end());
      buffer[3] = diff;

      // check if we havent already added bananas for this secret at the current hash.
      if (!map.count(buffer))
      {
        map.insert({buffer, {last_digit_b, i}});
      }
      else if (map[buffer].second < i ){
        // if there is no addition at all on this sequence or sthe the sequence has not been counted for
        // this secret
        //  add the bananas.
        auto &[first, second] = map[buffer];
        first += last_digit_b;
        // consider this sequence already added.
        second = i;
      }

      last_digit_a = last_digit_b;
    }
  }
}


int main()
{
  std::string line;
  std::ifstream input("C:\\Users\\Dwight\\Desktop\\c_dev\\AoC\\22\\p2\\build\\input.txt");
  std::vector<uint64_t> secrets;

  while (std::getline(input, line))
  {
    unsigned long val = std::stoul(line);
    secrets.push_back(static_cast<uint64_t>(val));
  }

  std::unordered_map<std::array<int, 4>, std::pair<int, int>, fourIntTupleHash> map;
  tt(secrets, map);

  // Transfer to vector of pairs
  std::vector<std::pair<std::array<int, 4>, std::pair<int, int>>> vec(map.begin(), map.end());

  // Sort by the bananas.
  std::sort(vec.begin(), vec.end(),
            [](const std::pair<std::array<int, 4>, std::pair<int, int>> &a,
               std::pair<std::array<int, 4>, std::pair<int, int>> &b)
            {
              return a.second.first > b.second.first;
            });


  /* for (auto [first, second] : vec) {
    print_tuple(first);
    std::cout << " " << second.first << "\n";  
  } 
  
  std::cout << "\n"; */
  for (int i = 0; i < 10 && i < vec.size(); i++) {
    for (int j = 0; j < 4; j++){
      std::cout << vec[i].first[j] << " ";
    }
    std::cout << " = " << vec[i].second.first << std::endl;
  }

  return 0;
}