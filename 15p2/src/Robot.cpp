#include "Robot.h"


Robot::Robot(int x, int y) : Entity(x, y, 1, 1) {
  
}


//override
const char* Robot::getSymbol() {
  return "@";
}


bool Robot::can_move(const DIRECTION &direction, std::vector<std::unique_ptr<Entity>> &others) {
  std::pair<int, int> direction_vector = dirVec(direction);
  
  this->move(direction_vector.first, direction_vector.second);
  
  //move the robot -> see what happens

  std::vector<std::unique_ptr<Entity>&> all_collisions; 
  std::vector<std::unique_ptr<Entity>&> collisions;
  for (auto &o : others ) {
    
    
    if(collidesWith(this->getBoundingBox(), o->getBoundingBox())){
      //we found a collision
      collisions.push_back(o);
    }
  }


  
}
