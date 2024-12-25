#include "Box.h"


//magically coded 2 as width with the [] is maybe dumb.
Box::Box(int x, int y) : Entity(x,y,2,1) {

}

const char* Box::getSymbol() {
  return "[]";
}