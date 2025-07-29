#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <algorithm>

#define INPUT "input.txt"

int getValue(const char* a) {
    return (a[0] - 'a' + (a[1] - 'a') * (('z' - 'a') + 1)) + 1;  // Node ID
}


void fromValue(int i, char* buffer) {
  int totalLetters = ('z' - 'a') + 1;  // 26 letters.
  buffer[0] = 'a' + ((i - 1) % totalLetters);  
  buffer[1] = 'a' + ((i - 1) / totalLetters); 
  buffer[2] = '\0';  
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


  std::unordered_set<int> visited;

  std::deque<int> stack;
   // Iterate over the keys of the unordered_map and add them to the stack
  for (const auto& pair : graph) {
    stack.push_back(pair.first);
  }

  

  //outer loop - so that we look at each node in case the graph is not connected. 
  while (!stack.empty()) {
    int node = stack[0];
    stack.pop_front();

    
    if (visited.count(node) == 0) {
      //we have not been here before - lets find the largest clique here. 

      std::vector<int> current_network = {node}; //vector to store network. 
      std::vector<int> candidates; //here we push back all neighbours of the component.

      for (const int& neighbour : graph[node]) {
        
      }

      for (const int& n : current_network) {
        if (graph[n].count(node)) {
          
        }
      }



    }
  }

  return 0;
}
