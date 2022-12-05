#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class UnorderedMapIndex {
private:
    unordered_map<string, set<int>> index;
    vector<string> orderedKeys;
public:
    UnorderedMapIndex(vector<pair<string, int>>& words);
    unordered_map<string, set<int>> getMap();
    vector<string> getKeys();
    void printIndex();
};

UnorderedMapIndex::UnorderedMapIndex(vector<pair<string, int>>& words) {
    this-> index = unordered_map<string, set<int>>();
    
    for(auto& word : words){
        if (this->index.find(word.first) != this->index.end()) {
            index[word.first].insert(word.second);
        }
        else {
            index[word.first] = set<int>();
            index[word.first].insert(word.second);
        }
    }
}

unordered_map<string, set<int>> UnorderedMapIndex::getMap() {
    return index;
}

vector<string> UnorderedMapIndex::getKeys() {
    unordered_map<string, set<int>>::iterator it;
    orderedKeys.clear();

    for (it = index.begin(); it != index.end(); it++) {
        orderedKeys.push_back(it->first);
    }

    sort(orderedKeys.begin(), orderedKeys.end());

    return orderedKeys;

}

void UnorderedMapIndex::printIndex() {
    for (auto& [word, lineNumbers] : this->index) {
        cout << word << " ";
        for (auto& line : lineNumbers) {
            cout << line << " ";
        }
        cout << endl;
    }
}
