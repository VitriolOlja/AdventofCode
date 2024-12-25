#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <array>
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

// DFS Helper Function
void dfs(int node, std::unordered_map<int, std::unordered_set<int>>& graph, 
         std::unordered_set<int>& visited, std::vector<int>& component) {
    visited.insert(node);  // Mark the node as visited
    component.push_back(node);  // Add it to the current component
    for (int neighbor : graph[node]) {
        if (visited.find(neighbor) == visited.end()) {  // If not visited
            dfs(neighbor, graph, visited, component);
        }
    }
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


    std::unordered_set<std::string> visited;
    std::vector<std::vector<int>> networks;

    for (int i = 0; i < ('z' - 'a') + 1; i++) {
      //char buffer[3];
      //fromValue('t' - 'a' + ('z' - 'a' + 1) * i + 1, buffer) ;
      //std::cout << buffer << std::endl ;
      int t_values = 't' - 'a' + ('z' - 'a' + 1) * i + 1;

      for (const int &compA : graph[t_values]) {
        //check if there are any networks between num. and some other in 
        for (const int &compB : graph[t_values]) {
          if (compA < compB) {
            if (graph[compA].count(compB) ) { // || graph[compB].count(compA)
              
              std::vector<int> arr = {t_values, compA, compB};
              std::sort(arr.begin(), arr.end());

              std::string networkKey = std::to_string(arr[0]) + "," +
                                         std::to_string(arr[1]) + "," +
                                         std::to_string(arr[2]);

              if (visited.count(networkKey) == 0) {  
                    networks.push_back(std::vector<int>{t_values, compA, compB});  
                    visited.insert(networkKey);  
                }
            }
          }
        }
      }
    }


    std::cout << networks.size() << std::endl;

    return 0;
}
