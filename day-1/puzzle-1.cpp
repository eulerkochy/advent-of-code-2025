#include <bits/stdc++.h>

using namespace std;

auto main() -> int {
    auto current_position = 50;
    auto ans = 0; // num times current_position is 0

    std::string input_file = "input.txt";
    std::ifstream file(input_file);

    std::string line;
    while (std::getline(file, line)) {
        char c = line[0];
        int value = std::stoi(line.substr(1));
        int sign = (c == 'L') ? -1 : 1;
        current_position += sign * value;
        current_position %= 100;

        ans += (current_position == 0);
    }
    std::cout << ans << std::endl;
    return 0;
}
