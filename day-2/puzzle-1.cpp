#include <bits/stdc++.h>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <ranges>
#include <string_view>
#include <vector>

using namespace std;

static constexpr std::string_view file_name = "input.txt";

auto read_input() -> vector<tuple<long long, long long>> {
  std::ifstream file(file_name.data());
  std::string line;
  std::vector<tuple<long long, long long>> ranges;
  while (std::getline(file, line)) {
    std::vector<std::string> ranges_vec;
    boost::algorithm::split(ranges_vec, line, boost::is_any_of(","));
    ranges =
        ranges_vec | std::views::transform([](auto &range) {
          //   std::println("{}", range);
          std::vector<std::string> split_range;
          boost::algorithm::split(split_range, range, boost::is_any_of("-"));
          //   std::println("{} {}", split_range[0], split_range[1]);
          return std::make_tuple(std::stoll(split_range[0]),
                                 std::stoll(split_range[1]));
        }) |
        std::ranges::to<vector<tuple<long long, long long>>>();
  }
  return ranges;
}

auto number_of_digits(auto number) -> long long {
  return std::to_string(number).length();
}

auto check_and_update(auto number) -> long long {
  auto digits = number_of_digits(number);
  if ((digits & 1) == 1) {
    return 0ll;
  }

  auto first_half = std::to_string(number).substr(0, digits / 2);
  auto second_half = std::to_string(number).substr(digits / 2);

  if (first_half == second_half) {
    return number;
  }

  return 0ll;
}

auto main() -> int {
  auto ranges = read_input();
  auto ans = 0ll;
  for (auto [start, end] : ranges) {
    for (auto number = start; number <= end; number++) {
      ans += check_and_update(number);
    }
  }
  std::println("{}", ans);
  return 0;
}