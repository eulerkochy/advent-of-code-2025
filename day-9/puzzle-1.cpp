#include <algorithm>
#include <bits/stdc++.h>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/multiprecision/cpp_int.hpp>

#include <string_view>
#include <vector>

using namespace std;

static constexpr std::string_view file_name = "input.txt";

using Point = tuple<long long, long long>;

auto print_point(auto &point) -> void {
  auto [x, y] = point;
  std::println("x: {}, y: {}", x, y);
}

auto read_input() -> std::vector<Point> {
  auto output = std::vector<Point>();
  std::ifstream file(file_name.data());
  std::string line;
  while (std::getline(file, line)) {
    vector<string> tokens;
    boost::algorithm::split(tokens, line, boost::is_any_of(","));
    output.push_back(
        std::make_tuple(std::stoll(tokens[0]), std::stoll(tokens[1])));
  }
  return output;
}

constexpr auto area = [](auto &a, auto &b) -> long long {
  // a and b are tuples of long long
  auto [x1, y1] = a;
  auto [x2, y2] = b;
  auto len = abs(x1 - x2 + 1);
  auto width = abs(y1 - y2 + 1);
  return len * width;
};

auto compute_max_area(auto &points) -> long long {
  auto ans = 0ll;

  for (auto i = 0; i < points.size(); ++i) {
    for (auto j = i + 1; j < points.size(); ++j) {
      ans = max(ans, area(points[i], points[j]));
    }
  }

  return ans;
}

auto main() -> int {
  auto points = read_input();
  auto ans = compute_max_area(points);
  std::println("{}", ans);
  return 0;
}