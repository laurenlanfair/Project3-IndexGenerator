#include <string>
#include <map>
#include <set>
using namespace std;

class MapIndex {
private:
    map<string, set<int>> index;
public:
    MapIndex(vector<pair<string, int>>& words);
    map<string, set<int>> getMap();
};

MapIndex::MapIndex(vector<pair<string, int>>& words) {
    for(auto& word : words){
        index[word.first].insert(word.second);
    }
}

map<string, set<int>> MapIndex::getMap() {
    return index;
}
