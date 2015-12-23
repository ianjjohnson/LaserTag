#ifndef MAIN_H
#define MAIN_H

/*
 Map of every player (on both teams).
    - playerNumberDictionary[n].first is the name of the player whose ID number is n.
    - playerNumberDictionary[n].second is the team number of the player whose ID number is n.
 */
extern map<int, pair<string, int> > playerNumberDictionary;

//Total number of hits registered in the match text file
extern int totalHitCountInGame;

//The file stream used to write the output file describing the match
extern ofstream* outputFileStream;

/*
 Pointer to what will become an array of int pairs, where:
    - hitRegistry[n].first is the ID number of the player who was the shooter
    - hitRegistry[n].second is the ID number of the player who was hit
 Used for high-verbosity output. O(n) memory usage, but prevents having to parse the file multiple times
 */
extern pair<int, int> *hitRegistry;


//See main.cpp implementations for method descriptions

void registerPlayer(int number, string name, int teamNumber);

map<int, int> buildHitValuesMap();

void registerHit(string hitDataString, int registryIndex);

void processHitsFromFile(char* fileName);

void printOverallWinners();

void printResults(char* outputFileName, char* verbosityPreference);

#endif // MAIN_H

