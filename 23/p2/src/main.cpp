#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <algorithm>

#define INPUT "C:/Users/Dwight/Desktop/c_dev/AoC/23/p2/build/input.txt"


/**
 * Idea for solution. 
 * 
 * We want to find the biggest clique. 
 * If we consider the problem - The largest possible clique can only be as large as the size of the adjacency list for the node that has the 
 * most neighbors. 
 * 
 * So we sort the nodes by the size of their adjacency list. 
 * 
 * The other nodes adjacency lists have to be equally large - as they have to have as many neighbors. 
 * 
 * ... 
 * 
 * I see now, they are all equally large. So it was not that easy. 
 * 
 * Lets look into the Bron kerbosch algorithm for maximum cliques a little bit. 
 * 
 * The idea seems to be to have 3 sets, call the function recursively to find the max
 * 
 * We keep a potential set of nodes, which in the beginning is all nodes. 
 * 
 * For all of the nodes in the potential set, we run the algorithm recursively, 
 * 
 * we remove the selected node from the potential set.
 * Intersect the Neigbors of the selected node with the set of potential nodes. 
 * 
 * we keep our selected nodes in a separate set. 
 * 
 * 
 */

int getValue(const char* a) {
    return (a[0] - 'a' + (a[1] - 'a') * (('z' - 'a') + 1)) + 1;  // Node ID
}


void fromValue(int i, char* buffer) {
  int totalLetters = ('z' - 'a') + 1;  // 26 letters.
  buffer[0] = 'a' + ((i - 1) % totalLetters);  
  buffer[1] = 'a' + ((i - 1) / totalLetters); 
  buffer[2] = '\0';  
}

/**
 * Return the intersection, 
 * for all nodes in one, check if it is in the other set, if it is, add it to the return set. 
 */
std::unordered_set<int> set_intersection(const std::unordered_set<int> &a, const std::unordered_set<int> &b) {
  std::unordered_set<int> intersected;

  for (int i : a) {
    if (b.count(i)){
      intersected.insert(i);
    }
  }
  return intersected;
}

std::unordered_set<int> set_union(const std::unordered_set<int> &a, const std::unordered_set<int> &b) {
  std::unordered_set<int> u;

  for(int i : a) {
    u.insert(i);
  }
  for (int i : b){
    u.insert(i);
  }

  return u;
}

int main() {
  std::ifstream inputFile(INPUT);
  if (!inputFile) {
      std::cerr << "Error: could not open file!" << std::endl;
      return 1;
  }
  // Adjacency list using a map of sets
  std::unordered_map<int, std::unordered_set<int>> graph;

  std::string line;
  while (std::getline(inputFile, line)) {
    int compA = getValue(&line[0]);
    int compB = getValue(&line[3]);

    // Insert connections (bidirectional)
    graph[compA].insert(compB);
    graph[compB].insert(compA);
  }




  return 0;
}
