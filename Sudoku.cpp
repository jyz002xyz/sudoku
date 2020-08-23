#include <iostream>
#include <vector>
#include <set>
#include <string>

// To draw a board with 2d vector
using Field = std::vector<std::vector<int>>;

// all_numbers
const std::set<int> all_numbers({1, 2, 3, 4, 5, 6, 7, 8, 9});

// Class for solving Sudoku
class Sudoku {
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
              nums(9, std::vector<std::vector<int>>(9, std::vector<int>(9, 0))),
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
  void put_detail(int x, int y, int val, int x2, int y2);

  //
  void reset_detail(int x, int y, int val, int x2, int y2);

  //
  void put(int x, int y, int val);

  //
  void reset(int x, int y);

  //
  void process();

  void print() {
    for (int x = 0; x < 9; ++x) {
      for (int y = 0; y < 9; ++y) {
        if (field[x][y] == -1) std::cout << "*";
        else std::cout << field[x][y];
        std::cout << " ";
      }
      std::cout << std::endl;
    }
  }

};

bool Sudoku::find_empty(int &x, int &y) {
  size_t min_num_choices = 10;
  for (int i = 0; i < 9; ++i) {
    for (int j = 0; j < 9; ++j) {
      if (field[i][j] != -1) continue;
      if (min_num_choices > choices[i][j].size()) {
          min_num_choices = choices[i][j].size();
          x = i;
          y = j;
      }
    }
  }
  //
  if (min_num_choices == 10) return false;
  else return true;
}


//
void Sudoku::put_detail(int x, int y, int val, int x2, int y2) {
  if (x == x2 && y == y2) return;

  //
  if (field[x2][y2] != -1) return;

  //
  if (nums[x2][y2][val - 1] == 0) choices[x2][y2].erase(val);

  //
  ++nums[x2][y2][val - 1];

}

//
void Sudoku::put(int x, int y, int val) {
    //
    field[x][y] = val;

    //
    for (int i = 0; i < 9; i++) put_detail(x, y, val, x, i);
    for (int i = 0; i < 9; i++) put_detail(x, y, val, i, y);
    int cx = x / 3 * 3 + 1, cy = y / 3 * 3 + 1;
    for (int i = cx - 1; i <= cx + 1; ++i)
      for (int j = cy - 1; j <= cy + 1; ++j)
        put_detail(x, y, val, i, j);

}

//
void Sudoku::reset_detail(int x, int y, int val, int x2, int y2) {
  if (x == x2 && y == y2) return;

  //
  if (field[x2][y2] != -1) return;

  //
  --nums[x2][y2][val - 1];

  //
  if (nums[x2][y2][val - 1] == 0) choices[x2][y2].insert(val);

}

//
void Sudoku::reset(int x, int y) {
  //
  int val = field[x][y];
  for (int i = 0; i < 9; i++) reset_detail(x, y, val, x, i);
  for (int i = 0; i < 9; i++) reset_detail(x, y, val, i, y);
  int cx = x / 3 * 3 + 1, cy = y / 3 * 3 + 1;
  for (int i = cx - 1; i <= cx + 1; ++i)
    for (int j = cy - 1; j <= cy + 1; ++j)
      reset_detail(x, y, val, i, j);

    //
    field[x][y] = -1;
}

void Sudoku::process() {
  //
  for (int val = 1; val <= 9; ++val) {
    //
    for (int x = 0; x < 9; ++x) {
      bool exist = false;
      std::vector<int> can_enter;
      for (int y = 0; y < 9; ++y) {
        if (field[x][y] == val) exist = true;
        if (field[x][y] == -1
            && choices[x][y].count(val)) {
              can_enter.push_back(y);
      }
    }
    //
    if (!exist && can_enter.size() == 1) {
      int y = can_enter[0];
      put(x, y, val);
    }
  }

  //
  for (int y = 0; y < 9; ++y) {
    bool exist = false;
    std::vector<int> can_enter;
    for (int x = 0; x < 9; ++x) {
      if (field[x][y] == val) exist = true;
      if (field[x][y] == -1
        && choices[x][y].count(val)) {
          can_enter.push_back(x);
        }
    }
    //
    if (!exist && can_enter.size() == 1) {
      int x = can_enter[0];
      put(x, y, val);
    }
  }
  //
  for (int bx = 0; bx < 3; ++bx) {
    for (int by = 0; by < 3; ++by) {
      bool exist = false;
      std::vector<std::pair<int,int>> can_enter;
      for (int x = bx *3; x < (bx + 1) * 3; ++x) {
        for (int y = by * 3; y < (by + 1) *3; ++y) {
          if (field[x][y] == val) exist = true;
          if (field[x][y] == -1
              && choices[x][y].count(val)) {
                can_enter.emplace_back(x, y);
              }
        }
      }
      //
      if (!exist && can_enter.size() == 1) {
        int x = can_enter[0].first;
        int y = can_enter[0].second;
        put(x, y, val);
      }
    }
  }
}
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
      for (int y = 0; y < 9; ++y) {
        std::cout << result[x][y] << " ";
      }
      std::cout << std::endl;
    }
  }
}
