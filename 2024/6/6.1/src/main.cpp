#include <vector>
#include <iostream>
#include <fstream>

int main()
{

  std::ifstream input("C:\\Users\\Dwight\\Desktop\\c_dev\\AoC\\6\\6.1\\build\\input.txt");

  std::string line;
  int guard_x;
  int guard_y;
  int vel_x;
  int vel_y;

  std::vector<std::vector<char>> map;
  int row = 0;

  std::vector<std::pair<int, int>> obstacles;

  while (std::getline(input, line))
  {
    std::vector<char> row_vector;
    for (int i = 0; i < line.size(); i++)
    {
      if (line[i] == '^' || line[i] == 'v' || line[i] == '<' || line[i] == '>')
      {
        guard_x = i;
        guard_y = row;

        switch (line[i])
        {
        case '^':
          vel_y = -1; // Up
          vel_x = 0;
          break;
        case '>':
          vel_y = 0;
          vel_x = 1; // Right
          break;
        case 'v':
          vel_y = 1; // Down
          vel_x = 0;
          break;
        case '<':
          vel_y = 0;
          vel_x = -1; // Left
          break;
        }
      }
      else if (line[i] == '#') {
        obstacles.push_back(std::make_pair(i,row));
      }

      row_vector.push_back(line[i]);
    }
    map.push_back(row_vector);
    row += 1;
  }

  int visited = 1;
  while (guard_x + vel_x >= 0 && guard_x + vel_x < map[0].size() && guard_y + vel_y >= 0 && guard_y + vel_y < map.size()) {
    if (map[guard_y + vel_y][guard_x + vel_x] == '.' ) {
      map[guard_y][guard_x] = 'X';
      guard_x += vel_x;
      guard_y += vel_y;

      visited += 1;
    }
    else if (map[guard_y + vel_y][guard_x + vel_x] == '#') {
      //spin around.
      //we want to spin around clockwise. 
      //multiply by rotation vector
      //[[0, 1],
      // [-1, 0]]
      int temp = vel_x;
      vel_x = -vel_y;
      vel_y = temp;
    }
    else if (map[guard_y + vel_y][guard_x + vel_x] == 'X' ) {
      map[guard_y][guard_x] = 'X';
      guard_x += vel_x;
      guard_y += vel_y;
    }
  }

  std::cout << visited << std::endl;

  for (int i = 0; i < obstacles.size(); i++) {
    std::cout << "Obstacle " << i + 1 << ", (x,y) = (" << obstacles[i].first << "," << obstacles[i].second << ")  \n";
  }

  return 0;
}