#include "boost/algorithm/string/trim.hpp"
#include <algorithm>
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <ranges>
#include <string_view>
#include <vector>

using namespace std;

static constexpr std::string_view file_name = "input.txt";

struct Board {
  int n;
  vector<int> config;
  vector<vector<int>> switches;
  vector<int> extra;

  friend ostream &operator<<(ostream &os, const Board &board) {
    os << "Board(n=" << board.n << ", config={";
    for (auto &ch : board.config) {
      os << ch << ", ";
    }
    os << "}, switches={";
    for (auto &sw_indices : board.switches) {
      os << "(";
      for (auto &index : sw_indices) {
        os << index << ", ";
      }
      os << "), ";
    }
    os << "}, extra={";
    for (auto &ch : board.extra) {
      os << ch << ", ";
    }
    os << "})";
    return os;
  }
};

// [.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
// [...#.] (0,2,3,4) (2,3) (0,4) (0,1,2) (1,2,3,4) {7,5,12,7,2}
// [.###.#] (0,1,2,3,4) (0,3,4) (0,1,2,4,5) (1,2) {10,11,11,5,10,5}
auto read_input() -> std::vector<Board> {
  auto output = std::vector<Board>();
  std::ifstream file(file_name.data());
  std::string line;
  auto parse_config = [](const std::string &config_str) -> vector<int> {
    auto config = vector<int>();
    for (auto &ch : config_str) {
      if (ch == '.') {
        config.push_back(0);
      } else {
        config.push_back(1);
      }
    }
    return config;
  };

  auto parse_switches =
      [](const std::string &switches_str) -> vector<vector<int>> {
    auto switches = vector<vector<int>>();
    auto tokens = vector<string>();
    // each switch is a list of indices under (parentheses)
    boost::algorithm::split(tokens, switches_str, boost::is_any_of(")"));
    for (auto &token : tokens) {
      boost::trim(token);
      if (token.size() == 0) {
        continue;
      }
      boost::trim_left_if(token, boost::is_any_of("("));
      auto switch_indices = vector<int>();
      auto indices_str = token;
      boost::trim(indices_str);
      if (indices_str.size() == 0) {
        continue;
      }
      auto indices_tokens = vector<string>();
      boost::algorithm::split(indices_tokens, indices_str,
                              boost::is_any_of(","));
      for (auto &index_str : indices_tokens) {
        switch_indices.push_back(std::stoi(index_str));
      }
      switches.push_back(switch_indices);
    }
    return switches;
  };

  auto parse_extra = [](auto &extra_str) -> vector<int> {
    // remove { and } from the string
    boost::trim(extra_str);
    extra_str = extra_str.substr(1, extra_str.size() - 2);
    auto extra = vector<int>();
    auto tokens = vector<string>();
    boost::algorithm::split(tokens, extra_str, boost::is_any_of(","));
    for (auto &token : tokens) {
      extra.push_back(std::stoi(token));
    }
    return extra;
  };

  while (std::getline(file, line)) {
    // parse until the first ]
    Board board;
    auto pos = line.find(']');
    auto config_str = line.substr(1, pos - 1);
    board.n = config_str.size();
    board.config = parse_config(config_str);
    auto remaining_str = line.substr(pos + 1);
    // find the last )
    auto last_paren_pos = remaining_str.rfind(')');
    auto switches_str = remaining_str.substr(0, last_paren_pos + 1);
    board.switches = parse_switches(switches_str);

    auto extra_str = remaining_str.substr(last_paren_pos + 1);
    board.extra = parse_extra(extra_str);
    output.push_back(board);
  }
  return output;
}

auto solve_board(const Board &board) -> int {
  auto n = board.n;
  auto target_mask = 0ll;
  for (auto [i, ch] : std::views::enumerate(board.config)) {
    if (ch == 1) {
      target_mask |= (1ll << i);
    }
  }
  vector<int> sw_masks;
  for (const auto &sw : board.switches) {
    auto mask = 0ll;
    for (const auto &index : sw) {
      mask |= (1ll << index);
    }
    sw_masks.push_back(mask);
  }

  auto num_switches = board.switches.size();
  auto max_mask = (1ll << num_switches) - 1;

  vector<int> final_mask(max_mask + 1);
  final_mask[0] = 0;
  auto ans = num_switches + 1ll;
  for (auto mask = 1ll; mask <= max_mask; ++mask) {
    auto count = 0ull;
    for (auto i = 0; i < num_switches; ++i) {
      if ((mask & (1ll << i)) != 0) {
        count++;
        final_mask[mask] ^= sw_masks[i];
      }
    }

    if (final_mask[mask] == target_mask) {
      ans = min(ans, count);
    }
  }

  return ans;
}

auto solve(auto &boards) {
  auto ans = 0;
  for (auto &board : boards) {
    ans += solve_board(board);
  }
  return ans;
}

auto main() -> int {
  auto boards = read_input();
  auto ans = solve(boards);
  std::println("{}", ans);
  return 0;
}