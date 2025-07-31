#ifndef ROBOT_H
#define ROBOT_H

#include <string>

class Robot {
  private:
    //position
    long m_x;
    long m_y;
    
    //velocity
    int m_vx;
    int m_vy;

  public:
    // Constructor
    Robot(int x, int y, int vx, int vy);

    // Destructor (if necessary)
    //~Robot();

    // Method to update the robot's state
    void update(const int delta_time, int total_width, int total_height);

    // Optional: Getters or Setters for position and velocity if needed
    int getX() const;
    int getY() const;
    int getVX() const;
    int getVY() const;

    std::string get_display() const;


    int getPeriodicity(int total_width, int total_height);
};

#endif