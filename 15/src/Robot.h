#ifndef ROBOT_H
#define ROBOT_H

#include <vector>
#include "Direction.h"
#include "Entity.h"


class Robot {
  private:
  int m_x;
  int m_y;


  public:
    //Robot(int x, int y);

    void move(DIRECTION direction, std::vector<std::vector<ENTITY>> &robot_matrix);

    
    // Getter method definitions
    int getX() const;
    int getY() const;
    void setX(int x);
    void setY(int y);
};



#endif