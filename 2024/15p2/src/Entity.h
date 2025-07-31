#ifndef ENTITY_H
#define ENTITY_H

#include "Direction.h"
#include "BoundingBox.h"

/**
 * 
 */
class Entity {
  private:
    BoundingBox boundingBox;

  protected:
    Entity(int x, int y, int w, int h) : 
      boundingBox{x, y, w, h} {}

  public:
    virtual const char* getSymbol();

    //virtual Entity& move(DIRECTION direction) = 0;

    int getX() const;
    int getY() const;
    int getWidth() const;
    int getHeight() const;
    void move(int dx, int dy);
    BoundingBox getBoundingBox() const;
};

#endif