#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <chrono>
#include "MapIndex.h"
#include "UnorderedMapIndex.h"

using namespace std;

int main()
{
    cout << "************************************************" << endl;      //print proj name + authors
    cout << "*                                              *" << endl;
    cout << "*          AUTOMATIC INDEX GENERATOR           *" << endl;
    cout << "*                                              *" << endl;
    cout << "*   Created by: Lauren Lanfair & Rohan Singh   *" << endl;
    cout << "************************************************" << endl;
    cout << "\nChoose file to open: " << endl;                              //which file to index
    cout << "   1. Moby Dick by Herman Melville \n"
            "   2. David Copperfield by Charles Dickens \n";
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
    vector<pair<string, int>> words;             //vector of words: word -> line found
    vector<pair<string, int>> chapters;          //vector of chapters: "chapter" + number -> line found
    bool newLine;

    if(!file.is_open()){
        cout << "file not open. check working directory in ide" << endl;
    }

    if(file.is_open()) {
        getline(file, title);
        getline(file, line);                //empty line
        getline(file, author);
        getline(file, line);                //empty line
        lineNum += 4;                              //increment line counter for lines gotten

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
                    else w += char(tolower(word[j]));                  //makes all lowercase
                }
                if(w.find(' ')){                      //if string is multiple words (after punctuation is removed)
                    istringstream stream2(w);         //separate it and add each word to the vector
                    string w2;
                    while(stream2 >> w2){
                        words.push_back(make_pair(w2, lineNum));    //add to words vector
                    }
                }
                else words.push_back(make_pair(w, lineNum));        //add to words vector
            }
            lineNum++;
        }
    }
    file.close();

    int max = 0;                                        //this section for formatting purposes
    for(auto & w : words){
        if(w.first.length() > max){
            max = int(w.first.length());
        }
    }
    max++;

    cout << "************************************************" << endl;

    /* ===============MAP INDEX FILE CREATION + TIMER=============== */

    auto start = chrono::high_resolution_clock::now();   //start timer for MapIndex.txt creation
    map<string, set<int>> indexMap = MapIndex(words).getMap();

    /*_CREATE FILE FROM MAP_*/
    ofstream mapFile;
    mapFile.open("src/MapIndex.txt");                //print title, author, word count, and unique word count
    mapFile << "TITLE: " << title << "\n";
    mapFile << "AUTHOR: " << author << "\n";
    mapFile << "WORD COUNT: " << words.size() << "\n";
    mapFile << "NUMBER OF UNIQUE WORDS: " << indexMap.size() << "\n";

    mapFile << setfill('-') << setw(max + max + 8) << "\n";
    mapFile << setfill(' ');

    mapFile << "CHAPTER INDEX:\nCHAPTER" << setw(max - 3) << "| LINE" << "\n";
    mapFile << setfill('-') << setw(max + max + 8) << "\n";
    mapFile << setfill(' ');

    for(auto& c : chapters){                                //print out chapters + lines
        mapFile << c.first << setw(int(max - c.first.length())) << ": " << c.second << "\n";
    }

    mapFile << setfill('-') << setw(max + max + 8) << "\n";
    mapFile << setfill(' ');

    mapFile << "INDEX:\nWORD" << setw(max) << "| LINE" << "\n";
    mapFile << setfill('-') << setw(max + max + 8) << "\n";
    mapFile << setfill(' ');


    for(auto& i : indexMap){                                    //print words + line numbers
        mapFile << i.first << setw(int(max - i.first.length())) << ": ";
        for(auto j : i.second){
            mapFile << j << " ";
        }
        mapFile << "\n";
    }

    auto stop = chrono::high_resolution_clock::now();                 //stop timer
    auto duration = chrono::duration_cast<chrono::nanoseconds>(stop - start);


    /* ===============UNORDERED_MAP INDEX FILE CREATION + TIMER=============== */

    auto start2 = chrono::high_resolution_clock::now();  //start timer for UnorderedMapIndex.txt creation
    UnorderedMapIndex wordIdx = UnorderedMapIndex(words);
    unordered_map<string, set<int>> unorderedMapIndex = UnorderedMapIndex(words).getMap();
    vector<string> orderedKeys = wordIdx.getKeys();                     //call to sort function

    /*_CREATE FILE FROM UNORDERED MAP_*/
    ofstream unorderedMapFile;
    unorderedMapFile.open("src/UnorderedMapIndex.txt");  //print title, author, word count, and unique word count
    unorderedMapFile << "TITLE: " << title << "\n";
    unorderedMapFile << "AUTHOR: " << author << "\n";
    unorderedMapFile << "WORD COUNT: " << words.size() << "\n";
    unorderedMapFile << "NUMBER OF UNIQUE WORDS: " << unorderedMapIndex.size() << "\n";

    unorderedMapFile << setfill('-') << setw(max + max + 8) << "\n";
    unorderedMapFile << setfill(' ');

    unorderedMapFile << "CHAPTER INDEX:\nCHAPTER" << setw(max - 3) << "| LINE" << "\n";
    unorderedMapFile << setfill('-') << setw(max + max + 8) << "\n";
    unorderedMapFile << setfill(' ');

    for(auto& c : chapters){                                    //print chapters + lines
        unorderedMapFile << c.first << setw(int(max - c.first.length())) << ": " << c.second << "\n";
    }

    unorderedMapFile << setfill('-') << setw(max + max + 8) << "\n";
    unorderedMapFile << setfill(' ');

    unorderedMapFile << "INDEX:\nWORD" << setw(max) << "| LINE" << "\n";
    unorderedMapFile << setfill('-') << setw(max + max + 8) << "\n";
    unorderedMapFile << setfill(' ');

    for(auto& i : orderedKeys){              //iterate through ordered keys, find values in unordered_map
        unorderedMapFile << i << setw(int(max - i.length())) << ": ";   //print words + line numbers
        for(auto j : unorderedMapIndex[i]){
            unorderedMapFile << j << " ";
        }
        unorderedMapFile << "\n";
    }

    auto stop2 = chrono::high_resolution_clock::now();          //stop timer
    auto duration2 = chrono::duration_cast<chrono::nanoseconds>(stop2 - start2);

    /* ===============END OF FILE CREATIONS=============== */

    cout << "Files created in src folder: " << endl;                       //display files created
    cout << "       'MapIndex.txt'\n"
            "       'UnorderedMapIndex.txt'" << endl;
    cout << "************************************************" << endl;

    //print durations for creation of each file
    cout << "\nTIME FOR MAPINDEX CREATION:          " << duration.count() << " nanoseconds" << endl;
    cout << "TIME FOR UNORDEREDMAPINDEX CREATION: " << duration2.count() << " nanoseconds" << endl;
    cout << "\n************************************************" << endl;

    cout << "\nSearch for a word (or enter 0 to exit program): " << endl;     //search feature, 0 to end program
    string inputWord;                                                         //(using unordered map bc faster search)
    cin >> inputWord;

    if (inputWord == "0") {         //end program
        return 0;
    }
    if (unorderedMapIndex.find(inputWord) == unorderedMapIndex.end()) {     //if word is not in index
        cout << "'" << inputWord << "' not found in index!" << endl;
    }
    else {
        cout << "************************************************" << endl;
        //print word, number of instances, and lines found on
        cout << "\n'" << inputWord << "' was found " << unorderedMapIndex[inputWord].size() << " time(s)." << endl;
        cout << "Found on lines: ";
        for (auto i: unorderedMapIndex[inputWord]) {
            cout << i << " ";
        }
    }
    cout << "\n\n************************************************" << endl;

    /* ======TIMES FOR SEARCH (separate so no double printing of line numbers)====== */
    auto startSearch1 = chrono::high_resolution_clock::now();
    indexMap.find(inputWord);
    auto stopSearch1 = chrono::high_resolution_clock::now();
    auto durationSearch1 = chrono::duration_cast<chrono::nanoseconds>(stopSearch1 - startSearch1);
    cout << "\nTIME FOR MAP SEARCH:           " << durationSearch1.count() << " nanoseconds" << endl;

    auto startSearch2 = chrono::high_resolution_clock::now();
    unorderedMapIndex.find(inputWord);
    auto stopSearch2 = chrono::high_resolution_clock::now();
    auto durationSearch2 = chrono::duration_cast<chrono::nanoseconds>(stopSearch2 - startSearch2);
    cout << "TIME FOR UNORDERED_MAP SEARCH: " << durationSearch2.count() << " nanoseconds" << endl;

    cout << "\n************************************************" << endl;

    //yay done :)
    return 0;
}
