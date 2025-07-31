
#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <string>
#include <unordered_set> 
#include <sstream>
#include <string_view>
#include <unordered_map>


/**
 * Idea:
 * 1.
 * Create a hashtable the given patterns. 
 * 
 * 2.
 * loop over every possible substring. See if the substring match a 
 * pattern in the hash. 
 * 
 * 3.
 * (some boolean variable here to keep track of the recursive matching)
 * If we find a match - we remove that part of the string - 
 * and recursively call step two - oring the return of the recursive function with our boolean.
 * (there will have to be two calls here - one to the left side of the substr and one to the right,
 * and these need to be ANDed together.)
 * (if any recursive call is true - then we found a match.)
 * 
 * if we reach this recursive function with a string of length zero -
 * that means we removed all patterns and can return true.
 * if we do not find a match for any potential substring. 
 * 
 * Very slow for large inputs - we need some memoization:
 * Idea: 
 * map already calculated strings in a hashmap - to values of true and false;
 * once we calculated we add all substrings to this hashmap and if it was possible to create. 
 * before we do a recursive calc - we check this hashmap and if its there we return its value. 
 * Could become very large hashmap? 
 * lets try.
 * 
 * Part 2: we need to memoize how many ways we can create a substr. 
 * We can no longer preempt the calculation - we need to calculate the substr
 * completely unless it is already in the cache. 
 * instead of AND the result we multiply the number of ways we can do the two substrs
 * if we can not do the left it does not matter if we can do the right. 
 * and if we can do the right in two ways and the left in two ways - it is multiplicatative.
 * and then we sum this result to what is now the bool possible - but will be a sum. then memo the result
 * and return it. 
 * 
 * I am not sure this works. 
 * Another way seems to be using a Trie data Structure. 
 * If we save all our substrings in the Trie. 
 * 
 */


bool possible_design(const std::string &pattern, const std::unordered_set<std::string> &towels, std::unordered_map<std::string, bool> &memo) {
  if (pattern.empty() ) {
      return 1;
  }

  auto it = memo.find(pattern);
  if (it != memo.end()) {
    return it->second;
  }
  //all substrings of the pattern
  int possible_ways = 0;
  for (size_t start = 0; start < pattern.length(); ++start) {
        std::string substring = "";
        for (size_t end = start; end < pattern.length(); ++end) {
            substring += pattern[end];
            
            if(towels.count(substring)) {
                std::string left = pattern.substr(0, start);
                std::string right = pattern.substr(end + 1);
                possible_ways += (possible_design(left, towels, memo) 
                * possible_design(right, towels, memo));
            }
        }
    }

  memo[pattern] = possible_ways;
  return possible_ways;
}

int main(int argc, char* argv[])
{
  std::string line;
  std::ifstream input("C:/Users/Dwight/Desktop/c_dev/AoC/19/part2/build/input.txt");

  std::unordered_set<std::string> towels;

  std::vector<std::string> patterns;

  //get the first line with towels.
  std::getline(input, line);
  std::istringstream ss(line);
  std::string towel;
  while (std::getline(ss, towel, ',')) {
      // Trim whitespace from the token
      size_t start = towel.find_first_not_of(" \t");
      size_t end = towel.find_last_not_of(" \t");
      if (start != std::string::npos) {
          towel = towel.substr(start, end - start + 1);
      }

      // Add the trimmed token to the vector
      towels.insert(towel);
  }

  /* 
  std::string test = "uuwr";

  std::cout << (towels.count(test) ? "true" : "false") << std::endl; */

  /* for (const auto t : towels ) {
    std::cout << t << ", ";
  } */
  std::cout << std::endl;

  std::string pattern;
  while(std::getline(input, line)) {
    if (line.empty()) {
      continue;
    }
    // Trim whitespace from the token
    size_t start = line.find_first_not_of(" \t");
    size_t end = line.find_last_not_of(" \t");
    if (start != std::string::npos) {
        pattern = line.substr(start, end - start + 1);
    }
    
    patterns.push_back(pattern);
  }

  /* for (const auto p : patterns ) {
    std::cout << p << ", ";
  } */

  //found all patterns. 

  int number_of_designs_possible = 0;
  std::unordered_map<std::string, bool> memo;

  for (const auto &pattern : patterns){
    number_of_designs_possible += possible_design(pattern, towels, memo);
  }

  std::cout << number_of_designs_possible << std::endl;

  return 0;
}