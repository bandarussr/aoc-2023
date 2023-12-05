#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

// Max cubes in the bag at a time.
const int max_red = 12;
const int max_green = 13;
const int max_blue = 14;

struct Set {
    Set() {
        this->red = 0;
        this->green = 0;
        this->blue = 0;
    }

    int red;
    int blue;
    int green;
};

struct Game {
    int id;
    bool possible;
    vector<Set> sets;
    Set max;
    int power;
};

int main() {
    vector<Game> games;

    string line;
    while (getline(cin, line)) {
        Game g;
        istringstream sin(line);

        // Get game id.
        string id;
        sin.ignore(5, ' ');
        getline(sin, id, ':');
        g.id = stoi(id);

        // Get sets.
        string set;
        while (getline(sin, set, ';')) {
            istringstream setin(set);

            Set s;
            int num;
            string color;
            while (setin >> num >> color) {
                if (color.find("red") != color.npos) {
                    s.red = num;
                }
                else if (color.find("blue") != color.npos) {
                    s.blue = num;
                }
                else if (color.find("green") != color.npos) {
                    s.green = num;
                }
            }

            g.sets.push_back(s);
        }
        games.push_back(g);
    }

    // Part 1 Logic
    // Check for impossible games.
    for (auto &g : games) {
        g.possible = true;
        for (auto s : g.sets) {
            if (s.red > max_red || s.green > max_green || s.blue > max_blue) {
                g.possible = false;
            }
        }
    }

    // Sum possible games' ID's.
    int sum = 0;
    for (auto g : games) {
        if (!g.possible) continue;
        sum += g.id;
    }
    cout << "Part 1" << endl;
    cout << "Sum of possible games' ID's is: " << sum << endl;

    // Part 2 Logic
    // Check for minimum number of cubes of each color needed for game to work.
    // Calculate power for each game.
    for (auto &g : games) {
        for (auto s : g.sets) {
            g.max.red = max(s.red, g.max.red);
            g.max.green = max(s.green, g.max.green);
            g.max.blue = max(s.blue, g.max.blue);
        }

        g.power = g.max.red * g.max.green * g.max.blue;
    }

    // Calculate sum of powers.
    sum = 0;
    for (auto g : games) {
        sum += g.power;
    }
    cout << "Part 2" << endl;
    cout << "Sum of powers of games is: " << sum << endl;
}
