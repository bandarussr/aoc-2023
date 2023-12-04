#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

bool is_engine_part(vector<vector<char>> engine, int num_y, int num_start_x, int num_end_x) {
    bool engine_part = false;

    // Create "window" around number.
    int start_y = (num_y == 0) ? 0 : num_y - 1;
    int end_y = (num_y == engine.size() - 1) ? num_y : num_y + 1;
    int start_x = (num_start_x == 0) ? 0 : num_start_x - 1;
    int end_x = (num_end_x == engine[0].size() - 1) ? num_end_x : num_end_x + 1;

    // Check all characters in "window".
    for (int y = start_y; y <= end_y; y++) {
        for (int x = start_x; x <= end_x; x++) {
            engine_part |= (engine[y][x] != '.') && !is_digit(engine[y][x]);
        }
    }

    return engine_part;
}

vector<vector<int>> get_gear_ratio_coords(vector<vector<char>> engine, int gear_y, int gear_x) {
    vector<vector<int>> gear_ratio_coords;

    // Create "window" around gear.
    int start_y = (gear_y == 0) ? 0 : gear_y - 1;
    int end_y = (gear_y == engine.size() - 1) ? gear_y : gear_y + 1;
    int start_x = (gear_x == 0) ? 0 : gear_x - 1;
    int end_x = (gear_x == engine[0].size() - 1) ? gear_x : gear_x + 1;

    // Check all characters in "window".
    for (int y = start_y; y <= end_y; y++) {
        // Prevent coordinates to the same number being stored again.
        bool adj = false;
        for (int x = start_x; x <= end_x; x++) {
            if (!is_digit(engine[y][x])) {
                adj = false;
                continue;
            }
            if (adj) continue;

            gear_ratio_coords.push_back(vector<int>({y, x}));
            adj = true;
        }
    }

    return gear_ratio_coords;
}

int get_gear_ratio_number(vector<vector<char>> engine, int y, int x) {
    string num = "";

    int i = x;
    while (true) {
        if (!is_digit(engine[y][i])) break;
        if (i < 0 || i >= engine[y].size()) break;
        num += engine[y][i];
        i++;
    }

    i = x - 1;
    while (true) {
        if (!is_digit(engine[y][i])) break;
        if (i < 0 || i >= engine[y].size()) break;
        num = engine[y][i] + num;
        i--;
    }

    return stoi(num);
}

int main() {
    vector<vector<char>> engine;

    // Parse engine schematic.
    string line;
    while (getline(cin, line)) {
        engine.push_back(vector<char>(line.begin(), line.end()));
    }

    // Part 1: Check for numbers and add if it is an engine part.
    // Part 2: Find each gear and multiply the two adjacent 
    //         part numbers to get gear ratio if it exists.
    string num;
    int start_x, end_x;
    int sum_parts = 0;
    int sum_ratios = 0;
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

            if (engine[y][x] == '*') {
                // Needs 2 numbers to calculate gear ratio.
                vector<vector<int>> gear_ratio_coords = get_gear_ratio_coords(engine, y, x);
                if (gear_ratio_coords.size() < 2) continue;

                // Calculate gear ratio.
                int gear_ratio = 1;
                for (auto i : gear_ratio_coords) {
                    gear_ratio *= get_gear_ratio_number(engine, i[0], i[1]);
                }
                sum_ratios += gear_ratio;
                continue;
            }

            if (start_x == -1) continue;

            end_x = x - 1;
            if (is_engine_part(engine, y, start_x, end_x)) {
                sum_parts += stoi(num);
            }

            num = "";
            start_x = -1;
        }

        if (num == "") continue;
        end_x = engine[y].size() - 1;
        if (is_engine_part(engine, y, start_x, end_x)) {
            sum_parts += stoi(num);
        }
    }

    cout << "Part 1" << endl;
    cout << "Sum of engine part numbers is: " << sum_parts << endl;
    cout << "Part 2" << endl;
    cout << "Sum of gear ratios is: " << sum_ratios << endl;
}
