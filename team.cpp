#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

#include "team.h"
#include "main.h"

/*
 A Team object represents one SMU Laser Tag team, comprising of a number of players, each of whom has an ID number, a score, and a total tag count
 */

/*
 Adds a player to the team's playerScores map, and registers the player with the master player list
 @param inputLineFromFile - the unformatted data from the input file specifying the player's information
 */
void Team :: addPlayer(string inputLineFromFile){
    
    int indexOfWhitespace = inputLineFromFile.find(" "); //finds and saves the index of the space delimiter used to make a distinction between player ID and player name in the input file
    string playerName = inputLineFromFile.substr(indexOfWhitespace + 1); //saves the name of the player from the remainder of the input data string following the first space
    int playerNumber = atoi(inputLineFromFile.substr(0, indexOfWhitespace).c_str()); //translates player number to an integer (from a string via c_str and atoi)
    
    //register the player to the master registry, and save his information into the team's playerScores map
    registerPlayer(playerNumber, playerName, teamNumber);
    playerScores[playerNumber].first = 0;
    playerScores[playerNumber].second = 0;
}

//Default constructor -- unused
Team :: Team(){

}

/*
 Team constructor -- builds a team from a file called fileName, with a specific team number (either 1 or 2) as specified
 @param fileName - the name of the input file to be used to build the team
 @param teamNum - a 1 or a 2 to indicate which team is being built
 */
Team :: Team(char* fileName, int teamNum){
    
    teamNumber = teamNum;
    teamScore = 0;
    highestIndividualHitCount = 0;
    
    //open file stream with file name passed to the method
    ifstream inputFileStream;
    inputFileStream.open(fileName);
    
    //int for lineNumber indexing, string for saving each individual line
    string lineFromFile;
    int lineNumberInFile = 0;
    
    //'if' statement catches the case that the file is not found
    if(inputFileStream.is_open()){
        
        //loops through the entire file, line by line, saving each line to 'lineFromFile' one at a time
        while(getline(inputFileStream, lineFromFile)){
            
            //case: first line in the file, expecting to find the team name in this line
            if(lineNumberInFile == 0){
                teamName = lineFromFile;
            }
            
            //case: second line in the file, expecting to find the number of players on the team in this line
            else if (lineNumberInFile == 1) {
                numPlayers = atoi(lineFromFile.c_str());
            }
            
            //case: third+ line in the file, expecting to find registry information for a new player
            else if (lineNumberInFile <= numPlayers+1){
                addPlayer(lineFromFile);
            }
            
            lineNumberInFile++;
        }
        
    } else {
        //cout << "Can't find file."; // :'(
    }
}

//getter for number of players on the team
int Team :: getPlayerCount(){
    return numPlayers;
}

/*
 Adds a certain number of points to a player's score as the result of a hit by that player
 @param playerNum - the ID number of the player whose score will be increased
 @param scoreAddition - the number of points to be added to the designated player's score
 */
void Team :: addToScore(int playerNum, int scoreAddition){
    pair<int, int> *player = &playerScores[playerNum];
    player->first += scoreAddition; //increase player's individual score
    player->second++; //increment hit count for specified player
    teamScore += scoreAddition; //increase team score too
    
    //check for a surpassing of the highest individual hit count record on the team
    if(player->second > highestIndividualHitCount){
        highestIndividualHitCount = player->second;
        ////cout << "Highest Individual Hit Count: " << highestIndividualHitCount << "\n";
    }
}

//getter for total team score
int Team :: getTeamScore(){
    return teamScore;
}

//getter for a reference to the team's player registry map
map<int, pair<int, int> >* Team :: getPlayerMap(){
    return &playerScores;
}

//getter for the team's name
string Team :: getTeamName(){
    return teamName;
}


/*
 Generates a vector of the ID numbers of the player on the team in decreasing order of tags with ties broken by ID number
 Precondition: hit data has all been processed
 @return - a vector of all ID numbers associated with players on the team in the order they should be output
 */
vector<int> Team :: getPlayersInOutputOrder(){
    
    vector<int> playersInOutputOrder;
    
    //iterate through every possible number of hits on the team by going 0...highestIndividualHitCount
    for (int numHitsIndex = highestIndividualHitCount; numHitsIndex >= 0; numHitsIndex--){
        
        //all players who have a total number of tags equal to numHitsIndex are placed in this vector
        vector<int> IDNumbersTiedAtThisScore;
        
        //iterate through the players on the team, adding a player's ID number to the vector if he has numHitsIndex total registered hits
        for(map<int, pair<int, int> >::iterator it = playerScores.begin(); it  != playerScores.end(); ++it){
            if(it->second.second == numHitsIndex){
                IDNumbersTiedAtThisScore.push_back(it->first);
            }
        }
        
        //sort the vector of ID numbers to break ties by ID number
        sort(IDNumbersTiedAtThisScore.begin(), IDNumbersTiedAtThisScore.end());
        
        //add the sorted list of players with numHitsIndex total registered tags to the output vector
        playersInOutputOrder.insert(playersInOutputOrder.end(), IDNumbersTiedAtThisScore.begin(), IDNumbersTiedAtThisScore.end());
    }
    
    //the list is reversed so that the players with the *most* hits and *highest* id numbers appear first.
    return playersInOutputOrder;
    
}

/*
 Outputs the medium-verbosity tag information for the team (number of tags for each player)
 */
void Team :: printTeamTagCounts(){
    
    *outputFileStream << teamName << "\n";
    
    //generate and save the players to output in their correct output order
    vector<int> players = getPlayersInOutputOrder();
    
    for(vector<int>::iterator it = players.begin(); it!= players.end(); it++){
        int numHits = playerScores[*it].second;
        *outputFileStream << "\t" << playerNumberDictionary[*it].first << " had a total of " << numHits << ((numHits == 1)? " tag.\n" : " tags.\n");
    }

    players.clear();

    *outputFileStream << "\n";
    
}

/*
 Calculates and prints the highest score achieved on the team, and the name associated with that score
 */
void Team :: printBestScore(){
    
    //standard max algorithm, finds maximum score on the team
    int max = 0;
    string name = "";
    //uses iterator to iterate through the team's player registry map
    for(map<int, pair<int, int> >::iterator it = playerScores.begin(); it != playerScores.end(); it++){
        //case: current player has highest found score, set the new max and save his name
        if(it->second.first > max){
            name = playerNumberDictionary[it->first].first;
            max = it->second.first;
            
            //case: current player is tied for highest found score, add his name alongside that of the previous record-holder
        } else if(it->second.first == max){
            name += " and "  + playerNumberDictionary[it->first].first;
        }
    }
    *outputFileStream << "Best score from " << teamName << ": " << name << " (" << max << " points)\n";
}

/*
 Writes team name and score to output file. Utility method.
 */
void Team :: printTeamScore(){
    *outputFileStream << teamName << ": " << teamScore << " points.\n";
}

/*
 For each player on shooterTeam, writes how many times that player tagged each player on targetTeam to the output file
    @param &targetTeam - the team of players whom shooterTeam's players shot
 */
void Team :: printHighVerbosityScoreInfo(Team& targetTeam){
    *outputFileStream << getTeamName() << "\n";

    //generate and save the order in which the players from each team should be printed
    vector<int> shooterTeamInOrder = getPlayersInOutputOrder();
    vector<int> targetTeamInOrder = targetTeam.getPlayersInOutputOrder();

    //for each shooter on the shooter team
    for(vector<int>::iterator shooter = shooterTeamInOrder.begin(); shooter  != shooterTeamInOrder.end(); shooter++){

        //for each target
        for(vector<int>::iterator target = targetTeamInOrder.begin(); target  != targetTeamInOrder.end(); target++){
            int hitCount = 0;

            //for each registered hit, check if the hit matches the current shooter, target pair
            for(int i = 0; i < totalHitCountInGame; i++){
                if(hitRegistry[i].first == *shooter && hitRegistry[i].second == *target){
                    hitCount++;
                }
            }
            *outputFileStream << "\t" << playerNumberDictionary[*shooter].first << " tagged " << playerNumberDictionary[*target].first << " " << hitCount << " times.\n";
        }

        //shooter score is grabbed from shooter team's player map such that shooterScore = <shooter points, shooter tag count>
        pair<int, int> shooterScore = (*getPlayerMap())[*shooter];
        *outputFileStream << "\t" << playerNumberDictionary[*shooter].first << " had a total of " << shooterScore.first << " points and " << shooterScore.second << ((shooterScore.second == 1)? " tag.\n": " tags. \n");
    }
    *outputFileStream << "\t"; printTeamScore();
}

