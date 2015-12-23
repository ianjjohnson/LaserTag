#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <stdlib.h>

#include "team.h"
#include "main.h"

using namespace std;

//Declaring variables here because the 'extern' tag was used in main.h to prevent duplicity
map<int, pair<string, int> > playerNumberDictionary;
int totalHitCountInGame = 0;
ofstream* outputFileStream;
pair<int, int> *hitRegistry;

Team teamOne;
Team teamTwo;

/*
 Registers a player to the playerNumberDictionary
    @param number - the ID number of the player to register
    @param name - the name of the player to register
    @param teamNumber - identifies which team (precondition: teamNumber == 1 or 2) the player is on
 */
void registerPlayer(int number, string name, int teamNumber){
    playerNumberDictionary[number].first = name;
    playerNumberDictionary[number].second = teamNumber;
}

/*
 Builds and returns a simple lookup-map which saves the number of points rewarded for each hit location.
 Map literal reads: {{1,5},{2,8},{3,10},{4,15}}.
 */
map<int, int> buildHitValuesMap(){
    map<int, int> m;
    m[1] = 5;
    m[2] = 8;
    m[3] = 10;
    m[4] = 15;
    return m;
}

/*
 Registers a hit with hitRegistry, and updates scores for the team and player who shot the target
    @param hitDataString - the string from the input file containing a series of integers describing the hit
    @param &team1, &team2 - the two teams involved in the hit
    @param registryIndex - the index in the hitRegistry where this hit should be saved. Range = [0, total number of hits in match)
 */
void registerHit(string hitDataString, int registryIndex){

    /*
     hitDataString split up into an array of information as follows:
        hitDataArr[0] - the ID number of the shooter
        hitDataArr[1] - the ID number of the target
        hitDataArr[2] - the game time (ms) when the shot occured
        hitDataArr[3] - the location on the target's person that was shot (chest, gun, back, etc.)
     */
    string hitDataArr[4];

    //unwraps the hitDataString to the hitDataArr using a stringstream
    int iterator = 0;
    stringstream stream(hitDataString);
    while(stream.good()){
        string stringToConvert;
        stream >> hitDataArr[iterator];
        //cout << hitDataArr[iterator] << "\n";
        iterator++;
    }

    //builds and saves the map/dictionary which defines how many points are awarded for each type of hit
    map<int, int> hitValues = buildHitValuesMap();

    //convert hit information to integers via c_str and atoi
    int shooterIDNumber = atoi(hitDataArr[0].c_str());
    int targetIDNumber = atoi(hitDataArr[1].c_str());

    //add the score for the tag to the shooter's personal score and team score
    int teamNumber = playerNumberDictionary[shooterIDNumber].second;
    if(teamNumber == 1){
        teamOne.addToScore(shooterIDNumber, hitValues[atoi(hitDataArr[3].c_str())]);
    } else {
        teamTwo.addToScore(shooterIDNumber, hitValues[atoi(hitDataArr[3].c_str())]);
    }

    //save the hit information to the hitRegistry
    hitRegistry[registryIndex].first = shooterIDNumber;
    hitRegistry[registryIndex].second = targetIDNumber;

}

/*
 Parses a specified file for tag information and registers each individual hit/tag
    @param fileName - the name of the file to read from
    @param &team1, &team2 - the team objects associated with the match
 */
void processHitsFromFile(char* fileName){

    //open input file stream from file 'fileName'
    ifstream hitsFileStream;
    hitsFileStream.open(fileName);

    //hitDescription is the dynamic string used to store each line from the file
    string hitDescription;

    //numHits is the total number of hits registered by the input file
    int numHits;

    //if statement catches invalid filename
    if(hitsFileStream.is_open()){

        getline(hitsFileStream, hitDescription);
        numHits = atoi(hitDescription.c_str());

        //save numHits to a global variable for use elsewhere
        totalHitCountInGame = numHits;

        //array of <int, int> pairs to efficiently save tag data for later reference
        hitRegistry = new pair<int, int>[numHits];
        int hitRegistryAddIndex = 0;

        //Parses through entire file, registering each shot
        while(getline(hitsFileStream, hitDescription)){
            registerHit(hitDescription, hitRegistryAddIndex);
            hitRegistryAddIndex++;
        }

    } else {
        //cout << "Invalid file name.\n";
    }

    hitsFileStream.close();
}

/*
 Determines which team won the game and prints that team's name to the file
 */
void printOverallWinners(){
    if(teamOne.getTeamScore() == teamTwo.getTeamScore()){
        *outputFileStream << "The match was a draw!";
    } else {
        *outputFileStream << "Overall Winners: " << ((teamOne.getTeamScore() > teamTwo.getTeamScore())? teamOne.getTeamName() : teamTwo.getTeamName()) << "\n";
    }
}

/*
 Print the results of the match to a file based on verbosity input from command-line argument
    @param outputFileName - the name of the file to write the output to
    @param verbosityPreference - the command line argument specifying verbosity mode
    @param &teamOne, &teamTwo - the teams involved in the match
 */
void printResults(char* outputFileName, char* verbosityPreference){

    outputFileStream = new ofstream();
    outputFileStream->open(outputFileName);

    //Pointers to the two teams in alphabetical order
    Team* firstTeamAlphabetically;
    Team* secondTeamAlphabetically;


    //Assign the alphabetized pointers
    if(teamOne.getTeamName().compare(teamTwo.getTeamName()) < 0){
        firstTeamAlphabetically = &teamOne;
        secondTeamAlphabetically = &teamTwo;
    } else {
        firstTeamAlphabetically = &teamTwo;
        secondTeamAlphabetically = &teamOne;
    }

    //case: low verbosity
    if(verbosityPreference[1] == *"l"){

        //log the team scores and the overall winner
        firstTeamAlphabetically->printTeamScore();
        secondTeamAlphabetically->printTeamScore();

    }

    //case medium verbosity
    else if(verbosityPreference[1] == *"m"){

        //log team tag counts for both teams
        firstTeamAlphabetically->printTeamTagCounts();
        secondTeamAlphabetically->printTeamTagCounts();

        //log the best score from each team
        firstTeamAlphabetically->printBestScore();
        secondTeamAlphabetically->printBestScore();

        //log the team scores
        firstTeamAlphabetically->printTeamScore();
        secondTeamAlphabetically->printTeamScore();

    }

    //case high verbosity
    else if(verbosityPreference[1] == *"h"){

        //print detailed hit information for both teams
        firstTeamAlphabetically->printHighVerbosityScoreInfo(*secondTeamAlphabetically);
        *outputFileStream << "\n";
        secondTeamAlphabetically->printHighVerbosityScoreInfo(*firstTeamAlphabetically);

    } else {
        //cout << "Unrecognized verbosity specification.";
    }

    printOverallWinners();

    outputFileStream->close();
}

int main(int argc, char *argv[]){

    argc++;

    //Generate the two teams from file names specified by command line args
    teamOne = Team(argv[1], 1);
    teamTwo = Team(argv[2], 2);

    //Process all hits from the match file specified by a command line arg
    processHitsFromFile(argv[3]);

    //Write results with verbosity from command line arg to file named by a command line arg
    printResults(argv[4], argv[5]);

    delete[] hitRegistry;

    return 0;
}
