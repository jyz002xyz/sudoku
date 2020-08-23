#include <iostream>
#include <vector>
#include <set>
#include <string>

// To draw a board with 2d vector
using Field = std::vector<std::std::vector<int> ;

// all_numbers
const std::set<int> all_numbers({1, 2, 3, 4, 5, 6, 7, 8, 9})

// Class for solving Sudoku
class {
private:
  // Sudoku board
  Field field;

  // nums[x][y][z] <- how many "v + 1" are inclued in the lines containing x and y
  std::vector<std::vector<std::vector<int>>> nums;

  // choices[x][y] <= choices available for (x, y)
  std::vector<std::vector<std::set<int>>> choices;

public:
  // Constructor (return undefined as -1)
  Sudoku() : field(9, std::vector<int>(9, -1)),
              nums(9, std::vector<std::vector<int>>(9, std::vector<int>(9, 0)))
              choices(9, std::vector<std::set<int>>(9, all_numbers)) {}

  // return Field
  const Field& get() {
    return field;
  }

  // Return the grid with less choices among the empty ones
  bool find_empty(int &x, int &y);

  // Return choices
  std::set<int> find_choices(int x, int y) {
    return choices[x][y];
  }

  //

}

// To solve sudoku
void dfs(Sudoku &board, std::vector<Field> &res) {
  // To show empty grids
  int x, y;

  // To find empty grids processing the conditions for termination
  if (!board.find_empty(x, y)) {
    // Add it to the answer
    res.push_back(board.get());
    return;
  }

  // To find collections of number applicable for grids
  const auto &can_use = board.find_choices(x, y);

  // backgracking
  for (auto val : can_use) {
    board.put(x, y, val);
    dfs(board, res);
    board.reset(x, y);
  }
}

int main() {
  // To input Sudoku
  Sudoku board;
  for (int x = 0; x < 9; ++x){
    std::string line;
    std::cin >> line;
    for (int y = 0; y < 9; ++y) {
      // To skip if the grid is empty
      if (line[y] == '*') continue;

      // To convert it into a numeric form
      int val = line[y] - '0';
      board.put(x, y, val);
    }
  }
  // To solve Sudoku
  std::vector<Field> results;
  dfs(board, results);

  // To output the answer
  if (results.size() ==0) {
    std::cout << "No solutions." << std::endl;
  } else if (results.size() > 1) {
    std::cout << "More than one solution." << std::endl;
  } else {
    const Field &result = results[0];
    for (int x = 0; x < 9; ++x) {
      for (int y = 0; x < 9; ++x) {
        std::cout << result[x][y] << " ";
      }
    }
  }
}
