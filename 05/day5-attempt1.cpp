#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

struct MapRange {
    int64_t src_start;
    int64_t src_end;
    int64_t dst_start;

    MapRange(int64_t src, int64_t dst, int64_t size) {
        this->src_start = src;
        this->src_end = src + size - 1;
        this->dst_start = dst;
    }
};

struct SeedRange {
    int64_t start;
    int64_t end;

    SeedRange(int64_t start, int64_t size) : start(start), end(start + size - 1) {}
};

int64_t get_loc(const vector<vector<MapRange>> &maps, vector<unordered_map<int64_t, int64_t>> &lookups, int64_t seed) {
    // Lookup if calculation has been done before.
    auto it = lookups[0].find(seed);
    if (it != lookups[0].end()) {
        return it->second;
    }

    // Calculate if location is not in lookup table.
    int64_t loc = seed;
    for (int i = 0; i < 7; i++) {
        // Lookup if intermediary calculation has been done before.
        if (i != 0) {
            auto inter_it = lookups[i].find(loc);
            if (inter_it != lookups[i].end()) {
                loc = inter_it->second;
                continue;
            }
        }
        

        int64_t inter_val = loc;
        for (MapRange range : maps[i]) {
            if (loc < range.src_start || loc > range.src_end) continue;

            loc = range.dst_start + (loc - range.src_start);
            break;
        }

        if (i != 0) {
            // Store intermediary calculation.
            lookups[i].insert(pair<int64_t, int64_t>(inter_val, loc));
        }
        
    }

    lookups[0].insert(pair<int64_t, int64_t>(seed, loc));
    return loc;
}

int main() {
    vector<int64_t> seeds;
    vector<SeedRange> seed_ranges;
    vector<vector<MapRange>> maps(7);

    // Lookup table stores overall SEED - LOC calculations and
    // intermediary calculations such as FERT - WATER, preventing
    // unnecessary calculations.
    vector<unordered_map<int64_t, int64_t>> lookups(7);
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
            int64_t seed;
            while (sin >> seed) {
                seeds.push_back(seed);
            }

            // Parse seed ranges for part 2.
            for (int i = 0; i < seeds.size(); i += 2) {
                seed_ranges.push_back(SeedRange(seeds[i], seeds[i + 1]));
            }
            continue;
        }

        // Store maps into maps.
        int64_t src, dst, rng;
        sin >> dst >> src >> rng;
        maps[parsing_section].push_back(MapRange(src, dst, rng));
    }

    // Part 1 Logic
    // Find least valued location out of all seeds.
    int64_t min_1 = -1;
    for (int64_t seed : seeds) {
        if (min_1 == -1) {
            min_1 = get_loc(maps, lookups, seed);
            continue;
        }
        min_1 = min(get_loc(maps, lookups, seed), min_1);
    }

    // Part 2 Logic
    // Find least valued location out of all seed ranges.
    int64_t min_2 = -1;
    vector<int64_t> locations;
    for (SeedRange sr : seed_ranges) {
        for (int i = sr.start; i <= sr.end; i++) {
            if (min_2 == -1) {
                min_2 = get_loc(maps, lookups, i);
                continue;
            }
            min_2 = min(get_loc(maps, lookups, i), min_2);
        }
    }

    cout << "Part 1" << endl;
    cout << "Lowest location number is: " << min_1 << endl;
    cout << "Part 2" << endl;
    cout << "Lowest location number is: " << min_2 << endl;
}
