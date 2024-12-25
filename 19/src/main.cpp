
#include <iostream>
#include <cmath>
#include <vector>


std::vector<int> get_primes(int upper_bound) {
  std::vector<int> primes;
  for (int i = 2; i < upper_bound; i++) {
    bool is_prime = true;
    for (const int& prime : primes) {
      if (i % prime == 0) {
        is_prime = false;
        break;
      }
    }
    if (is_prime) {
      primes.push_back(i);
    }
  }

  return primes;
}

int main(int argc, char* argv[])
{

  if (argc != 2) { // Ensure exactly one argument is passed
      std::cerr << "Usage: " << argv[0] << " <number>" << std::endl;
      return 1;
  }

  // Convert the argument to a long integer
  long input = std::strtol(argv[1], nullptr, 10);


  std::vector<int> primes = get_primes(input);

  for (const int &p : primes) {
    std::cout << p << " ";
  }

  std::cout << std::endl;

  return 0;
}