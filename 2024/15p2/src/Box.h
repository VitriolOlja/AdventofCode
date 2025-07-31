#ifndef BOX_H
#define BOX_H

#include "Entity.h"

class Box : public Entity {
public:
  Box(int x, int y);
  /**
   * this returns two symbols. 
   * one left and one right.
   * maybe not the best way t handle it.
   */
  const char* getSymbol() override;
};



#endif