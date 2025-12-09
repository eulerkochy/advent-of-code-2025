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

struct Segment {
  long long x1, y1, x2, y2;
  bool is_horizontal;
};

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

auto build_segments(const std::vector<Point> &points) -> std::vector<Segment> {
  std::vector<Segment> segments;
  auto n = points.size();
  for (size_t i = 0; i < n; ++i) {
    auto [x1, y1] = points[i];
    auto [x2, y2] = points[(i + 1) % n];
    bool is_horizontal = (y1 == y2);
    segments.push_back({std::min(x1, x2), std::min(y1, y2), std::max(x1, x2),
                        std::max(y1, y2), is_horizontal});
  }
  return segments;
}

auto is_on_segment(long long px, long long py, const Segment &seg) -> bool {
  if (seg.is_horizontal) {
    return py == seg.y1 && px >= seg.x1 && px <= seg.x2;
  } else {
    return px == seg.x1 && py >= seg.y1 && py <= seg.y2;
  }
}

auto is_inside_or_on_boundary(long long px, long long py,
                              const std::vector<Segment> &segments) -> bool {
  for (const auto &seg : segments) {
    if (is_on_segment(px, py, seg)) {
      return true;
    }
  }

  // Ray casting
  int crossings = 0;
  for (const auto &seg : segments) {
    if (!seg.is_horizontal)
      continue;
    if (seg.y1 <= py)
      continue; // Segment must be above the point
    if (px >= seg.x1 && px < seg.x2) {
      crossings++;
    }
  }
  return (crossings % 2) == 1;
}

auto is_valid_rectangle(const Point &p1, const Point &p2,
                        const std::vector<Segment> &segments) -> bool {
  auto [px1, py1] = p1;
  auto [px2, py2] = p2;

  // Normalize so x1 < x2 and y1 < y2
  auto x1 = std::min(px1, px2);
  auto x2 = std::max(px1, px2);
  auto y1 = std::min(py1, py2);
  auto y2 = std::max(py1, py2);

  // Check all 4 corners are inside or on boundary
  std::array<std::pair<long long, long long>, 4> corners = {
      {{x1, y1}, {x1, y2}, {x2, y1}, {x2, y2}}};

  for (const auto &[cx, cy] : corners) {
    if (!is_inside_or_on_boundary(cx, cy, segments)) {
      return false;
    }
  }

  // Check no polygon edge passes through the interior of the rectangle
  for (const auto &seg : segments) {
    if (seg.is_horizontal) {
      if (seg.y1 > y1 && seg.y1 < y2) {
        if (seg.x2 > x1 && seg.x1 < x2) {
          return false;
        }
      }
    } else {
      if (seg.x1 > x1 && seg.x1 < x2) {
        if (seg.y2 > y1 && seg.y1 < y2) {
          return false;
        }
      }
    }
  }

  return true;
}

constexpr auto area = [](auto &a, auto &b) -> long long {
  auto [x1, y1] = a;
  auto [x2, y2] = b;
  auto len = abs(x1 - x2) + 1;
  auto width = abs(y1 - y2) + 1;
  return len * width;
};

auto compute_max_area(const std::vector<Point> &points,
                      const std::vector<Segment> &segments) -> long long {
  auto ans = 0ll;
  auto n = points.size();

  for (size_t i = 0; i < n; ++i) {
    for (size_t j = i + 1; j < n; ++j) {
      if (is_valid_rectangle(points[i], points[j], segments)) {
        ans = max(ans, area(points[i], points[j]));
      }
    }
  }

  return ans;
}

auto main() -> int {
  auto points = read_input();
  auto segments = build_segments(points);
  auto ans = compute_max_area(points, segments);
  std::println("{}", ans);
  return 0;
}