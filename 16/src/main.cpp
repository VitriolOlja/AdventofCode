#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include <queue>
#include <cmath>
#include <set>



struct Node {
  int x;
  int y;
  
  int shortest_distance;
  Node *previous_node;

  Node(int x, int y) : x(x), y(y), shortest_distance(INT_MAX), previous_node(nullptr) {}
  //Node(int x, int y, int cost) : x(x), y(y), cost(cost) {}
};

// Custom comparator (Functor)
struct CompareCost {
    bool operator()(const Node& a, const Node& b) {
        return a.shortest_distance > b.shortest_distance;  // Min-heap: smaller cost has higher priority
    }
};

double euclidian_distance(Node a, Node b) {
  return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}

double manhattan_distance(const Node &current,const Node &goal) {
  return std::abs(current.x - goal.x) + std::abs(current.y - goal.y);
}

double angle(const Node &current, const Node &goal);

double angular_difference(const Node &current, const Node &start,const Node &goal) {
  return std::abs(angle(current, goal) - angle(current, start));
}

// Function to calculate the angle between two vectors
double angle(const Node& current, const Node& goal) {
    // Form the vector from current to goal
    double vec_a_x = goal.x - current.x;
    double vec_a_y = goal.y - current.y;

    // Reference vector (1, 0) for straight east
    double vec_b_x = 1;
    double vec_b_y = 0;

    // Compute dot product
    double dot_product = vec_a_x * vec_b_x + vec_a_y * vec_b_y;

    // Compute magnitudes
    double magnitude_a = std::sqrt(vec_a_x * vec_a_x + vec_a_y * vec_a_y);
    double magnitude_b = std::sqrt(vec_b_x * vec_b_x + vec_b_y * vec_b_y);

    if (magnitude_a == 0 || magnitude_b == 0) {
        std::cerr << "Error: Zero-length vector detected.\n";
        return 0.0; // Avoid division by zero
    }

    // Compute cosine of the angle
    double cos_theta = dot_product / (magnitude_a * magnitude_b);

    // Clamp to [-1, 1] to avoid acos precision issues
    cos_theta = std::max(-1.0, std::min(1.0, cos_theta));

    // Return angle in radians
    return std::acos(cos_theta);
}

enum class ANGLE {
  UP,
  DOWN,
  LEFT,
  RIGHT
};

void calculate_all_distances(Node &t, ANGLE current_angle, std::vector<Node*> &nodes) {
  for (int i = 0; i < nodes.size(); i++) {
    if ((nodes[i]->y == t.y && std::abs(nodes[i]->x - t.x) == 1) || 
        (nodes[i]->x == t.x && std::abs(nodes[i]->y - t.y) == 1)) {
      //we have a node with distance 1.
      //get the currentAngle. 
      //int new_angle = angle(t, *nodes[i]);
      ANGLE new_angle;
      if (nodes[i]->y - t.y == 1) {
        new_angle = ANGLE::DOWN;
      } else if (nodes[i]->y - t.y == -1) {
        new_angle = ANGLE::UP;
      } else if (nodes[i]->x - t.x == 1) {
        new_angle = ANGLE::LEFT;
      } else {
        new_angle = ANGLE::RIGHT;
      }


      int cost_to_move_to_that_node = t.shortest_distance + 1;  //just add one for moving in striahg tline. 

      //if we switch angle -> add cost of 1000
      if (new_angle != current_angle) {
        cost_to_move_to_that_node += 1000;
      }

      //if the shortest distance to get to the node we come across is 
      //larger then the cost we calculated now. We need to update that Node. 
      if (cost_to_move_to_that_node < nodes[i]->shortest_distance) {
        nodes[i]->shortest_distance = cost_to_move_to_that_node;
        nodes[i]->previous_node = &t;

        //and also recalculate all following distances if they maybe are shorter then? 

        calculate_all_distances(*nodes[i], new_angle, nodes);
      }
    }
  }
}

#define INPUT "input.txt"


int main() {
  std::ifstream inputFile(INPUT);

  if (!inputFile)
  {
    std::cerr << "Error: could not open file!" << std::endl;
    return 1;
  }

  std::string line;
  std::set<Node> visited;
  Node *startNode;
  Node *endNode;
  std::vector<Node*> nodes;

  //std::priority_queue<Node, std::vector<Node*>, CompareCost> priority_queue;

  std::getline(inputFile, line);
  int total_width = line.find_last_of('#') - line.find_first_of('#') + 1;
  int row = 1;
  while (std::getline(inputFile, line) && (line.find_first_of('#') == 0))
  {
    for (int column = 0; column < total_width; column++){
      if (line[column] == '.') {
        nodes.push_back(new Node{column, row});
      }
      else if (line[column] == 'E') {
        endNode = new Node{column, row};
        endNode->x = column;
        endNode->y = row;    
        nodes.push_back(endNode);
      }
      else if (line[column] == 'S') {
        startNode = new Node{column, row};
        startNode->shortest_distance = 0;
        startNode->x = column;
        startNode->y = row;
        nodes.push_back(startNode);
      }
    }
    row++;
  }


  std::cout << "finished reading" << std::endl;
  
  ANGLE current_angle = ANGLE::LEFT;
  
  calculate_all_distances(*startNode, current_angle, nodes);

  Node *t = endNode;
  while (t != startNode) {
    std::cout << "(" << t->x << ", " << t->y << ")  has cost " << t->shortest_distance << "\n";
    t = t->previous_node;
  }

  //std::cout << endNode->shortest_distance << std::endl;

  std::cout << std::flush;


  for (Node* node : nodes) {
    delete node;
  }

}