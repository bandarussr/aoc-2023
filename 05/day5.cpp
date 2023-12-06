#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

struct Range {
    uint64_t start;
    uint64_t end;

    Range(uint64_t start, uint64_t size) : start(start), end(start + size - 1) {}
};

int main() {
    vector<Range> seeds;
    vector<Range> seed_ranges;
    vector<vector<pair<Range, Range>>> maps(7);
    string line, ignore;

    // Parsing
    int parsing_section = -1;
    bool header = false;
    while (getline(cin, line)) {
        istringstream sin(line);

        if (header) {
            header = false;
            continue;
        }

        if (line == "") {
            parsing_section++;
            header = true;
            continue;
        }

        if (parsing_section == -1) {
            sin >> ignore; // Ignore 'seeds:'
            uint64_t seed;
            while (sin >> seed) {
                seeds.push_back(Range(seed, seed));
            }

            // Parse seed ranges for part 2.
            for (int i = 0; i < seeds.size(); i += 2) {
                seed_ranges.push_back(Range(seeds[i].start, seeds[i + 1].start));
            }

            // Condense ranges
            sort(seed_ranges.begin(), seed_ranges.end(), [](const Range& a, const Range& b) {
                return a.start < b.start;
            });

            for (int i = 0; i < seed_ranges.size(); i++) {
                if (i + 1 < seed_ranges.size() || seed_ranges[i].end + 1 < seed_ranges[i + 1].start) continue;

                seed_ranges[i].end = max(seed_ranges[i].end, seed_ranges[i + 1].end);
                seed_ranges.erase(seed_ranges.begin() + i + 1);
                i--;
            }

            continue;
        }

        // Store maps.
        uint64_t src, dst, size;
        sin >> dst >> src >> size;
        maps[parsing_section].push_back(pair(Range(src, size), Range(dst, size)));
    }
}
