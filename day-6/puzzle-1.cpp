#include <bits/stdc++.h>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/multiprecision/cpp_int.hpp>

#include <string_view>
#include <vector>

using namespace std;

static constexpr std::string_view file_name = "input.txt";

using NumT = unsigned long long;

using Op = enum {
  kAdd,
  kMul,
};

using OpVec = tuple<vector<NumT>, Op>;

auto read_input() -> std::vector<OpVec> {
  auto output = std::vector<OpVec>();
  std::ifstream file(file_name.data());
  std::string line;
  vector<vector<NumT>> nums;
  vector<Op> ops;
  while (std::getline(file, line)) {
    vector<string> tokens;
    boost::algorithm::split(tokens, line, boost::is_any_of(" "));
    // filter out any blank/empty tokens
    tokens.erase(std::remove_if(
                     tokens.begin(), tokens.end(),
                     [](auto &token) { return token.empty() || token == " "; }),
                 tokens.end());

    auto nums_vec = vector<NumT>();
    bool is_num = true;
    for (auto &token : tokens) {
      // std::println("{}", token);
      if (token == "+") {
        ops.push_back(kAdd);
        is_num = false;
      } else if (token == "*") {
        ops.push_back(kMul);
        is_num = false;
      } else {
        if (is_num) {
          nums_vec.push_back(std::stoll(token));
        }
      }
    }
    if (is_num) {
      nums.push_back(std::move(nums_vec));
    }
  }

  auto num_cols = nums[0].size();
  assert(num_cols == ops.size());
  for (auto i = 0; i < num_cols; ++i) {
    auto nums_vec = vector<NumT>();
    nums_vec.reserve(nums.size());
    for (auto j = 0; j < nums.size(); ++j) {
      nums_vec.push_back(nums[j][i]);
    }
    output.push_back(std::make_tuple(std::move(nums_vec), ops[i]));
  }
  return output;
}

auto calculate(auto &sheet) -> NumT {
  NumT ans = 0;
  for (auto &[nums, op] : sheet) {
    NumT value;
    if (op == kAdd) {
      value =
          std::accumulate(nums.begin(), nums.end(), NumT(0), std::plus<NumT>());
    } else {
      value = std::accumulate(nums.begin(), nums.end(), NumT(1),
                              std::multiplies<NumT>());
    }
    ans += value;
  }
  return ans;
}

auto main() -> int {
  auto sheet = read_input();
  auto ans = calculate(sheet);
  std::cout << ans << std::endl;
  return 0;
}