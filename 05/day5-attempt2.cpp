#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

struct MapRange {
    int64_t src_start;
    int64_t dst_start;
    int64_t dst_end;

    MapRange(int64_t src, int64_t dst, int64_t size) {
        this->src_start = src;
        this->dst_start = dst;
        this->dst_end = dst + size - 1;
    }
};

struct SeedRange {
    int64_t start;
    int64_t end;

    SeedRange(int64_t start, int64_t size) : start(start), end(start + size - 1) {}
};

int64_t get_seed(const vector<vector<MapRange>> &maps, vector<map<int64_t, int64_t>> &lookups, int64_t loc) {
    int64_t seed = loc;
    for (int i = 6; i >= 0; i--) {
        // Lookup if intermediary calculation has been done before.
        // if (i != 0) {
        //     auto it = lookups[i - 1].find(seed);
        //     if (it != lookups[i - 1].end()) {
        //         seed = it->second;
        //         continue;
        //     }
        // }
        

        int64_t inter_val = seed;
        for (MapRange range : maps[i]) {
            if (seed < range.dst_start || seed > range.dst_end) continue;

            seed = range.src_start + (seed - range.dst_start);
            break;
        }

        // Store intermediary calculation.
        // if (i != 0) {
        //     lookups[i - 1].insert(pair<int64_t, int64_t>(inter_val, seed));
        // }
    }

    return seed;
}

int main() {
    vector<int64_t> seeds;
    vector<SeedRange> seed_ranges;
    vector<vector<MapRange>> maps(7);

    // Lookup table stores intermediary calculations such as
    // FERT - WATER, preventing unnecessary calculations.
    vector<map<int64_t, int64_t>> lookups(6);
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

        // Store maps.
        int64_t src, dst, size;
        sin >> dst >> src >> size;
        maps[parsing_section].push_back(MapRange(src, dst, size));
    }

    // Part 1 Logic
    // Find least valued location out of all seeds.
    // int64_t min_1 = 0;
    // while (true) {
    //     int64_t seed = get_seed(maps, lookups, min_1);
    //     cout << "Loc: " << min_1 << " Seed: " << seed << endl << flush;
    //     if (find(seeds.begin(), seeds.end(), seed) != seeds.end()) {
    //         break;
    //     }

    //     min_1++;
    // }

    // Part 2 Logic
    // Find least valued location out of all seed ranges.
    int64_t min_2 = 0;
    while (true) {
        int64_t seed = get_seed(maps, lookups, min_2);
        cout << "Loc: " << min_2 << " Seed: " << seed << endl;
        if (any_of(seed_ranges.begin(), seed_ranges.end(), [seed](const SeedRange &sr) {
            return seed >= sr.start && seed <= sr.end;
        })) break;

        min_2++;
    }

    // cout << "Part 1" << endl;
    // cout << "Lowest location number is: " << min_1 << endl;
    cout << "Part 2" << endl;
    cout << "Lowest location number is: " << min_2 << endl;
}
