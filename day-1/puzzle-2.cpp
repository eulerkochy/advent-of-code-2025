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
        
	int rotations = value / 100;
	value %= 100;

	ans += rotations;

	int old_position = current_position;	
	current_position += sign * value;
	auto did_crossover = (old_position != 0 && (current_position >= 100 || current_position <= 0)) ? 1: 0;

	current_position = (current_position + 100) % 100;

	
	
	ans += did_crossover;

    }
    std::cout << ans << std::endl;
    return 0;
}
