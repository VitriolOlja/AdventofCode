/**
 * 16777216 = 2^24
 */

#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>

constexpr uint32_t getNext(uint32_t secret) {
  constexpr unsigned int mask = (1u << 24) - 1;
  //multiply by 64 2^6
  uint64_t res = secret << 6; 
  secret = secret ^ res;  //mix
  secret &= mask;

  res = secret >> 5;
  secret = secret ^ res;  //mix
  secret &= mask;

  res = secret << 11; 
  secret = secret ^ res;  //mix
  secret &= mask;

  return secret;
}

constexpr uint32_t tt(uint32_t secret){
  for ( int i = 0; i < 2000; i++ ) {
    secret = getNext(secret);
  }
  return secret;
} 


int main() {
  std::string line;
  std::ifstream input("input.txt");

  size_t sum = 0;
  while (std::getline(input, line)) {
    unsigned long val = std::stoul(line);
    uint32_t num = static_cast<uint32_t>(val);

    uint32_t res = tt(num);
    std::cout << res << std::endl;
    sum += res;
  }

  std::cout << sum << std::endl;

  return 0;
}