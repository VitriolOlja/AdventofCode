#include "Wall.h"


Wall::Wall(int x, int y) : Entity(x, y, 2, 1) {
  
}

//override
const char* Wall::getSymbol() {
  return "##";
}