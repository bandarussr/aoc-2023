#include <algorithm>
#include <iostream>
#include <map>
#include <string>
using namespace std;

int main() {
    map<string, string> word_digits = {
        {"zero", "0"},
        {"one", "1"},
        {"two", "2"},
        {"three", "3"},
        {"four", "4"},
        {"five", "5"},
        {"six", "6"},
        {"seven", "7"},
        {"eight", "8"},
        {"nine", "9"}
    };

    auto is_ascii_digit = [](char c) {
        return c >= '0' && c <= '9';
    };

    auto get_word_digit = [word_digits](string word, bool backward) -> string {
        int start = (backward) ? word.size() - 1 : 0;
        int stop = (backward) ? -1 : word.size();
        int step = (backward) ? -1 : 1;

        for (int i = start; i != stop; i += step) {
            for (size_t j = 3; j <= min(word.size(), (size_t)5); j++) {
                auto it = word_digits.find(word.substr(i, j));
                if (it == word_digits.end()) continue;
                return it->second; 
            }
        }

        return "";
    };

    int sum = 0;
    string line;
    while (cin >> line) {
        string number = "xx";
        string word = "";

        for (auto &c : line) {
            if (is_ascii_digit(c)) {
                number[0] = c;
                break;
            }
            word += c;
        }

        string word_digit = get_word_digit(word, false);
        if (word_digit != "") {
            number[0] = word_digit[0];
        }

        word = "";
        reverse(line.begin(), line.end());
        for (auto &c : line) {
            if (is_ascii_digit(c)) {
                number[1] = c;
                break;  
            }
            
            word = c + word;
        }

        word_digit = get_word_digit(word, true);
        if (word_digit != "") {
            number[1] = word_digit[0];
        }
        
        sum += stoi(number);
    }
    
    cout << "Sum of calibration values is: " << sum << endl;
}
