#include <bits/stdc++.h>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <string_view>
#include <vector>

using namespace std;

static constexpr std::string_view file_name = "input.txt";

constexpr auto kThreshold = 4;

// 3-5
// 10-14
// 16-20
// 12-18
//
// 1
// 5
// 8
// 11
// 17
// 32
auto read_input()
    -> tuple<vector<tuple<long long, long long>>, vector<long long>> {
  std::ifstream file(file_name.data());
  std::string line;
  std::vector<tuple<long long, long long>> ranges;
  std::vector<long long> values;
  bool range_mode = true;
  while (std::getline(file, line)) {
    if (line.empty()) {
      range_mode = false;
      continue;
    }
    if (range_mode) {
      std::vector<std::string> split_line;
      boost::algorithm::split(split_line, line, boost::is_any_of("-"));
      ranges.push_back(std::make_tuple(std::stoll(split_line[0]),
                                       std::stoll(split_line[1])));
    } else {
      values.push_back(std::stoll(line));
    }
  }
  return std::make_tuple(ranges, values);
}

auto check_fresh(auto &ranges, auto &values) -> long long {
  std::ranges::sort(ranges, [](auto &a, auto &b) -> bool {
    return std::get<0>(a) < std::get<0>(b);
  });
  auto [L, R] = ranges[0];
  vector<tuple<long long, long long>> non_overlapping_rng;
  for (auto i = 1; i < ranges.size(); ++i) {
    auto [start, end] = ranges[i];
    if (start <= R) {
      // extend the range
      R = max(R, end);
    } else {
      non_overlapping_rng.push_back(std::make_tuple(L, R));
      std::tie(L, R) = ranges[i];
    }
  }
  non_overlapping_rng.push_back(std::make_tuple(L, R));
  auto ans = 0ll;
  for (auto &range : non_overlapping_rng) {
    auto [start, end] = range;
    ans += end - start + 1ll;
  }
  return ans;
}

auto main() -> int {
  auto [ranges, values] = read_input();
  auto ans = check_fresh(ranges, values);
  std::println("{}", ans);
  return 0;
}