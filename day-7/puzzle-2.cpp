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

auto calculate_timelines(auto &grid) {
  auto nums_rows = grid.size();
  auto nums_cols = grid[0].size();

  using BeamMap = std::unordered_map<int, long long>;

  BeamMap beams;
  for (auto &row : grid) {
    BeamMap new_beams;
    for (auto [col, ch] : std::views::enumerate(row)) {
      if (ch == 'S') { // start of a beam
        new_beams[col] += 1;
        continue;
      }

      if (!beams.contains(col)) {
        continue;
      }

      auto count = beams[col] * 1LL;

      if (ch == '^') {
        if (col - 1 >= 0) {
          new_beams[col - 1] += count;
        }
        if (col + 1 < nums_cols) {
          new_beams[col + 1] += count;
        }
      } else if (ch == '.') {
        new_beams[col] += count;
      }
    }

    beams = std::move(new_beams);
  }

  auto timeline = 0ll;
  for (auto [col, count] : beams) {
    timeline += count;
  }
  return timeline;
}

auto main() -> int {
  auto grid = read_input();
  auto ans = calculate_timelines(grid);
  std::cout << ans << std::endl;
  return 0;
}