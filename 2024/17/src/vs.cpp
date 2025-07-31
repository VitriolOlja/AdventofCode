#include <iostream>
#include <vector>

#include <map>
#include <functional>

struct TreeNode {
    unsigned long long data;                        
    std::vector<TreeNode*> children;   

    // Constructor to initialize the node with data only
    TreeNode(long long val) : data(val) {}

    // Constructor to initialize the node with data and children
    TreeNode(long long val, const std::vector<TreeNode*>& childNodes)
        : data(val), children(childNodes) {}
};


std::vector<int> program (long long A) {

  std::vector<int> ret;

  //2,4,1,5,7,5,
  //1,6,
  //0,3,
  //4,0,
  //5,5,
  //3,0

  do {
    int B = A & 7;          //2, 4 opcode 2 = bst  combo(4) = A
    B = B ^ 5;              //1, 5    opcode 1 = bxl bitwise xor B with literal 5 and store in B
    long long C = A >> B;   //7, 5 opcode 7 = cdv store in C = A / 2**combo(5) combo 5 is B
    B = B ^ 6;              //1, 6 literal 6 
    A = A >> 3;             //03, adv with combo 3 which is literal 3. 

    B = B ^ C;              //4, 0 bxc bitwise xor with b and C and store in B.

    ret.push_back(B & 7);   //5,5
  } while (A > 0);          //3,0

  return ret;
}


void recursive_delete(TreeNode* node) {
  if (node->children.empty()) {
    return;
  }
  for (TreeNode * child : node->children) {
    recursive_delete(child);
  }

  delete node;
}



int main(int argc, char* argv[]) {
  int instructions[] = {2,4,1,5,7,5,1,6,0,3,4,0,5,5,3,0};
  int sequence_length = sizeof(instructions) / sizeof(instructions[0]);


  // Root of the tree
    TreeNode* root = new TreeNode(0);

    // Build the tree
    std::vector<TreeNode*> current_level = {root};
    for (int i = 0; i < sequence_length; ++i) {
        std::vector<TreeNode*> next_level;

        for (TreeNode* node : current_level) {
            // Multiply the current value by 8
            unsigned long long A_here = node->data << 3;

            for (int k = 0; k < 8; k++) {
                unsigned long long candidate = A_here + k;

                // Compute the output
                long B = candidate & 7;
                B = B ^ 5;
                long C = candidate >> B;
                B = B ^ 6;
                B = B ^ C;
                long out = B & 7;

                // Verify the output
                bool found = true;
                std::vector<int> res = program(candidate);
                int rev_index = sequence_length - 1 - i;

                for (int j = 0; rev_index + j < sequence_length; j++) {
                    if (res[j] != instructions[rev_index + j]) {
                        found = false;
                        break;
                    }
                    if(res[j] < 0){
                      std::cout << "weird negative modulus???" << std::endl;
                    }
                }

                if (found) {
                    bool already_there = false;
                    for (TreeNode* n : next_level) {
                      if (n->data == candidate) {
                        already_there = true;
                        break;
                      }
                    }

                    if(!already_there) {
                      TreeNode* child = new TreeNode(candidate);
                      node->children.push_back(child);
                      next_level.push_back(child);

                      std::cout << "Matching for digit " << i
                                << " with A = " << candidate
                                << " gives output: [";
                      for (const int& a : res) {
                          std::cout << a << ",";
                      }
                      std::cout << "]\n";
                    }
                }
            }
        }

        // Move to the next level
        current_level = next_level;
    }

    std::cout << "\nTree built. Traversing possible sequences...\n";

    // Optional: Traverse the tree to find complete sequences
    std::function<void(TreeNode*, std::vector<long long>&)> traverse = [&](TreeNode* node, std::vector<long long>& path) {
        path.push_back(node->data);

        if (node->children.empty()) {
            // Leaf node; print the sequence
            std::cout << "Valid sequence: ";
            for (long long val : path) {
                std::cout << val << " ";
            }
            std::cout << "\n";
        } else {
            for (TreeNode* child : node->children) {
                traverse(child, path);
            }
        }

        path.pop_back();
    };

    std::vector<long long> path;
    traverse(root, path);

    // Clean up
    recursive_delete(root);

    return 0;
}