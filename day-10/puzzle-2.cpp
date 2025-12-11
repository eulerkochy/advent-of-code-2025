#include "boost/algorithm/string/trim.hpp"
#include <algorithm>
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <execution>
#include <ranges>
#include <string_view>
#include <vector>

using namespace std;

static constexpr std::string_view file_name = "input.txt";

struct Board {
  int n;
  vector<int> config;
  vector<vector<int>> switches;
  vector<int> joltage;

  friend ostream &operator<<(ostream &os, const Board &board) {
    os << "Board(n=" << board.n << ", config={";
    for (auto &ch : board.config) {
      os << ch << ", ";
    }
    os << "}, switches={";
    for (auto &sw_indices : board.switches) {
      os << "(";
      for (auto &index : sw_indices) {
        os << index << ", ";
      }
      os << "), ";
    }
    os << "}, joltage={";
    for (auto &ch : board.joltage) {
      os << ch << ", ";
    }
    os << "})";
    return os;
  }
};

// [.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
// [...#.] (0,2,3,4) (2,3) (0,4) (0,1,2) (1,2,3,4) {7,5,12,7,2}
// [.###.#] (0,1,2,3,4) (0,3,4) (0,1,2,4,5) (1,2) {10,11,11,5,10,5}
auto read_input() -> std::vector<Board> {
  auto output = std::vector<Board>();
  std::ifstream file(file_name.data());
  std::string line;
  auto parse_config = [](const std::string &config_str) -> vector<int> {
    auto config = vector<int>();
    for (auto &ch : config_str) {
      if (ch == '.') {
        config.push_back(0);
      } else {
        config.push_back(1);
      }
    }
    return config;
  };

  auto parse_switches =
      [](const std::string &switches_str) -> vector<vector<int>> {
    auto switches = vector<vector<int>>();
    auto tokens = vector<string>();
    // each switch is a list of indices under (parentheses)
    boost::algorithm::split(tokens, switches_str, boost::is_any_of(")"));
    for (auto &token : tokens) {
      boost::trim(token);
      if (token.size() == 0) {
        continue;
      }
      boost::trim_left_if(token, boost::is_any_of("("));
      auto switch_indices = vector<int>();
      auto indices_str = token;
      boost::trim(indices_str);
      if (indices_str.size() == 0) {
        continue;
      }
      auto indices_tokens = vector<string>();
      boost::algorithm::split(indices_tokens, indices_str,
                              boost::is_any_of(","));
      for (auto &index_str : indices_tokens) {
        switch_indices.push_back(std::stoi(index_str));
      }
      switches.push_back(switch_indices);
    }
    return switches;
  };

  auto parse_joltage = [](auto &extra_str) -> vector<int> {
    // remove { and } from the string
    boost::trim(extra_str);
    extra_str = extra_str.substr(1, extra_str.size() - 2);
    auto extra = vector<int>();
    auto tokens = vector<string>();
    boost::algorithm::split(tokens, extra_str, boost::is_any_of(","));
    for (auto &token : tokens) {
      extra.push_back(std::stoi(token));
    }
    return extra;
  };

  while (std::getline(file, line)) {
    // parse until the first ]
    Board board;
    auto pos = line.find(']');
    auto config_str = line.substr(1, pos - 1);
    board.n = config_str.size();
    board.config = parse_config(config_str);
    auto remaining_str = line.substr(pos + 1);
    // find the last )
    auto last_paren_pos = remaining_str.rfind(')');
    auto switches_str = remaining_str.substr(0, last_paren_pos + 1);
    board.switches = parse_switches(switches_str);

    auto extra_str = remaining_str.substr(last_paren_pos + 1);
    board.joltage = parse_joltage(extra_str);
    output.push_back(board);
  }
  return output;
}

auto solve_board_linear(const Board &board) -> long long {
  int num_counters = board.n;
  int num_buttons = board.switches.size();

  vector<vector<long double>> mat(num_counters,
                                  vector<long double>(num_buttons + 1, 0.0));

  // Fill Matrix
  // A_ij = 1 if button j affects counter i
  for (int btn = 0; btn < num_buttons; ++btn) {
    for (int affected_counter : board.switches[btn]) {
      mat[affected_counter][btn] = 1.0;
    }
  }
  for (int i = 0; i < num_counters; ++i) {
    mat[i][num_buttons] = board.joltage[i];
  }

  int pivot_row = 0;
  vector<int> pivot_col_to_row(num_buttons, -1);
  const long double EPS = 1e-9;

  for (int col = 0; col < num_buttons && pivot_row < num_counters; ++col) {
    // Find pivot
    int sel = -1;
    for (int row = pivot_row; row < num_counters; ++row) {
      if (abs(mat[row][col]) > EPS) {
        sel = row;
        break;
      }
    }

    if (sel == -1)
      continue; // No pivot in this column, it's a free variable

    // Swap rows
    swap(mat[pivot_row], mat[sel]);

    // Normalize row
    long double div = mat[pivot_row][col];
    for (int j = col; j <= num_buttons; ++j) {
      mat[pivot_row][j] /= div;
    }

    // Eliminate other rows
    for (int row = 0; row < num_counters; ++row) {
      if (row != pivot_row && abs(mat[row][col]) > EPS) {
        long double factor = mat[row][col];
        for (int j = col; j <= num_buttons; ++j) {
          mat[row][j] -= factor * mat[pivot_row][j];
        }
      }
    }

    pivot_col_to_row[col] = pivot_row;
    pivot_row++;
  }

  // Check for inconsistency (0 = non-zero)
  for (int row = pivot_row; row < num_counters; ++row) {
    if (abs(mat[row][num_buttons]) > EPS) {
      return -1; // No solution
    }
  }

  // Identify Free Variables
  vector<int> free_vars;
  vector<int> pivot_vars;
  for (int col = 0; col < num_buttons; ++col) {
    if (pivot_col_to_row[col] == -1)
      free_vars.push_back(col);
    else
      pivot_vars.push_back(col);
  }

  // Calculate strict upper bounds for free variables
  // Since matrix entries are non-negative, x_j cannot exceed the target of any
  // counter it affects.
  vector<long long> bounds(num_buttons);
  for (int btn = 0; btn < num_buttons; ++btn) {
    bounds[btn] = std::ranges::min(
        board.switches[btn] |
        std::views::transform([&](int idx) { return board.joltage[idx]; }));
  }

  long long min_total_presses = LLONG_MAX;

  // Recursive search over free variables
  auto solve_recursive = [&](auto &&self, int free_idx,
                             vector<long long> &current_solution) -> void {
    if (free_idx == free_vars.size()) {
      long long current_sum = 0;
      bool possible = true;

      for (int fv : free_vars)
        current_sum += current_solution[fv];

      // x_pivot = Target - sum(coeff * x_free)
      for (int pv : pivot_vars) {
        int r = pivot_col_to_row[pv];
        long double val = mat[r][num_buttons];

        for (int fv : free_vars) {
          if (abs(mat[r][fv]) > EPS) {
            val -= mat[r][fv] * current_solution[fv];
          }
        }

        // Check integer and non-negative
        if (val < -EPS) {
          possible = false;
          break;
        }
        long long round_val = llround(val);
        if (abs(val - round_val) > 1e-4) {
          possible = false;
          break;
        }

        current_solution[pv] = round_val;
        current_sum += round_val;
      }

      if (possible) {
        if (current_sum < min_total_presses) {
          min_total_presses = current_sum;
        }
      }
      return;
    }

    // Iterate this free variable
    int var_idx = free_vars[free_idx];
    for (auto val = 0ll; val <= bounds[var_idx]; ++val) {
      current_solution[var_idx] = val;
      self(self, free_idx + 1, current_solution);
    }
  };

  vector<long long> solution(num_buttons, 0);
  solve_recursive(solve_recursive, 0, solution);

  return min_total_presses;
}

auto solve(auto &boards) {
  return std::transform_reduce(std::execution::seq, boards.begin(),
                               boards.end(),
                               0LL,                    // initial value
                               std::plus<>{},          // reduce operation
                               [](const auto &board) { // transform operation
                                 return solve_board_linear(board);
                               });
}

auto main() -> int {
  auto boards = read_input();
  auto ans = solve(boards);
  std::println("{}", ans);
  return 0;
}