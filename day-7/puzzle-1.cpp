#include <bits/stdc++.h>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/multiprecision/cpp_int.hpp>

#include <string_view>
#include <vector>

using namespace std;

static constexpr std::string_view file_name = "input.txt";

auto read_input() -> std::vector<std::string> {
  auto output = std::vector<std::string>();
  std::ifstream file(file_name.data());
  std::string line;
  while (std::getline(file, line)) {
    output.push_back(line);
  }
  return output;
}

auto calculate_splits(auto &grid) -> int {
  auto nums_rows = grid.size();
  auto nums_cols = grid[0].size();

  using BeamSet = std::unordered_set<int>;

  auto num_splits = 0ll;

  BeamSet beams;
  for (auto &row : grid) {
    BeamSet new_beams;
    for (auto [i, ch] : std::views::enumerate(row)) {
      if (ch == 'S') { // start of a beam
        new_beams.insert(i);
        continue;
      }

      if (ch == '^' && beams.contains(i)) {
        ++num_splits;
        if (i + 1 < nums_cols) {
          new_beams.insert(i + 1);
        }
        if (i - 1 >= 0) {
          new_beams.insert(i - 1);
        }
      } else if (ch == '.' && beams.contains(i)) {
        new_beams.insert(i);
      }
    }
    beams.swap(new_beams);
  }
  return num_splits;
}

auto main() -> int {
  auto grid = read_input();
  auto ans = calculate_splits(grid);
  std::cout << ans << std::endl;
  return 0;
}