#include <iostream>
#include <vector>



std::vector<int> program (long long A) {

  std::vector<int> ret;

  do {
    int B = A % 8;
    B = B ^ 5;
    long long C = A >> B;
    B = B ^ 6;
    A = A >> 3;

    B = B ^ C;
    
    ret.push_back(B % 8);

    


  } while (A > 0);

  return ret;
}


int main(int argc, char* argv[]) {
  if (argc != 2) { // Ensure exactly one argument is passed
      std::cerr << "Usage: " << argv[0] << " <number>" << std::endl;
      return 1;
  }

  // Convert the argument to a long long integer
  long long input = std::strtoll(argv[1], nullptr, 10);

  // Call the program function
  std::vector<int> result = program(input);

  // Print the result
  for (const int& a : result) {
      std::cout << a << " ";
  }
  std::cout << std::endl;



  return 0;

}