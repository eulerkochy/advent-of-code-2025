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
  std::vector<tuple<long long, int>> events;
  for (auto &range : ranges) {
    auto [start, end] = range;
    events.push_back(std::make_tuple(start, 1));
    events.push_back(std::make_tuple(end, -1));
  }
  for (auto &value : values) {
    events.push_back(std::make_tuple(value, 0));
  }

  auto event_cmp = [](auto &a, auto &b) -> bool {
    auto &[x, t_x] = a;
    auto &[y, t_y] = b;
    return (x < y) || (x == y && t_x > t_y);
  };

  std::ranges::sort(events, event_cmp);
  auto ans = 0ll;
  auto open_ranges = 0;

  for (auto &event : events) {
    auto [value, type] = event;
    open_ranges += type;

    if (type == 0) {
      if (open_ranges > 0) {
        ans++;
        // std::println("{}", value);
      }
    }
  }
  return ans;
}

auto main() -> int {
  auto [ranges, values] = read_input();
  auto ans = check_fresh(ranges, values);
  std::println("{}", ans);
  return 0;
}