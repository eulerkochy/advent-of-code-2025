#include <bits/stdc++.h>
#include <string_view>
#include <vector>

using namespace std;

static constexpr std::string_view file_name = "input.txt";

constexpr auto kThreshold = 4;

auto read_input() -> vector<string> {
  std::ifstream file(file_name.data());
  std::string line;
  std::vector<string> ranges;
  while (std::getline(file, line)) {
    ranges.push_back(line);
  }
  return ranges;
}

using Graph = vector<unordered_set<int>>;

auto print_graph(const vector<string> &ranges, const Graph &graph) -> void {
  auto n = ranges.size();
  auto m = ranges[0].size();
  auto get_x_y = [m](int pos) -> tuple<int, int> { return {pos / m, pos % m}; };
  auto get_pos = [m](int i, int j) -> int { return i * m + j; };
  for (auto i = 0; i < ranges.size(); ++i) {
    for (auto j = 0; j < ranges[i].size(); ++j) {
      auto pos = get_pos(i, j);
      if (ranges[i][j] == '.') {
        std::print(".");
      } else {
        if (graph[pos].size() < kThreshold) {
          std::print("x");
        } else {
          std::print("@");
        }
      }
    }
    std::cout << std::endl;
  }
}

auto get_graph(const vector<string> &ranges) -> Graph {
  auto n = ranges.size();
  auto m = ranges[0].size();

  auto get_pos = [m](int i, int j) -> int { return i * m + j; };

  auto get_x_y = [m](int pos) -> tuple<int, int> { return {pos / m, pos % m}; };

  auto is_valid = [n, m](int i, int j) -> bool {
    return i >= 0 && i < n && j >= 0 && j < m;
  };

  auto max_size = get_pos(n - 1, m - 1) + 1;

  auto graph = Graph(max_size);

  constexpr auto dx = array<int, 8>{-1, 0, 1, 0, 1, -1, -1, 1};
  constexpr auto dy = array<int, 8>{0, 1, 0, -1, 1, 1, -1, -1};

  for (auto i = 0; i < n; ++i) {
    for (auto j = 0; j < m; ++j) {
      if (ranges[i][j] == '.') {
        continue;
      }
      auto pos = get_pos(i, j);
      for (auto [_x, _y] : std::views::zip(dx, dy)) {
        auto nx = i + _x;
        auto ny = j + _y;
        if (is_valid(nx, ny) && ranges[nx][ny] == '@') {
          auto npos = get_pos(nx, ny);
          graph[pos].emplace(npos);
        }
      }
    }
  }

  return graph;
}

auto find_forklifts(vector<string> &ranges, Graph &graph) -> long long {
  auto ans = 0ll;
  auto n = ranges.size();
  auto m = ranges[0].size();
  auto get_pos = [m](int i, int j) -> int { return i * m + j; };
  auto get_x_y = [m](int pos) -> tuple<int, int> { return {pos / m, pos % m}; };

  queue<int> q;

  // implement topological sort to find the order of removal of papers
  auto is_removeable = [&](int pos) -> bool {
    auto [x, y] = get_x_y(pos);
    if (ranges[x][y] == '.') {
      return false;
    }
    return graph[pos].size() < kThreshold;
  };

  auto remove_node = [&](int pos) -> void {
    auto [x, y] = get_x_y(pos);
    ranges[x][y] = '.';
    // now for the neighbours we need to remove this node from their adjacency
    // list
    for (auto neighbour : graph[pos]) {
      graph[neighbour].erase(pos);

      if (is_removeable(neighbour)) {
        q.push(neighbour);
      }
    }
  };

  // pre-fill the queue with all the nodes that are removeable
  for (int idx = 0; idx < graph.size(); ++idx) {
    if (is_removeable(idx)) {
      q.push(idx);
    }
  }

  while (!q.empty()) {
    auto pos = q.front();
    q.pop();

    auto [x, y] = get_x_y(pos);
    if (ranges[x][y] == '.') { // already removed
      continue;
    }

    remove_node(pos);
    ans++;
  }

  return ans;
}

auto main() -> int {
  auto ranges = read_input();
  auto graph = get_graph(ranges);
  auto ans = find_forklifts(ranges, graph);
  std::println("{}", ans);
  return 0;
}