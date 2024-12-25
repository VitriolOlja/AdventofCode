#include <iostream>
#include <fstream>
#include <vector>
#include "Direction.h"
#include "Entity.h"
#include "Robot.h"



void display(std::vector<std::vector<ENTITY>> &robot_matrix) {
  for (int row = 0; row < robot_matrix.size(); row++) {
    for (int column = 0; column < robot_matrix[0].size(); column++) {
      
      if (robot_matrix[row][column] == ENTITY::EMPTY) {
        std::cout << ".";
      }
      else if (robot_matrix[row][column] == ENTITY::WALL) {
        std::cout << "#";
      } 
      else if (robot_matrix[row][column] == ENTITY::BOX){
        std::cout << "0";
      }
      else if (robot_matrix[row][column] == ENTITY::LEFTBOX){
        std::cout << "[";
      }
      else if (robot_matrix[row][column] == ENTITY::RIGHTBOX){
        std::cout << "]";
      }
      else if (robot_matrix[row][column] == ENTITY::ROBOT) {
        std::cout << "@";
      }
    }
    std::cout << std::endl;
  }
}


int calculate_gps(std::vector<std::vector<ENTITY>> &robot_matrix) {
  int gps = 0;
  for (int row = 0; row < robot_matrix.size(); row++) {
    for (int column = 0; column < robot_matrix[0].size(); column++) {
      if (robot_matrix[row][column] == ENTITY::BOX) {
        gps += row * 100 + column;
      }
    }
  }

  return gps;
}

std::vector<std::vector<ENTITY>> resize(std::vector<std::vector<ENTITY>> &robot_matrix) {
  std::vector<std::vector<ENTITY>> rez;
  for (int row = 0; row < robot_matrix.size(); row++) {
    std::vector<ENTITY> row_vector;
    for (int column = 0; column < robot_matrix[0].size(); column++) {
      if (robot_matrix[row][column] == ENTITY::EMPTY) {
        row_vector.push_back(ENTITY::EMPTY);
        row_vector.push_back(ENTITY::EMPTY);
      }
      else if (robot_matrix[row][column] == ENTITY::WALL) {
        row_vector.push_back(ENTITY::WALL);
        row_vector.push_back(ENTITY::WALL);
      } 
      else if (robot_matrix[row][column] == ENTITY::BOX){
        row_vector.push_back(ENTITY::LEFTBOX);
        row_vector.push_back(ENTITY::RIGHTBOX);
      }
      else if (robot_matrix[row][column] == ENTITY::ROBOT) {
        row_vector.push_back(ENTITY::ROBOT);
        row_vector.push_back(ENTITY::EMPTY);
      }
    }
    rez.push_back(row_vector);
  }
  return rez;
}






//#define INPUT "super_small.txt"
#define INPUT "super_small.txt"

int main()
{
  std::ifstream inputFile(INPUT);

  if (!inputFile)
  {
    std::cerr << "Error: could not open file!" << std::endl;
    return 1;
  }

  std::string line;
  std::getline(inputFile,line);
  int total_width = line.find_last_of('#') - line.find_first_of('#') + 1;

  std::vector<ENTITY> first_row;

  for (int i = 0; i < total_width; i++) {
    first_row.push_back(ENTITY::WALL);
  }
  std::vector<std::vector<ENTITY>> matrix;
  matrix.reserve(total_width); //really high
  matrix.push_back(first_row);
  Robot robot;


  int row = 1;
  while (std::getline(inputFile, line) && (line.find_first_of('#') == 0))
  {
    //its a part of the map.
    std::vector<ENTITY> row_vector;
    row_vector.reserve(total_width);
    for (int i = 0; i < total_width; i++) {
      if (line[i] == '.'){
        row_vector.push_back(ENTITY::EMPTY);
      }
      if (line[i] == '#') {
        row_vector.push_back(ENTITY::WALL);
      } 
      else if (line[i] == 'O'){
        row_vector.push_back(ENTITY::BOX);
      }
      else if (line[i] == '@') {
        robot.setX(i);
        robot.setY(row);
        row_vector.push_back(ENTITY::ROBOT);
      }
    }
    matrix.push_back(row_vector);
    row += 1;
  }

  //get instructions from rest of file.
  std::vector<DIRECTION> instructions;
  while(std::getline(inputFile, line)) {
    for (const char &c : line) {
      if (c == '<') {
        instructions.push_back(DIRECTION::LEFT);
      }
      else if (c == '>') {
        instructions.push_back(DIRECTION::RIGHT);
      }
      else if (c == '^') {
        instructions.push_back(DIRECTION::UP);
      }
      else if (c == 'v') {
        instructions.push_back(DIRECTION::DOWN);
      }
    }
  }

  inputFile.close();

  /* for (DIRECTION &instruction : instructions) {
    robot.move(instruction, matrix);
  } */
  
  display(matrix);

  std::vector<std::vector<ENTITY>> rez = resize(matrix);

  display(rez);
  

  return 0;
}
