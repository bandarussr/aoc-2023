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

void transform_ranges(vector<Range> &seeds, const vector<pair<Range, Range>> &map) {
    vector<Range> transformed;
    for (auto seed : seeds) {
        for (auto range : map) {
            Range new_range = seed;
            if (seed.start > range.first.end || seed.end < range.first.start) continue;

            if (seed.start >= range.first.start) {
                new_range.start = range.second.start + (seed.start - range.first.start);
            }
            else {
                new_range.start = range.second.start;
                transformed.push_back(Range(seed.start, range.first.start - seed.start));
            }


            if (seed.end <= range.first.end) {
                new_range.end = range.second.start + (seed.end - range.first.start);
            }
            else {
                new_range.end = range.second.end;
                transformed.push_back(Range(range.first.end + 1, seed.end - range.first.end));
            }

            transformed.push_back(new_range);
        }
    }

    seeds = transformed;
}

void condense_ranges(vector<Range> &ranges) {
    sort(ranges.begin(), ranges.end(), [](const Range& a, const Range& b) {
        return a.start < b.start;
    });

    for (int i = 0; i < ranges.size(); i++) {
        if (i + 1 < ranges.size() && ranges[i].end + 1 >= ranges[i + 1].start) {
            ranges[i].end = std::max(ranges[i].end, ranges[i + 1].end);
            ranges.erase(ranges.begin() + i + 1);
            i--;
        }
    }
}

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
            condense_ranges(seed_ranges);

            continue;
        }

        // Store maps.
        uint64_t src, dst, size;
        sin >> dst >> src >> size;
        maps[parsing_section].push_back(pair(Range(src, size), Range(dst, size)));
    }

    // Part 1
    for (auto map : maps) {
        transform_ranges(seeds, map);
    }
    condense_ranges(seeds);

    for (auto seed : seeds) {
        cout << "[" << seed.start << ", " << seed.end << "], ";
    }
    cout << endl;
    
    // Part 2
    for (auto map : maps) {
        transform_ranges(seed_ranges, map);
    }
    condense_ranges(seed_ranges);

    for (auto seed : seed_ranges) {
        cout << "[" << seed.start << ", " << seed.end << "], ";
    }
    cout << endl;
}
