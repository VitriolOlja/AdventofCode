#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <array>

#define my_type long long


int main() {

  std::string line;
  std::ifstream input("C:/Users/Dwight/Desktop/c_dev/AoC/13/part2/build/input.txt");

  //3 tokens to press A, and 1 token to press B

  std::vector<std::array<std::array<my_type, 2>, 3>> machines;
  int i = 0;
  std::array<std::array<my_type, 2>, 3> machine;

  while(std::getline(input, line)) {
    if (i == 3) { //we could also have every fourth..
      machines.push_back(machine);
      i = 0;
      machine = {};
      continue;
    }
    //magic math
    int first = line.find_first_of('X') + 2;
    int comma = line.find_first_of(',');
    machine[i][0] = std::stoll(line.substr(first, comma - first));
    machine[i][1] = std::stoll(line.substr(comma + 4, line.size() - comma - 4));
    i++;
  }

  if (i == 3) {
    machines.push_back(machine);
  }

  my_type total_cost = 0;

  std::cout << "\n\n";
  for (const auto &[A, B, point] : machines) {
    const auto &[Ax, Ay] = A; //knapp A
    const auto &[Bx, By] = B;
    const auto &[x, y] = point; 
    my_type x_new = x + 10000000000000;
    my_type y_new = y + 10000000000000;

    std::cout << typeid(Ax).name() << std::endl;

    my_type det = Ax * By - Ay * Bx;
    if (det == 0) {
      continue;
    }
    my_type detx = (x_new) * By - (y_new) * Bx; //520,000,000,966,524
    my_type cramer_x = -1;      //cramers sats för lösning av ekvation. 
    if (detx % det == 0) {    //ej heltals lösning -> inget resultat.
      cramer_x = detx / det;
    }

    my_type dety = Ax * (y_new) - Ay * (x_new);
    my_type cramer_y = -1;
    if (dety % det == 0) { 
      cramer_y = dety / det;
    }
    
    if (cramer_y >= 0 && cramer_x >= 0) {
      my_type cost = 3 * cramer_x + cramer_y;
      total_cost += cost;
    }
    else {
      bool x = true;
    }
  }

  std::cout << total_cost << std::endl;
  return 0;
}