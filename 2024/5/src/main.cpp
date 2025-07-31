#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_set>
#include <sstream>



//part1.
// approach:
//save the rules in a map. where each page maps to a vector of pages. it has 
// iterate over the updated pages.
//for each page - use it on the map as key - the result is a vector of pages that has to come before it. 
//check the visited - have we visited those yet? if not - it does not follow the rules. 



//part 2:
//quicksort based sorting? 
//take the rightmost element - check all elements before it
//put those that are not allowed in front behind the element. 
//now sort the values to the left and the values to the right recursively. (Divide and Conquer.)
//hopefully works.

int partition(std::vector<int> &update, const std::map<int, std::unordered_set<int>> &rules, int left, int right) {

    // Selecting last element as the pivot
    int pivot = update[right];

    // Index of elemment just before the last element
    // It is used for swapping
    int i = (left - 1);



    for (int j = left; j <= right - 1; j++) {
        // if there are no rules for the pivot. 
        //or the element at j is allowed to be before the pivot. i.e there are not rules for it. 

        if (rules.count(pivot) == 0 || rules.at(pivot).count(update[j]) == 0) {
            i++;
            std::swap(update[i], update[j]);
        }
    }

    // Put pivot to its position
    std::swap(update[i + 1], update[right]);

    // Return the point of partition
    return (i + 1);
}

void quickSort(std::vector<int> &update, const std::map<int, std::unordered_set<int>> &rules, int left, int right) {

    // Base case: This part will be executed till the starting
    // index low is lesser than the ending index high
    if (left < right) {

        // pi is Partitioning Index, arr[p] is now at
        // ri
        int pi = partition(update, rules, left, right);

        // Separately sort elements before and after the
        // Partition Index pi
        quickSort(update, rules, left, pi - 1);
        quickSort(update, rules, pi + 1, right);
    }
}



int main() {
  //hardcoded so debugger can find it in vscode. 
  std::ifstream input("C:\\Users\\Dwight\\Desktop\\c_dev\\AoC\\5\\build\\input.txt");


  std::map<int, std::vector<int>> rules;
  std::vector<std::vector<int>> updates;

  std::map<int, std::unordered_set<int>> rules2; //another type of rules for simpler checking.
  
  std::string line;
  while(std::getline(input, line)) {
    if (line.empty()) {
      break; 
    }



    std::stringstream ss(line);
    std::string pair;
    int first;
    int second;

    std::getline(ss, pair, '|'); // Split line by '|'
    try {
        first = std::stoi(pair); // Convert string to int and add to group
    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid number in file: " << pair << std::endl;
    }
    std::getline(ss, pair, '|'); // Split line by '|'
    try {
        second = std::stoi(pair); // Convert string to int and add to group
    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid number in file: " << pair << std::endl;
    }
    
    rules[first].push_back(second);
    rules2[first].insert(second);
  }

  while(std::getline(input, line)) {
    std::vector<int> group; 
    std::stringstream ss(line);
    std::string pair;

    while (std::getline(ss, pair, ',')) { // Split line by '|'
        try {
            group.push_back(std::stoi(pair)); // Convert string to int and add to group
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid number in file: " << pair << std::endl;
        }
    }

    updates.push_back(group);
  }

  int sum = 0;

  std::vector<std::vector<int>> not_correct;

  for ( const std::vector<int> &update : updates) {
    std::unordered_set<int> visited;

    bool correct = true;
    for (const int& page : update) {
      //std::vector<int> can_not_come_before = ;
      
      for(const int& forbidden_page : rules[page]) {
        if (visited.count(forbidden_page) > 0){
          correct = false; 
          break;
        }
      }
      if(!correct) {
        break;
      }

      visited.insert(page);
    }

    if(correct) {
      //get the middle page number
      int middle = update.size() / 2;
      //std::cout << update[middle] << std::endl;
      sum += update[middle];
    }
    if(!correct){
      not_correct.push_back(update);
    }

  }


  //lets sort of do quicksrt of sorts.

  for (std::vector<int> &update : not_correct){
    quickSort(update, rules2, 0, update.size() - 1);
  }

  std::cout << std::endl;

  int sum2 = 0;
  for (const std::vector<int> & update : not_correct){
    int middle = update.size() / 2;
    sum2 += update[middle];
  }

  std::cout << sum2 << std::endl;
  



  return 0;
}