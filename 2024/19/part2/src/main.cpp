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
 * Hashmap seems like the zen road of Tries, 
 * Not as memory intensive as the array implementation.
 * and not as time consuming as the linked list implementation.
 */
struct HashMapTrieNode {
  bool is_word;
  std::unordered_map<char, HashMapTrieNode*> children;

  HashMapTrieNode() : is_word(false) {}
  ~HashMapTrieNode() {
    for (auto child : children) {
      delete(child.second);
    }
  }
};

void insert_word(const std::string &word, size_t index, HashMapTrieNode &node) {
  if (index == word.size()) {
    node.is_word = true;
    return;
  }

  if(node.children.count(word[index]) == 0) {
    node.children.insert({word[index], new HashMapTrieNode()});
  }

  insert_word(word, index + 1, *node.children[ word[index] ]); 
}

bool find(const std::string &word, size_t index, const HashMapTrieNode &node) { 
  if (index + 1 == word.size()) {
    return node.children.count(word[index]) ? node.children.at(word[index])->is_word : false;
  }

  return node.children.count(word[index]) ? find(word, index + 1, *node.children.at(word[index])) : false;
}

/**
 * we send in a word we want to construct
 * for each letter. we iterate 
 */
long long possible_designs(const std::string &word, const HashMapTrieNode &root, std::unordered_map<std::string, long long> &cache) {

  if (cache.count(word)){
    return cache[word];
  }

  long long designs = 0;

  size_t i = 0;

  //for debug
  //std::string current_word = word.substr(index, word.size() - index);

  const HashMapTrieNode *node = &root;
  //for each substring we can remove from the front of the node. 
  std::string current_substring;
  while(i < word.size() && node->children.count(word[i])) {
    current_substring += word[i];

    if (node->children.at(word[i])->is_word) {
      if (i + 1 == word.size()){
        return designs + 1; //we are done because there is no more of the word - we might already have some designs though for the shorter strings.
      }
      //else

      //check if cached here? if so return add it and keep going. 
      std::string remaining = word.substr(i + 1);
      designs += possible_designs(remaining, root, cache);  //if it is a word - and we still have more of the words - keep going recursively on that string.
    }

    node = node->children.at(word[i]);
    i += 1;
  } 

  //cache the word. 
  cache.insert({word, designs});

  return designs;
}


int main(int argc, char* argv[])
{
  std::string line;
  std::ifstream input("C:/Users/Dwight/Desktop/c_dev/AoC/19/part2/build/input.txt");

  HashMapTrieNode root; //

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
      insert_word(towel, 0, root);
  }


/*   // Tests for finding words
  std::cout << "Find 'r': " << (find("r", 0, root) ? "Found" : "Not Found") << std::endl;
  std::cout << "Find 'wr': " << (find("wr", 0, root) ? "Found" : "Not Found") << std::endl;
  std::cout << "Find 'b': " << (find("b", 0, root) ? "Found" : "Not Found") << std::endl;
  std::cout << "Find 'g': " << (find("g", 0, root) ? "Found" : "Not Found") << std::endl;
  std::cout << "Find 'bwu': " << (find("bwu", 0, root) ? "Found" : "Not Found") << std::endl;
  std::cout << "Find 'rb': " << (find("rb", 0, root) ? "Found" : "Not Found") << std::endl;
  std::cout << "Find 'gb': " << (find("gb", 0, root) ? "Found" : "Not Found") << std::endl;
  std::cout << "Find 'br': " << (find("br", 0, root) ? "Found" : "Not Found") << std::endl;

  // Tests for non-matching words/substrings
  std::cout << "Find 'bwr': " << (find("bwr", 0, root) ? "Found" : "Not Found") << std::endl;
  std::cout << "Find 'gr': " << (find("gr", 0, root) ? "Found" : "Not Found") << std::endl;
  std::cout << "Find 'wrb': " << (find("wrb", 0, root) ? "Found" : "Not Found") << std::endl;
  std::cout << "Find 'bwg': " << (find("bwg", 0, root) ? "Found" : "Not Found") << std::endl;
  std::cout << "Find 'bgr': " << (find("bgr", 0, root) ? "Found" : "Not Found") << std::endl;

 */


  std::vector<std::string> patterns;
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

  long long number_of_designs_possible = 0;
  std::unordered_map<std::string, long long> cache;
  for (const auto &pattern : patterns){
    number_of_designs_possible += possible_designs(pattern, root, cache);
  } 

  std::cout << number_of_designs_possible << std::endl;

  return 0;
}