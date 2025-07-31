#include <iostream>
#include <sstream>
#include "Robot.h"
#include <numeric>


// Constructor definition
Robot::Robot(int x, int y, int vx, int vy) 
    : m_x{x}, m_y{y}, m_vx{vx}, m_vy{vy} {
    //std::cout << "Constructed robot with position (" << m_x << "," << m_y << ") and velocity ("
    //          << m_vx << "," << m_vy << "). " << std::endl;
}




// Update method definition
void Robot::update(int delta_time, int total_width, int total_height) {
    m_x += m_vx * delta_time;
    m_y += m_vy * delta_time;

    //teleport if necessary.
    //apparently modulo does not work like this????
    //m_x %= total_width;
    //m_y %= total_height;

    while (m_x < 0) {
      m_x += total_width;
    }
    while (m_x >= total_width) {
      m_x -= total_width;
    }
    while (m_y < 0) {
      m_y += total_height;
    }
    while (m_y >= total_height) {
      m_y -= total_height;
    }
}


// Getter method definitions
int Robot::getX() const { return m_x; }
int Robot::getY() const { return m_y; }
int Robot::getVX() const { return m_vx; }
int Robot::getVY() const { return m_vy; }

// Get the current state of the robot as a string
std::string Robot::get_display() const {
    std::ostringstream oss;  // Create an output string stream
    oss << "Current position: (" << m_x << ", " << m_y << ") and velocity: ("
        << m_vx << ", " << m_vy << ")";
    return oss.str();  // Return the string representation
}
