#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

int func(const std::string &line)
{
  static bool doo = true;
  int sum = 0;

  int docount = 0;
  int dontcount = 0;

  int mulcount = 0;
  for (int i = 0; i < line.size(); i++)
  {
    if ((i + 3) < line.size() && line[i] == 'd' && line[i + 1] == 'o' && line[i + 2] == '(' && line[i + 3] == ')')
    {
      doo = true;
      docount += 1;
      i +=3;
    }
    else if ((i + 6) < line.size() && line[i] == 'd' && line[i + 1] == 'o' && line[i + 2] == 'n'  && line[i + 4] == 't' && line[i + 5] == '(' && line[i + 6] == ')')
    {
      doo = false;
      dontcount += 1;
      i += 6;
    }
    else if ((i + 3) < line.size() && line[i] == 'm' && line[i + 1] == 'u' && line[i + 2] == 'l' && line[i + 3] == '(')
    {
      // we found mul(

      // Start parsing the 'mul' values
      int d = i + 4; // Position after 'mul('
      int first = 0, second = 0;
      bool correct = true;

      // Parse first number (until comma)
      while (line[d] != ',')
      {
        if (!std::isdigit(line[d]))
        {
          correct = false;
          break;
        }
        first = first * 10 + (line[d] - '0');
        d++;
      }

      if (!correct){
        continue;
      }



      d += 1;
      while (line[d] != ')')
      {
        if (!std::isdigit(line[d]))
        {
          correct = false;
          break;
        }
        second = second * 10 + (line[d] - '0');
        d++;
      }

      if (!correct){
        continue;
      }
        

      if (first == 917 && second == 130){
        std::cout << "here we are : 9" << std::endl;
      }

      if (doo)
      {
        sum += first * second;
      }

      mulcount += 1;
    }
  }

  std::cout << "do's : " << docount << " don't's : " << dontcount << "   mulcount " << mulcount << std::endl;

  return sum;
}

int main()
{
  std::ifstream input("C:\\Users\\Dwight\\Desktop\\c_dev\\AoC\\3\\src\\input.txt");

 int sum = 0;
/*
     std::string test = "xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))";

    std::cout << func(test) << std::endl;

    return 0;  
 */

  std::string line;
  while (std::getline(input, line))
  {
    sum += func(line);
  }

  std::cout << sum << std::endl;
  return 0;
}