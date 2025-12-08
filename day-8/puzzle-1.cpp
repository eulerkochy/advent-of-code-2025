#include <algorithm>
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
    std::ranges::iota(parent, 0);
    std::ranges::fill(rank, 1);
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

auto compute_circuits(auto &points, auto num_closest_pairs) -> long long {
  auto ans = 0ll;

  DisjointSet dsu(points.size());

  priority_queue<tuple<long double, int, int>> global_closest_pairs;

  for (auto [i, point] : std::views::enumerate(points)) {
    for (auto [j, other_point] : std::views::enumerate(points)) {
      if (j <= i)
        continue;
      auto dist = L2_norm(point, other_point);
      if (global_closest_pairs.size() >= num_closest_pairs) {
        if (dist < get<0>(global_closest_pairs.top())) {
          global_closest_pairs.pop();
          global_closest_pairs.push(std::make_tuple(dist, i, j));
        }
      } else {
        global_closest_pairs.push(std::make_tuple(dist, i, j));
      }
    }
  }

  while (!global_closest_pairs.empty()) {
    auto [dist, i, j] = global_closest_pairs.top();
    global_closest_pairs.pop();
    if (dsu.find(i) != dsu.find(j)) {
      dsu.union_sets(i, j);
    }
  }

  unordered_set<int> root_components;
  for (auto i = 0; i < points.size(); ++i) {
    root_components.insert(dsu.find(i));
  }

  // top-3 ranks
  constexpr auto kTopRanks = 3;
  std::vector<int> ranks;
  for (auto root : root_components) {
    ranks.push_back(dsu.rank_of(root));
  }
  std::ranges::partial_sort(ranks, ranks.begin() + kTopRanks,
                            std::greater<int>());
  ans = std::accumulate(ranks.begin(), ranks.begin() + kTopRanks, 1ll,
                        std::multiplies<long long>());
  return ans;
}

auto main() -> int {
  auto points = read_input();
  auto ans = compute_circuits(points, 1000);
  std::println("{}", ans);
  return 0;
}