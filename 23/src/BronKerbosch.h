#ifndef BK_H
#define BK_H

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>



class BK {
public:
  BK(const std::unordered_map<int, std::unordered_set<int>> &graph, const std::unordered_set<int> &subset);


  void findCliques();




private:
  const std::unordered_map<int, std::unordered_set<int>> &graph;
  const std::unordered_set<int> &subset;

  /*
    R - current clique
    P - candidate set
    X - exclusion set
  */
  void BronKerbosch(const std::unordered_set<int> &R, const std::unordered_set<int> &P, const std::unordered_set<int> &X);


};

#endif