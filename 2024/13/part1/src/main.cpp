#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <array>


/* int gcd(const int& a, const int& b) {
  int mod = a % b;
  if (mod == 0) {
    return b;
  }
  return gcd(b, mod);
} */

/* ax + by = res
std::pair<int, int> diophantine(const int &a, const int &b, const int& res) {

} */

int main() {

  std::string line;
  std::ifstream input("C:/Users/Dwight/Desktop/c_dev/AoC/13/build/input.txt");

  //3 tokens to press A, and 1 token to press B

  std::vector<std::array<std::array<int, 2>, 3>> machines;
  int i = 0;
  std::array<std::array<int, 2>, 3> machine;

  while(std::getline(input, line)) {
    if (line.empty()) { //we could also have every fourth..
      machines.push_back(machine);
      i = 0;
      continue;
    }
    //magic math
    int first = line.find_first_of('X') + 2;
    int comma = line.find_first_of(',');
    machine[i][0] = std::stoi(line.substr(first, comma - first));
    machine[i][1] = std::stoi(line.substr(comma + 4, line.size() - comma - 4));
    i++;
  }

  long long total_cost = 0;

  std::cout << "\n\n";
  for (const auto &[A, B, point] : machines) {
    const auto &[Ax, Ay] = A; // Unpack A
    const auto &[Bx, By] = B; // Unpack B
    const auto &[x, y] = point; // Unpack point

    int det = Ax * By - Ay * Bx;
    int detx = x * By - y * Bx;
    int cramer_x = -1;
    if (detx % det == 0) {  //if a even solution.
      cramer_x = detx / det;
    }

    int dety = Ax * y - Ay * x;
    int cramer_y = -1;
    if (dety % det == 0) {  //if a even solution.
      cramer_y = dety / det;
    }
    
    if (cramer_y != -1 && cramer_x != -1) {
      total_cost += (3 * cramer_x + cramer_y);
    }
  }

  std::cout << total_cost << std::endl;


/*   std::cout << std::endl;std::cout << std::endl;
  for (const auto &[A, B, point] : machines) {
    const auto &[Ax, Ay] = A; // Unpack A
    const auto &[Bx, By] = B; // Unpack B
    const auto &[x, y] = point; // Unpack point

    int first = Bx < Ax ? gcd(Ax, Bx) : gcd(Bx, Ax);
    int second = By < Ay ? gcd(Ay, By) : gcd(By, Ay);

    std::cout << first << "    " <<  second << "\n";
  }
  std::cout << std::endl; */




  return 0;
}