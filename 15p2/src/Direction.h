#ifndef DIRECTION_H
#define DIRECTION_H
#include <utility> 
/**
 * Represents different directions.
 */
enum class DIRECTION {
    LEFT,
    RIGHT,
    UP,
    DOWN
};

std::pair<int, int> dirVec(const DIRECTION &dir) {
    switch (dir) {
        case DIRECTION::LEFT:
            return {-1, 0};
        case DIRECTION::RIGHT:
            return {1, 0}; 
        case DIRECTION::UP:
            return {0, -1};
        case DIRECTION::DOWN:
            return {0, 1};
    }
}

#endif