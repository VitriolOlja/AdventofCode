#include "Entity.h"

int Entity::getX() const {return this->boundingBox.m_x;}
int Entity::getY() const {return this->boundingBox.m_y;}
int Entity::getWidth() const {return this->boundingBox.height;}
int Entity::getHeight() const {return this->boundingBox.width;}

void Entity::move(int dx, int dy){
  this->boundingBox.m_x += dx;
  this->boundingBox.m_y += dx;
}

BoundingBox Entity::getBoundingBox() const {return this->boundingBox;}
