#ifndef TEAM_H
#define TEAM_H

using namespace std;

class Team
{

private:
    string teamName; //Team's name as designated by input file
    int teamNumber; //In the context of SMU Laser tag, this is a 1 or a 2 to indicate which team is represented by this Team object.
    int teamScore; //Total score of all players on the team combined
    int numPlayers; //The number of constituent players on the team
    int highestIndividualHitCount; //The highest number of hits any member of the team had

    /*
     A map of the players in the team, hashed by ID number.
        - playerScores[n].first is the total score of the player whose ID number is n
        - playerScores[n].second is the number of hits attributed to the player whose ID number is n
     */
    map<int, pair<int, int> > playerScores;

    void addPlayer(string inputLineFromFile);

public:
    
    //see team.cpp for implementations and method descriptions
    Team(char* fileName, int teamNum);
    Team();
    int getPlayerCount();
    void addToScore(int playerNum, int scoreAddition);
    int getTeamScore();
    map<int, pair<int, int> >* getPlayerMap();
    string getTeamName();
    vector<int> getPlayersInOutputOrder();
    void printTeamTagCounts();
    void printBestScore();
    void printTeamScore();
    void printHighVerbosityScoreInfo(Team& targetTeam);

//signals:

//spublic slots:
};

#endif // TEAM_H
