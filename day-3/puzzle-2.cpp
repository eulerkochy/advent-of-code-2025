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

constexpr auto kMaxDigits = 13;
std::array<long long, kMaxDigits> pow10 = {
    1,           10,           100,          1000,      10000,
    100000,      1000000,      10000000,     100000000, 1000000000,
    10000000000, 100000000000, 1000000000000};

auto find_joltage(const std::string &line) -> long long {
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

  auto max_value = 0, max_idx = -1;
  constexpr auto kMaxDigits = 12;

  auto joltage = 0ll;

  for (auto j = 1; j <= kMaxDigits; ++j) {
    auto mul = pow10[kMaxDigits - j];
    auto start = max_idx + 1;
    auto end = static_cast<size_t>(n - (kMaxDigits - j));
    auto [current_max_value, current_max_idx] = get_max_in_range(start, end);
    long long current_value = current_max_value * 1LL * mul;
    joltage += current_value;
    max_idx = current_max_idx;
  }

  return joltage;
}

auto main() -> int {
  auto ranges = read_input();
  auto ans = 0ll;
  std::vector<std::future<long long>> futures;
  futures.reserve(ranges.size());
  for (auto &line : ranges) {
    futures.emplace_back(std::async(std::launch::deferred, find_joltage, line));
  }
  for (auto &future : futures) {
    ans += future.get();
  }
  std::println("{}", ans);
  return 0;
}