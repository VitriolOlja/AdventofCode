#ifndef ROBOT_H
#define ROBOT_H

#include "Entity.h"
#include <vector>
#include <memory>
#include "Direction.h"
#include "BoundingBox.h"

class Robot : public Entity {

public:
  Robot(int x, int y);
  const char* getSymbol() override;

  bool can_move(const DIRECTION &direction, std::vector<std::unique_ptr<Entity>> &others);
};



#endif