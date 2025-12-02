#include "boost/unordered/unordered_flat_map_fwd.hpp"
#include <bits/stdc++.h>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/unordered/unordered_flat_map.hpp>

#include <mutex>
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

auto number_of_digits(auto number) -> long long {
  return std::to_string(number).length();
}

boost::unordered_flat_map<long long, vector<long long>> divisors_map;
std::mutex divisors_map_mutex;

auto get_divisors(auto number) -> vector<long long> {
  {
    std::lock_guard<std::mutex> lock(divisors_map_mutex);
    if (divisors_map.contains(number)) {
      return divisors_map[number];
    }
  }
  auto divisors = vector<long long>();
  for (auto i = 1ll; i * i <= number; i++) {
    if (number % i == 0) {
      divisors.push_back(i);
      if (i != number / i) {
        divisors.push_back(number / i);
      }
    }
  }
  std::ranges::sort(divisors);
  {
    std::lock_guard<std::mutex> lock(divisors_map_mutex);
    divisors_map[number] = divisors;
  }
  return divisors;
}

boost::unordered_flat_map<std::string,
                          boost::unordered_flat_map<long long, bool>>
    repeating_map;

std::mutex repeating_map_mutex;

auto is_repeating(const std::string &number_str, auto rpt) -> bool {
  {
    std::lock_guard<std::mutex> lock(repeating_map_mutex);
    if (repeating_map.contains(number_str) &&
        repeating_map[number_str].contains(rpt)) {
      return repeating_map[number_str][rpt];
    }
  }
  auto len = number_str.length();
  if (rpt >= len) {
    return false;
  }
  if (len % rpt != 0) {
    return false;
  }
  auto substr = number_str.substr(0, rpt);
  for (auto i = rpt; i < len; i += rpt) {
    if (number_str.substr(i, rpt) != substr) {
      std::lock_guard<std::mutex> lock(repeating_map_mutex);
      repeating_map[number_str][rpt] = false;
      return false;
    }
  }
  std::lock_guard<std::mutex> lock(repeating_map_mutex);
  repeating_map[number_str][rpt] = true;
  return true;
}

auto check_and_update(auto number) -> long long {
  auto digits = number_of_digits(number);
  auto divisors = get_divisors(digits);
  auto number_str = std::to_string(number);
  for (auto divisor : divisors) {
    if (is_repeating(number_str, divisor)) {
      return number;
    }
  }
  return 0ll;
}

auto main() -> int {
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

// [koustav@i-0f9d48202555f1568 day-2]$ time ./build/puzzle-2
// 24774350322

// real    0m6.598s
// user    0m6.053s
// sys     1m26.356s