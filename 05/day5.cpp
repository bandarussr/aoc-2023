#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

/**
 * The ranges added using lines marked with A are not being checked b/c of issue with logic, but
 * for whatever reason, if I comment them, I get the correct answer for my input, but if I uncomment
 * them I get the correct answer for the example.
*/

struct Range {
    uint64_t start;
    uint64_t end;

    Range(uint64_t start, uint64_t size) : start(start), end(start + size - 1) {}
};

void transform_ranges(vector<Range> &seeds, const vector<pair<Range, Range>> &map) {
    vector<Range> transformed;
    for (auto seed : seeds) {
        int prev_size = transformed.size();

        for (auto range : map) {
            Range new_range = seed;
            // No overlap with map range.
            if (seed.start > range.first.end || seed.end < range.first.start) continue;

            // Handle start value.
            if (seed.start >= range.first.start) {
                new_range.start = range.second.start + (seed.start - range.first.start);
            }
            else {
                new_range.start = range.second.start;
                transformed.push_back(Range(seed.start, range.first.start - seed.start)); // A
            }

            // Handle end value.
            if (seed.end <= range.first.end) {
                new_range.end = range.second.start + (seed.end - range.first.start);
            }
            else {
                new_range.end = range.second.end;
                transformed.push_back(Range(range.first.end + 1, seed.end - range.first.end)); // A
            }

            transformed.push_back(new_range);
        }
        
        // If range was not transformed, put it back as is.
        if (prev_size == transformed.size()) {
            transformed.push_back(seed);
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
                seeds.push_back(Range(seed, 1));
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
        maps[parsing_section].push_back(make_pair(Range(src, size), Range(dst, size)));
    }

    // Part 1 & 2
    for (auto map : maps) {
        transform_ranges(seeds, map);
        transform_ranges(seed_ranges, map);
    }
    condense_ranges(seeds);
    condense_ranges(seed_ranges);

    cout << "Part 1" << endl;
    cout << "Lowest location number is: " << seeds[0].start << endl;
    cout << "Part 2" << endl;
    cout << "Lowest location number is: " << seed_ranges[0].start << endl;
}
