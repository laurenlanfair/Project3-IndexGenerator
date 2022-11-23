#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "MapIndex.h"

using namespace std;

int main()
{
    cout << "Choose file to open: " << endl;
    cout << "1. Moby Dick by Herman Melville \n"
            "2. David Copperfield by Charles Dickens \n";
    string input;
    cin >> input;
    string filepath;
    if(input == "1"){
        filepath = "src/mobydick.txt";
    }
    else if(input == "2"){
        filepath = "src/davidcopperfield.txt";
    }
    else {
        cout << "Error - terminating program!" << endl;
        return 0;
    }
    ifstream file(filepath);

    string word;
    string line;
    string title;
    string author;
    int lineNum = 1;
    vector<pair<string, int>> words;
    vector<pair<string, int>> chapters;
    bool newLine = true;

    if(!file.is_open()){
        cout << "file not open. check working directory in ide" << endl;
    }

    if(file.is_open()) {
        getline(file, title);
        getline(file, line);                //empty line
        getline(file, author);
        getline(file, line);                //empty line
        lineNum += 4;

        while (getline(file, line)) {
            newLine = true;
            istringstream stream(line);

            while (stream >> quoted(word)) {
                if (newLine && (word == "Chapter" || word == "CHAPTER")) {      //if chapter, get chapter + chapter num
                    string number;
                    stream >> number;
                    word += " " + number;
                    chapters.push_back(make_pair(word, lineNum));      //push onto chapters vector
                    break;
                }
                newLine = false;
                string w;
                for (int j = 0; j < word.length(); j++) {
                    if (ispunct(word[j]) && word[j] != '/') {
                        word.erase(j--, 1);                 //removes punctuation
                    }
                    else w += tolower(word[j]);                  //makes all lowercase
                }
                if(w.find(' ')){                      //if string is multiple words (after punctuation is removed)
                    istringstream stream2(w);         //separate it and add each word to the vector
                    string w2;
                    while(stream2 >> w2){
                        words.push_back(make_pair(w2, lineNum));
                    }
                }
                else words.push_back(make_pair(w, lineNum));
            }
            lineNum++;
        }
    }
    file.close();

    auto start = chrono::high_resolution_clock::now();
    map<string, set<int>> indexMap = MapIndex(words).getMap();
    auto stop = chrono::high_resolution_clock::now();
    auto duration = duration_cast<chrono::microseconds>(stop - start);
    cout << "TIME FOR MAP CREATION: " << duration.count() << " microseconds" << endl;


/* Here I just created sample outputs, one directly from the vector and then one from my map
   so I could see what was going on */

//    ofstream index;
//    index.open("src/NotRealIndex.txt");
//
//    index << "TITLE: " << title << "\n";
//    index << "AUTHOR: " << author << "\n";
//    index << "WORD COUNT: " << words.size() << "\n";
//    index << "Word, Line: " << "\n";
//
//    for(auto& i : words){
//        index << i.first << ", " << i.second << "\n";
//    }
//
//
//
//    ofstream otherindex;
//    otherindex.open("src/MAPINDEX.txt");
//    otherindex << "TITLE: " << title << "\n";
//    otherindex << "AUTHOR: " << author << "\n";
//    otherindex << "WORD COUNT: " << words.size() << "\n";
//    otherindex << "Chapter Index: " << "\n";
//
//    for(auto& c : chapters){
//        otherindex << c.first << " : " << c.second << "\n";
//    }
//
//    otherindex << "Word Index: " << "\n";
//
//    for(auto& i : indexMap){
//        otherindex << i.first << " : ";
//        for(auto j : i.second){
//            otherindex << j << " ";
//        }
//        otherindex << "\n";
//    }

    return 0;
}
