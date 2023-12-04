#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool is_engine_part(vector<vector<char>> engine, int y, int start_x, int end_x) {
    bool engine_part = false;

    // Check left, left up, left down.
    if (start_x != 0) {
        engine_part |= engine[y][start_x - 1] != '.';
        engine_part |= (y != 0) && (engine[y - 1][start_x - 1] != '.');
        engine_part |= (y != engine.size() - 1) && (engine[y + 1][start_x - 1] != '.');
    }

    // Check right, right up, right down.
    if (end_x != engine[y].size() - 1) {
        engine_part |= engine[y][end_x + 1] != '.';
        engine_part |= (y != 0) && (engine[y - 1][end_x + 1] != '.');
        engine_part |= (y != engine.size() - 1) && (engine[y + 1][end_x + 1] != '.');
    }

    // Check above and below.
    for (int i = start_x; i <= end_x; i++) {
        engine_part |= (y != 0) && (engine[y - 1][i] != '.');
        engine_part |= (y != engine.size() - 1) && (engine[y + 1][i] != '.');
    }

    return engine_part;
}

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

int main() {
    vector<vector<char>> engine;

    // Parse engine schematic.
    string line;
    while (getline(cin, line)) {
        engine.push_back(vector<char>(line.begin(), line.end()));
    }

    // Part 1 Logic
    // Check for numbers and add if it is an engine part.
    string num;
    int start_x, end_x;
    int sum = 0;
    for (int y = 0; y < engine.size(); y++) {
        num = "";
        start_x = -1;

        for (int x = 0; x < engine[y].size(); x++) {
            char c = engine[y][x];
            if (is_digit(c)) {
                start_x = (start_x == -1) ? x : start_x;
                num += c;
                continue;
            }
            if (start_x == -1) continue;

            end_x = x - 1;
            if (is_engine_part(engine, y, start_x, end_x)) {
                sum += stoi(num);
            }

            num = "";
            start_x = -1;
        }

        if (num == "") continue;
        end_x = engine[y].size() - 1;
        if (is_engine_part(engine, y, start_x, end_x)) {
            sum += stoi(num);
        }
    }

    cout << "Part 1" << endl;
    cout << "Sum of engine part numbers is: " << sum << endl;

    // Part 2 Logic
    // Find each gear and multiply the two adjacent 
    // part numbers to get gear ratio if it exists.
    for (int y = 0; y < engine.size(); y++) {

    }

    cout << "Part 2" << endl;
    cout << "Sum of gear ratios is: " << 0 << endl;
}
