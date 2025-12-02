#include <bits/stdc++.h>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <ranges>
#include <string_view>
#include <thread>
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
          std::vector<std::string> split_range;
          boost::algorithm::split(split_range, range, boost::is_any_of("-"));
          return std::make_tuple(std::stoll(split_range[0]),
                                 std::stoll(split_range[1]));
        }) |
        std::ranges::to<vector<tuple<long long, long long>>>();
  }
  return ranges;
}

static constexpr auto kMaxDigits = 31;
static std::array<std::vector<long long>, kMaxDigits> divisors_table = {};

auto get_divisors(long long n) -> const std::vector<long long> & {
  return divisors_table[n];
}

auto precompute_divisors() -> void {
  auto compute_divisors = [](auto number) -> std::vector<long long> {
    auto divisors = std::vector<long long>();
    for (auto i = 1ll; i * i <= number; i++) {
      if (number % i == 0) {
        divisors.push_back(i);
        if (i != number / i) {
          divisors.push_back(number / i);
        }
      }
    }
    std::ranges::sort(divisors);
    return divisors;
  };
  for (auto i = 1ll; i <= kMaxDigits; i++) {
    divisors_table[i] = compute_divisors(i);
  }
}

auto is_repeating(const std::string &number_str, long long rpt) -> bool {
  auto len = number_str.length();
  if (rpt >= len || len % rpt != 0) {
    return false;
  }
  auto substr = number_str.substr(0, rpt);
  for (size_t i = rpt; i < len; i += rpt) {
    if (number_str.substr(i, rpt) != substr) {
      return false;
    }
  }
  return true;
}

auto check_and_update(auto number) -> long long {
  auto number_str = std::to_string(number);
  auto digits = number_str.length();
  const auto &divisors = get_divisors(digits);
  for (auto divisor : divisors) {
    if (is_repeating(number_str, divisor)) {
      return number;
    }
  }
  return 0ll;
}

auto main() -> int {
  precompute_divisors();
  auto ranges = read_input();
  std::vector<std::jthread> threads;
  threads.reserve(ranges.size());
  std::atomic<long long> ans(0ll);
  for (auto [start, end] : ranges) {

    threads.emplace_back([start, end, &ans]() {
      for (auto number = start; number <= end; number++) {
        ans.fetch_add(check_and_update(number));
      }
    });
  }
  for (auto &thread : threads) {
    thread.join();
  }
  std::println("{}", ans.load());
  return 0;
}

// [koustav@i-0f9d48202555f1568 day-2 (master)]$ time ./build/puzzle-2
// 24774350322

// real    0m0.087s
// user    0m1.304s
// sys     0m0.006s