#include <iostream>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

int main() {
    int sum = 0;
    int num_cards = 0;
    vector<int> num_matches;

    // Parsing and Part 1 Logic
    string card;
    while (getline(cin, card)) {
        istringstream sin(card);

        set<string> winning_numbers;
        int card_points = 0;
        int matches = 0;
        string str;
        bool win_nums = true;
        bool first_match = true;

        // Ignore "Card #:"
        sin >> str >> str;

        while (sin >> str) {
            if (str == "|") {
                win_nums = false;
                continue;
            };

            if (win_nums) {
               winning_numbers.insert(str); 
               continue;
            }
            
            if (winning_numbers.find(str) == winning_numbers.end()) {
                continue;
            }

            matches += 1;
            if (first_match) {
                card_points = 1;
                first_match = false;
            }
            else {
                card_points *= 2;
            }
        }
        
        sum += card_points;
        num_cards += 1;
        num_matches.push_back(matches);
    }

    // Part 2 Logic
    vector<int> total_num_cards(num_cards, 1);
    for (int i = 0; i < num_matches.size(); i++) {
        for (int j = i + 1; j < i + num_matches[i] + 1; j++) {
            if (j >= total_num_cards.size()) break;
            total_num_cards[j] += total_num_cards[i];;
        }
    }
    int total_cards = accumulate(total_num_cards.begin(), total_num_cards.end(), 0);

    // Output
    cout << "Part 1" << endl;
    cout << "Sum of all card points is: " << sum << endl;
    cout << "Part 2" << endl;
    cout << "Total number of cards is: " << total_cards << endl;
}
