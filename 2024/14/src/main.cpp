#include <iostream>
#include <fstream>
#include <vector>
#include <numeric> 
#include <chrono>
#include <thread>


#include "robot.h"

//for real
#define total_width 101
#define total_height 103


//for testing
//#define total_width 11
//#define total_height 7

#define INPUT "input.txt"

std::vector<std::vector<int>> getRobotMatrix(const std::vector<Robot> &robots) {
  //int robot_matrix[total_height][total_width] = {};
  std::vector<std::vector<int>> robot_matrix(total_height, std::vector<int>(total_width, 0));

  for (const Robot &robot : robots) {
    robot_matrix[robot.getY()][robot.getX()] = 1;
  }

  return robot_matrix;
}


void display(std::vector<std::vector<int>> robot_matrix) {
  

  for (int row = 0; row < robot_matrix.size(); row++) {
    for (int column = 0; column < robot_matrix[0].size(); column++) {
      
      if (robot_matrix[row][column] == 0) {
        std::cout << ". ";
      }
      else {
        std::cout << robot_matrix[row][column] << " ";
      }
    }
    std::cout << "\n";
  }
}

bool check_symmetry(std::vector<std::vector<int>> robot_matrix){
  bool straight_line = false;

  for (int row = 0; row < robot_matrix.size() - 4; row++) {
    for (int column = 0; column < (robot_matrix[0].size()); column++) {
      if ((robot_matrix[row][column] & robot_matrix[row + 1][column] & robot_matrix[row + 2][column] & robot_matrix[row + 3][column]) > 0) {
        straight_line = true;
        break;
      }
    }
  }

  //check for edge
  // 00100
  // 01110
  // 11111

  bool edge = false;

  for (int row = 0; row < robot_matrix.size() - 3; row++) {
    for (int column = 0; column < (robot_matrix[0].size() - 5); column++) {
      if ((robot_matrix[row + 2][column] & 
      robot_matrix[row + 2][column + 1] & 
      robot_matrix[row + 2][column + 2] & 
      robot_matrix[row + 2 ][column + 3] & 
      robot_matrix[row + 2 ][column + 4] &

      robot_matrix[row + 1][column + 1] &
      robot_matrix[row + 1][column + 2] & 
      robot_matrix[row + 1][column + 3] & 

      robot_matrix[row][column + 2]) > 0) {
        edge = true;
        break;
      }
    }
  }

  return straight_line && edge;
}


int calculate_safety_factor(const std::vector<Robot> &robots) {
  int q_width = (total_width - 1) / 2;
  int q_height = (total_height - 1) / 2;
  int quadrants[4] = {};

  for (const Robot &robot : robots) {
    
    //which quadrant do i belong to? 
    int x = robot.getX();
    int y = robot.getY();

    if (x < q_width) {
      if (y < q_height) {
        quadrants[0]++;
      } else if (y > q_height){
        quadrants[2]++;
      }
    } else if ( x > q_width) {
      if (y < q_height) {
        quadrants[1]++;
      } else if (y > q_height){
        quadrants[3]++;
      }
    }
  }

  int safety_factor = 1;
  for (const int &q : quadrants) {
    std::cout << q << "\t";
    safety_factor *= q;
  }

  std::cout << "safet factor   " << safety_factor << "\n";
  return safety_factor;
}



// ProgrammerHorror worthy
int main()
{
  std::ifstream inputFile(INPUT);

  if (!inputFile)
  {
    std::cerr << "Error: could not open file!" << std::endl;
    return 1;
  }

  std::vector<Robot> robots;

  std::string line;
  while (std::getline(inputFile, line))
  {
    int p_index = line.find("p");
    int v_index = line.find("v");

    // std::cout << "Position: " << line.substr(p + 2, v - p - 2) << " Velocity: " <<  line.substr(v + 2, line.size() - 2 - v) << std::endl;

    std::string position = line.substr(p_index + 2, v_index - p_index - 2);
    std::string velocity = line.substr(v_index + 2, line.size() - 2 - v_index);

    int p_comma = position.find(',');
    int v_comma = velocity.find(',');

    /* std::cout << "x: " << position.substr(0, p_comma) << 
    " y: " << position.substr(p_comma + 1, position.size() - p_comma) <<
    " vx: " << velocity.substr(0, v_comma) <<
    " vy: " << velocity.substr(v_comma + 1, velocity.size() - v_comma) << std::endl; */

    int x, y, vx, vy;
    try {
      x = std::stoi(position.substr(0, p_comma));
      y = std::stoi(position.substr(p_comma + 1, position.size() - p_comma));
      vx = std::stoi(velocity.substr(0, v_comma));
      vy =  std::stoi(velocity.substr(v_comma + 1, velocity.size() - v_comma));
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Invalid argument: " << e.what() << std::endl;
    }
    catch (const std::out_of_range& e) {
        std::cout << "Out of range: " << e.what() << std::endl;
    }

    robots.push_back(Robot(x, y, vx, vy));
  }

  //std::cout << "Constructed robots" << std::endl


  //std::cout << "Inital position: \n";

  //display(getRobotMatrix(robots));

  //calculate_safety_factor(robots);


  long total_seconds = 0;
  while (true){
    for (Robot &robot : robots) {
      robot.update(1, total_width, total_height);
    }
    total_seconds++;

    std::vector<std::vector<int>> matrix = getRobotMatrix(robots);

    if (check_symmetry(matrix)) {

      std::cout << "something symmetrical after " << total_seconds << " seconds";
      display(matrix);
      std::this_thread::sleep_for(std::chrono::seconds(2)); // Sleep for 2 seconds
    }
  } 


  

  //display(robots);
  //calculate_safety_factor(robots);



  inputFile.close();

  return 0;
}
