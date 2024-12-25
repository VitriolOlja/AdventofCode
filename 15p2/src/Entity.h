#ifndef ENTITY_H
#define ENTITY_H

#include "Direction.h"

class Entity {
  private:
    int m_x;
    int m_y;
    int width;
    int height;

  protected:
    Entity(int x, int y, int w, int h) : 
      m_x{x}, m_y{y}, width{w}, height{h} {}

  public:
    virtual const char* getSymbol() = 0;

    //virtual Entity& move(DIRECTION direction) = 0;

    int getX() const {return this->m_x;}
    int getY() const {return this->m_y;}
    int getWidth() const {return this->width;}
    int getHeight() const {return this->height;}
};

#endif