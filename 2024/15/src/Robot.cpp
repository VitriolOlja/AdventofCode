#include <iostream>
#include "Robot.h"


void increment_v(int &vx, int &vy, const DIRECTION &dir) {
  switch (dir) {
      case DIRECTION::UP:
        vy += -1;
        break;
      case DIRECTION::DOWN:
        vy += 1;
        break;
      case DIRECTION::LEFT:
        vx += -1;
        break;
      case DIRECTION::RIGHT:
        vx += 1;
        break;
    }
}


// Update method definition
void Robot::move(DIRECTION direction, std::vector<std::vector<ENTITY>> &robot_matrix) {
  int vx = 0;
  int vy = 0;

  increment_v(vx, vy, direction);

  //robot_matrix[row][column]
  //moveuntil not on a box. 
  
  while(robot_matrix[m_y + vy][m_x + vx] == ENTITY::BOX) {
    increment_v(vx, vy, direction);
  }

  std::cout << m_x << ", "<<  m_y << std::endl;
  std::cout << vx << ", " <<  vy << std::endl;
  //if we reach a wall. do nothing
  //if we reach an empty slot. Switch that slot. @OOO.#
  //otherwise do nothing just return.
  

  if (robot_matrix[m_y + vy][m_x + vx] == ENTITY::EMPTY) {
    robot_matrix[m_y][m_x] = ENTITY::EMPTY;
    robot_matrix[m_y + vy][m_x + vx] = ENTITY::BOX;

    vx = (vx > 0) - (vx < 0);
    vy = (vy > 0) - (vy < 0);

    robot_matrix[m_y + vy][m_x + vx] = ENTITY::ROBOT;
    m_x += vx;
    m_y += vy;
  }
}

// Getter method definitions
int Robot::getX() const { return m_x; }
int Robot::getY() const { return m_y; }

void Robot::setX(int x) {
  this->m_x = x;

}

void Robot::setY(int y) {
  this->m_y = y;
}