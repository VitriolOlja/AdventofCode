#include <iostream>
#include <fstream>
#include <vector>



void getRow(const std::string &line, std::vector<char> &row){
  for (const char& c : line) {
    row.push_back(c);
  }
}



int findXmas(const std::vector<std::vector<char>> &matrix) {

  int xmas = 0;
  
  for (int row = 0; row < matrix.size(); row++) {
    for (int column = 0; column < matrix[0].size(); column++) {
      
      if (matrix[row][column] == 'X') {
        //check north
        if (row - 3 >= 0) {
          //check if there is one up.
          if(matrix[row - 1][column] == 'M' && matrix[row - 2][column] == 'A' && matrix[row - 3][column] == 'S') {
            //found xmas vertical
            xmas++;
          }
          //check northwest
          if (column - 3 >= 0) {
            if(matrix[row - 1][column - 1] == 'M' && matrix[row - 2][column - 2] == 'A' && matrix[row - 3][column - 3] == 'S') {
              //found xmas vertical
              xmas++;
            }
          }
          //check northeast
          if (column + 3 < matrix[0].size()) {
            if(matrix[row - 1][column + 1] == 'M' && matrix[row - 2][column + 2] == 'A' && matrix[row - 3][column + 3] == 'S') {
              //found xmas vertical
              xmas++;
            }
          }

        }

        //check west
        if (column - 3 >= 0) {
          if(matrix[row][column - 1] == 'M' && matrix[row][column - 2] == 'A' && matrix[row][column - 3] == 'S') {
              //found xmas west
              xmas++;
          }
        }
        //check east
        if (column + 3 < matrix[0].size()) {
          if(matrix[row][column + 1] == 'M' && matrix[row][column + 2] == 'A' && matrix[row][column + 3] == 'S') {
              //found xmas east
              xmas++;
          }
        }

        //check south
        if (row + 3 < matrix.size()) {
          if(matrix[row + 1][column] == 'M' && matrix[row + 2][column] == 'A' && matrix[row + 3][column] == 'S') {
              //found xmas south
              xmas++;
          }

          //check southwest
          if (column - 3 >= 0) {
            if(matrix[row + 1][column - 1] == 'M' && matrix[row + 2][column - 2] == 'A' && matrix[row + 3][column - 3] == 'S') {
              //found xmas vertical
              xmas++;
            }
          }
          //check southeast
          if (column + 3 < matrix[0].size()) {
            if(matrix[row + 1][column + 1] == 'M' && matrix[row + 2][column + 2] == 'A' && matrix[row + 3][column + 3] == 'S') {
              //found xmas vertical
              xmas++;
            }
          }
        }

      }
    }
  }


  return xmas;
}

//part two
int x_mas(const std::vector<std::vector<char>> &matrix) {
  int x_mas = 0;


  for (int row = 1; row < matrix.size() - 1; row++) {
    for (int column = 1; column < matrix[0].size() - 1; column++) {
        if (matrix[row][column] == 'A') {
          bool nw_se = false; //north west to south east.

          if ((matrix[row - 1][column - 1] == 'M' && matrix[row + 1][column + 1] == 'S')
            || (matrix[row - 1][column - 1] == 'S' && matrix[row + 1][column + 1] == 'M')) {
              nw_se = true;
          }

          bool ne_sw = false; //north east to south west
          if ((matrix[row - 1][column + 1] == 'M' && matrix[row + 1][column - 1] == 'S')
            || (matrix[row - 1][column + 1] == 'S' && matrix[row + 1][column - 1] == 'M')) {
              ne_sw = true;
          }

          if (nw_se && ne_sw) {
            x_mas++;
          }
      }
    }
  }




  return x_mas;
}

int main() {

  std::ifstream input("C:\\Users\\Dwight\\Desktop\\c_dev\\AoC\\4\\build\\input.txt");



  std::string line;

  std::vector<std::vector<char>> letters;



  while(std::getline(input, line)) {
    std::vector<char> row;

    getRow(line, row);

    letters.push_back(row);
  }

  std::cout << x_mas(letters) << std::endl;


  

  return 0;
}