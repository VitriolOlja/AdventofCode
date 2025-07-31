#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

#include "Direction.h"
#include "Entity.h"
#include "Robot.h"
#include "Wall.h"
#include "Box.h"



void display(
    int total_width,
    int total_height,
    std::unique_ptr<Robot> &robot,
    std::vector<std::unique_ptr<Entity>> &entities
) {

  char pixels[total_height][total_width] = {};

  pixels[robot->getY()][robot->getX()] = robot->getSymbol()[0];

  for (auto& e : entities) {
    const char* symbols = e->getSymbol();
    for (int i = 0; i < e->getWidth(); i++) {
      pixels[e->getY()][e->getX() + i] = symbols[i];
    }
  }

  for (int row = 0; row < total_height; row++) {
    for (int column = 0; column < total_width; column++) {
      if (pixels[row][column] == '\0') {
        std::cout << ".";
      }
      else {
        std::cout << pixels[row][column];
      }
    }
    std::cout << "\n";
  }
  
  std::cout << std::flush;
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

  
  //std::getline(inputFile,line);
  //int total_width = line.find_last_of('#') - line.find_first_of('#') + 1;

  //Robot robot;
  std::vector<std::unique_ptr<Entity>> entities;
  std::unique_ptr<Robot> robot; 


  std::string line;
  int row = 0;
  int scale_factor = 2;
  int total_width = 0;  
  while (std::getline(inputFile, line) && (line.find_first_of('#') == 0))
  {

    total_width = std::max(total_width, static_cast<int>(line.find_last_of('#') + 1));

    for (int column = 0; column < line.find_last_of('#') + 1; column++) {
      if (line[column] == '#') {
        entities.push_back(std::make_unique<Wall>(column * scale_factor, row));
      } else if (line[column] == 'O') {
        entities.push_back(std::make_unique<Box>(column * scale_factor, row));
      } else if(line[column] == '@') {
        robot = std::make_unique<Robot>(column * scale_factor, row);
      }
    }

    row++;
  }

  int total_height = row;
  total_width *= scale_factor;

  display(total_width, total_height, robot, entities);

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

  for (const DIRECTION &dir : instructions) {
    if (Robot.can_move(dir, entities) {
      std::cout << "yes i can moveeee" std::endl;
    }
  }

  inputFile.close();

  return 0;
}
