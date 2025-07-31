
#include <iostream>
#include <cmath>
#include <vector>

int combo_operand(int combo,unsigned long long  *A,unsigned long long  *B, unsigned long long  *C)
{
  if (combo >= 0 && combo <= 3)
  {
    return combo;
  }
  else if (combo == 4)
  {
    return *A;
  }
  else if (combo == 5)
  {
    return *B;
  }
  else if (combo == 6)
  {
    return *C;
  }

  // not valid...
  return -1;
}

int op_adv(unsigned long long  *A,unsigned long long  *B,unsigned long long  *C, int *instruction_ptr, int *program)
{
  int combo = combo_operand(program[*instruction_ptr + 1], A, B, C);

  // std::cout << "combo: " << combo << std::endl;

  if (combo == -1)
  {
    *instruction_ptr = *instruction_ptr + 2;
  }

  *A = (*A) >> combo;

  *instruction_ptr += 2;

  return -1;
}

int op_bxl(unsigned long long  *A,unsigned long long  *B,unsigned long long  *C, int *instruction_ptr, int *program)
{
  /* The bxl instruction (opcode 1)
   *  calculates the bitwise XOR of register B and the instruction's literal operand,
   *  then stores the result in register B.
   */

  *B = (*B) ^ program[(*instruction_ptr) + 1];
  *instruction_ptr += 2;

  return -1;
}

int op_bst(unsigned long long  *A,unsigned  long long  *B,unsigned long long  *C, int *instruction_ptr, int *program)
{
  *B = combo_operand(program[(*instruction_ptr) + 1], A, B, C) & 7;
  *instruction_ptr += 2;

  return -1;
}

int op_jnz(unsigned long long  *A,unsigned long long  *B,unsigned long long  *C, int *instruction_ptr, int *program)
{
  if (*A == 0)
  {

    *instruction_ptr += 2;
    return -1;
  }

  *instruction_ptr = program[(*instruction_ptr) + 1];

  return -1;
}

int op_bxc(unsigned long long  *A,unsigned long long  *B,unsigned long long  *C, int *instruction_ptr, int *program)
{

  *B = (*B) ^ (*C);

  *instruction_ptr += 2;

  return -1;
}

int op_out(unsigned long long  *A,unsigned long long  *B,unsigned long long  *C, int *instruction_ptr, int *program)
{

  int combo = combo_operand(program[(*instruction_ptr) + 1], A, B, C) & 7;

  *instruction_ptr += 2;

  return combo;
}

int op_bdv(unsigned long long  *A,unsigned long long  *B,unsigned long long  *C, int *instruction_ptr, int *program)
{
  int combo = combo_operand(program[(*instruction_ptr) + 1], A, B, C);

  // std::cout << "combo: " << combo << std::endl;

  if (combo == -1)
  {
    *instruction_ptr = (*instruction_ptr) + 2;
  }

  *B = (*A) >> combo;

  (*instruction_ptr) += 2;

  return -1;
}

int op_cdv(unsigned long long *A, unsigned long long  *B,unsigned long long  *C, int *instruction_ptr, int *program)
{
  int combo = combo_operand(program[(*instruction_ptr) + 1], A, B, C);

  // std::cout << "combo: " << combo << std::endl;

  if (combo == -1)
  {
    *instruction_ptr = *instruction_ptr + 2;
  }

  *C = *A >> combo;

  *instruction_ptr += 2;

  return -1;
}

int main(int argc, char* argv[])
{

  if (argc != 2) { // Ensure exactly one argument is passed
      std::cerr << "Usage: " << argv[0] << " <number>" << std::endl;
      return 1;
  }

  // Convert the argument to a long long integer
  long long input = std::strtoll(argv[1], nullptr, 10);


  // list of function ptrs.
  // for opcode 0->7 send in opcode instructions[opcode](A,B,C,instruction_ptr)
  int (*instructions[])(unsigned long long  *,unsigned long long  *,unsigned long long  *, int *, int *) = {op_adv, op_bxl, op_bst, op_jnz, op_bxc, op_out, op_bdv, op_cdv};



  
    // Registers.
    unsigned long long A = input;
    unsigned long long  B = 0;
    unsigned long long  C = 0;

    int instruction_ptr = 0; // where we are right now.

    int program[] = {2,4,1,5,7,5,1,6,0,3,4,0,5,5,3,0};

    std::vector<int> correct_values;

    while (instruction_ptr < sizeof(program) / sizeof(program[0]) && instruction_ptr >= 0)
    {
      int ret = instructions[program[instruction_ptr]](&A, &B, &C, &instruction_ptr, program);

      if (ret != -1)
      {
        correct_values.push_back(ret);
      }
    }

    std::cout << "with A  = " << input << " gives : ";
    // loop through A til next -> bruteforce!!!
    for (int &p : correct_values) {
      std::cout << p << " ";
    }

    std::cout << std::endl;
  
  


  return 0;
}
