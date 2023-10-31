#include "wordfrequency.h"
#include <fstream>
#include <iostream>
#include <algorithm>
/**
 * Assignment 3 for COSE213 Data Structures
 *
 * Won-Ki Jeong (wkjeong@korea.ac.kr)
 *
 * 2022. 5. 1
 *
 */

// used to parse pure alphabetical word from normal word
class MyStringStream {
  public:
    MyStringStream(std::string word) : data(word), len(word.size()){};

    std::string Next() {
        int start;
        bool state = false;
        while (pos < len) {
            if (isAlphabet(data[pos])) {
                if (!state) {
                    start = pos;
                    state = true;
                }
            } else if (state) {
                // finish search
                break;
            }
            pos++;
        }

        if (!state) {
            return "";
        }
        return data.substr(start, pos - start);
    }

  private:
    bool isAlphabet(char ch) {
        return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
    }
    int pos = 0;
    int len;
    std::string data;
};

WordFrequency::WordFrequency() { map = new WordFreqMap; }

WordFrequency::~WordFrequency() { delete map; }

void WordFrequency::ReadText(const char *filename) {
    std::ifstream file(filename);
    std::string buf;
    while (getline(file, buf)) {
        MyStringStream ss(buf);
        std::string word;
        while ((word = ss.Next()) != "") {
            std::transform(word.begin(), word.end(),  word.begin(), ::tolower);
            IncreaseFrequency(word);
        }
    }
}

int WordFrequency::GetFrequency(const std::string word) {
    auto found = map->find(word);
    if (found == nullptr) {
        return 0;
    }
    return found->val;
}

void WordFrequency::IncreaseFrequency(const std::string word) {
    auto found = map->find(word);
    if (found == nullptr) {
        map->insert(word, 1);
        return;
    }
    // increase
    map->insert(found->key, found->val + 1);
}