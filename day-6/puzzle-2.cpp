#include <bits/stdc++.h>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/join.hpp>
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

using OpVec = tuple<vector<string>, Op>;

struct ColumnRange {
  size_t start;
  size_t length;
};

auto infer_schema(const string &op_line) -> vector<ColumnRange> {
  vector<ColumnRange> schema;
  for (size_t i = 0; i < op_line.size(); ++i) {
    if (op_line[i] == '*' || op_line[i] == '+') {
      schema.push_back({i, 0}); // length filled in below
    }
  }
  for (size_t i = 0; i < schema.size(); ++i) {
    if (i + 1 < schema.size()) {
      schema[i].length = schema[i + 1].start - schema[i].start;
    } else {
      schema[i].length = op_line.size() - schema[i].start;
    }
  }
  return schema;
}

auto extract_columns_by_schema(const string &line,
                               const vector<ColumnRange> &schema)
    -> vector<string> {
  vector<string> columns;
  for (const auto &col : schema) {
    if (col.start < line.size()) {
      size_t len = min(col.length, line.size() - col.start);
      columns.push_back(line.substr(col.start, len));
    } else {
      columns.push_back("");
    }
  }
  return columns;
}

auto read_input() -> std::vector<OpVec> {
  auto output = std::vector<OpVec>();
  std::ifstream file(file_name.data());
  std::string line;

  vector<string> all_lines;
  string op_line;
  while (std::getline(file, line)) {
    if (line.find('*') != string::npos || line.find('+') != string::npos) {
      op_line = line;
    } else {
      all_lines.push_back(line);
    }
  }

  auto schema = infer_schema(op_line);

  vector<Op> ops;
  for (const auto &col : schema) {
    char c = op_line[col.start];
    ops.push_back(c == '*' ? kMul : kAdd);
  }

  vector<vector<string>> nums;
  for (const auto &data_line : all_lines) {
    nums.push_back(extract_columns_by_schema(data_line, schema));
  }

  auto num_cols = schema.size();
  assert(num_cols == ops.size());
  for (size_t i = 0; i < num_cols; ++i) {
    auto nums_vec = vector<string>();
    nums_vec.reserve(nums.size());
    for (size_t j = 0; j < nums.size(); ++j) {
      nums_vec.push_back(std::move(nums[j][i]));
    }
    output.push_back(std::make_tuple(std::move(nums_vec), ops[i]));
  }
  return output;
}

auto calculate(auto &sheet) -> NumT {
  NumT ans = 0;
  for (auto &[nums, op] : sheet) {
    auto column_size = nums[0].size();
    vector<NumT> column_nums;
    for (auto i = 0; i < column_size; ++i) {
      auto number = 0;
      for (auto &num : nums) {
        char ch = num[i];
        auto value = 0;
        if (isdigit(ch)) {
          value = ch - '0';
        } else {
          value = 0;
        }
        if (value) {
          number = number * 10 + value;
        }
      }
      if (number != 0) {
        column_nums.push_back(number);
      }
    }
    if (op == kAdd) {
      ans += std::accumulate(column_nums.begin(), column_nums.end(), NumT(0),
                             std::plus<NumT>());
    } else {
      ans += std::accumulate(column_nums.begin(), column_nums.end(), NumT(1),
                             std::multiplies<NumT>());
    }
  }
  return ans;
}

auto main() -> int {
  auto sheet = read_input();
  auto ans = calculate(sheet);
  std::cout << ans << std::endl;
  return 0;
}