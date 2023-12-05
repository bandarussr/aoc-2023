#include <algorithm>
#include <iostream>
#include <map>
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

enum ParsingSection {
    SEEDS = -1,
    SEED_TO_SOIL,
    SOIL_TO_FERT,
    FERT_TO_WATER,
    WATER_TO_LIGHT,
    LIGHT_TO_TEMP,
    TEMP_TO_HUMID,
    HUMID_TO_LOC
};

int main() {
    vector<int64_t> seeds;
    vector<vector<MapRange>> maps(7);
    string line, ignore;

    // Parsing
    ParsingSection ps = SEEDS;
    bool header = false;
    while (getline(cin, line)) {
        istringstream sin(line);
        

        if (header) {
            header = false;
            continue;
        }

        if (line == "") {
            ps = static_cast<ParsingSection>(ps + 1);
            header = true;
            continue;
        }

        if (ps == SEEDS) {
            sin >> ignore; // Ignore 'seeds:'
            int64_t seed;
            while (sin >> seed) {
                seeds.push_back(seed);
            }
            continue;
        }

        // Store maps into maps.
        int64_t src, dst, rng;
        sin >> dst >> src >> rng;
        maps[ps].push_back(MapRange(src, dst, rng));
    }

    // Part 1 Logic
    // Find least valued location out of all seeds.
    for (int64_t &seed : seeds) {
        for (int i = 0; i < 7; i++) {
            for (MapRange range : maps[i]) {
                if (seed < range.src_start || seed > range.src_end) continue;

                seed = range.dst_start + (seed - range.src_start);
                break;
            }
        }
    }

    cout << "Part 1" << endl;
    cout << "Lowest location number is: " << *min_element(seeds.begin(), seeds.end()) << endl;
}
