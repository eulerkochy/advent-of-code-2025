#include <bits/stdc++.h>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/multiprecision/cpp_int.hpp>

#include <string_view>
#include <vector>

using namespace std;

static constexpr std::string_view file_name = "input.txt";

using Point = tuple<long long, long long, long long>;

auto print_point(auto &point) -> void {
  auto [x, y, z] = point;
  std::println("x: {}, y: {}, z: {}", x, y, z);
}

auto read_input() -> std::vector<Point> {
  auto output = std::vector<Point>();
  std::ifstream file(file_name.data());
  std::string line;
  while (std::getline(file, line)) {
    vector<string> tokens;
    boost::algorithm::split(tokens, line, boost::is_any_of(","));
    output.push_back(std::make_tuple(
        std::stoll(tokens[0]), std::stoll(tokens[1]), std::stoll(tokens[2])));
  }
  return output;
}

constexpr auto L2_norm = [](auto &a, auto &b) -> long double {
  // a and b are tuples of long long
  auto [x1, y1, z1] = a;
  auto [x2, y2, z2] = b;
  return sqrtl(powl(x1 - x2, 2) + powl(y1 - y2, 2) + powl(z1 - z2, 2));
};

struct DisjointSet {
  vector<int> parent;
  vector<int> rank;
  DisjointSet(int n) : parent(n), rank(n) {
    for (int i = 0; i < n; i++) {
      parent[i] = i;
      rank[i] = 1;
    }
  }

  auto find(int x) -> int {
    return parent[x] == x ? x : (parent[x] = find(parent[x]));
  }

  auto union_sets(int x, int y) -> void {
    auto root_x = find(x);
    auto root_y = find(y);
    if (root_x == root_y)
      return;
    if (rank[root_x] > rank[root_y]) {
      swap(root_x, root_y);
    }
    parent[root_x] = root_y;
    rank[root_y] += rank[root_x];
  }

  auto rank_of(int x) -> int { return rank[find(x)]; }
};

auto compute_circuits(auto &points) -> long long {
  auto ans = 0ll;

  DisjointSet dsu(points.size());
  using DistPair = tuple<long double, int, int>;

  priority_queue<DistPair, vector<DistPair>, greater<DistPair>>
      global_closest_pairs;

  for (auto [i, point] : std::views::enumerate(points)) {
    for (auto [j, other_point] : std::views::enumerate(points)) {
      if (j <= i)
        continue;
      auto dist = L2_norm(point, other_point);
      global_closest_pairs.push(std::make_tuple(dist, i, j));
    }
  }

  int num_components = points.size();

  while (num_components != 1) {
    auto [dist, i, j] = global_closest_pairs.top();
    global_closest_pairs.pop();
    auto root_i = dsu.find(i);
    auto root_j = dsu.find(j);
    if (root_i != root_j) {
      dsu.union_sets(i, j);
      ans = get<0>(points[i]) * get<0>(points[j]);
      num_components--;
    }
  }

  return ans;
}

auto main() -> int {
  auto points = read_input();
  auto ans = compute_circuits(points);
  std::println("{}", ans);
  return 0;
}