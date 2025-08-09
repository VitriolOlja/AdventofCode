#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>

// Euclidean algorithm for GCD
uint64_t gcd(uint64_t a, uint64_t b) {
    while (b != 0) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// LCM using GCD
uint64_t lcm(uint64_t a, uint64_t b) {
    return (a / gcd(a, b)) * b;
}

uint64_t lcm_array(const uint64_t *arr, size_t n) {
    uint64_t result = arr[0];
    for (size_t i = 1; i < n; i++) {
        result = lcm(result, arr[i]);
    }
    return result;
}


int main(){
  
  const uint64_t cycles[6] = {22199 ,13207, 16579, 18827, 17141, 14893};
  printf("%llu\n", lcm_array(cycles, 6));

  return 0;
}