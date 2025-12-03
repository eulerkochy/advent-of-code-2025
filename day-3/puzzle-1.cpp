#include <bits/stdc++.h>
#include <string_view>
#include <vector>

using namespace std;

static constexpr std::string_view file_name = "input.txt";

auto read_input() -> vector<std::string> {
  std::ifstream file(file_name.data());
  std::string line;
  std::vector<std::string> ranges;
  while (std::getline(file, line)) {
    ranges.push_back(line);
  }
  return ranges;
}

auto find_joltage(const std::string &line) -> int {
  auto n = line.size();
  auto get_max_in_range = [&](auto start, auto end) -> tuple<int, int> {
    auto max_value = 0, max_idx = 0;
    for (auto i = start; i < end; ++i) {
      auto value = line[i] - '0';
      if (value > max_value) {
        max_value = value;
        max_idx = i;
      }
    }
    return std::make_tuple(max_value, max_idx);
  };
  auto [max_value, max_value_idx] = get_max_in_range(0, n - 1);
  auto [second_max_value, _] = get_max_in_range(max_value_idx + 1, n);
  return max_value * 10 + second_max_value;
}

auto main() -> int {
  auto ranges = read_input();
  auto ans = 0;
  for (auto &line : ranges) {
    ans += find_joltage(line);
  }
  std::println("{}", ans);
  return 0;
}