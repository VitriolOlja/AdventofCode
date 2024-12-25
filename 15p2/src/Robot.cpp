#include "Robot.h"



Robot::Robot(int x, int y) : Entity(x, y, 1, 1) {
  
}


//override
const char* Robot::getSymbol() {
  return "@";
}

