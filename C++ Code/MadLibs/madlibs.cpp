// Don't forget to write your own madlib!!!!!

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "simpio.h"
#include "strlib.h"
using namespace std;


// function decomposition
void intro();
void createMadLib(double &totaltokens, double &totalmadlibs);
    void parseLine(string& line, int& madlibtokens);
        string getMadLib(string token);
    void addMadLibLine(string line, string &madlib);
    void printMadLib(string madlibstory, int madlibtokens, double &totaltokens, double &totalmadlibs);
void printEndGame(double &totaltokens, double &totalmadlibs);


int main() {
  double totaltokens = 0;
  double totalmadlibs = 0;

  intro();
  createMadLib(totaltokens, totalmadlibs);
  while (getYesOrNo("Do another Mad Lib (Y/N)?")) {
      createMadLib(totaltokens, totalmadlibs);
  }
  printEndGame(totaltokens, totalmadlibs);
  return 0;
}

// prints out the welcome message of the game
void intro() {
  cout << "Welcome to CS 106B Mad Libs!" << endl <<
"I will ask you to provide various words" << endl <<
"and phrases to fill in a story." << endl <<
"At the end, I will display your story to you." << endl << endl;
}

//essentially plays the game
// opens the file, reads each line for madlib prompts (parseLine)
// and adds them to the madlib (addMadLibLine)
void createMadLib(double &totaltokens, double &totalmadlibs){
    string filename = promptUserForFile("Mad Lib input file?");
    cout << endl;
    string madlibstory = "";
    ifstream input;
    int madlibtokens = 0;

    input.open(filename);
    string line;
    while (getline(input,line)) {
        parseLine(line, madlibtokens);
        addMadLibLine(line, madlibstory);
    }
    input.close();
    printMadLib(madlibstory, madlibtokens, totaltokens, totalmadlibs);

}

// Find "<" ">" tokens
// passes the token to getMadLib
// replaces the "< >" token with the input from getMadLib
// increments the token finder past the token
// increments the madlibtoken counter by 1 for every token processsed
void parseLine(string& line, int& madlibtokens) {
    int tokenstart;
    int tokenend;
    int tokenlen;
    string token;
    string madlib;

    for (int i = 0; i <= line.length(); i++) {
        tokenstart = line.find('<',i)+1;
        if (tokenstart != 0) {
            tokenend = line.find('>',tokenstart);
            tokenlen = tokenend-tokenstart;
            token = line.substr(tokenstart,tokenlen);
            line.replace(tokenstart-1,tokenlen+2,getMadLib(token));
            i = tokenstart;
            madlibtokens++;
        } else {
            break;
        }
    }
}

// Propmts the user for whatever token was passed in
// returns the user's input to the parseLine function
string getMadLib(string token) {
    string madLibPrompt;
    if(token[0] == 'a'|| token[0] == 'e' || token[0] == 'i' || token[0] == 'o' || token[0] == 'u' ||
        token[0] == 'A'|| token[0] == 'E' || token[0] == 'I' || token[0] == 'O' || token[0] == 'U' ){
        madLibPrompt = getLine("Please type an " + token + ":");
    } else {
        madLibPrompt = getLine("Please type a " + token + ":");
    }
    return madLibPrompt;
}

//adds the madlib line after it had been filed with user inputs
void addMadLibLine(string line, string& madlib) {
    madlib+= "\n" + line;
}

// increments the total amount of madlib tokens processed
// increments the totl amount of madlibs made
//prints the "stats" for an individual madlib
void printMadLib(string madlibstory, int madlibtokens, double &totaltokens, double &totalmadlibs) {
    totaltokens += madlibtokens;
    totalmadlibs++;
    cout << endl << "Your Mad Lib story:" << madlibstory << endl << endl;
    cout << madlibtokens << " placeholder(s) replaced." << endl;
}

// calculates the number of madlib tokens per story
// prints out total "stats"
void printEndGame(double &totaltokens, double &totalmadlibs){
    double madlibs_per_story = totaltokens/totalmadlibs;
    cout << endl << "Mad Lib stories you created: " << totalmadlibs << endl;
    cout << "Total placeholders replaced: " << totaltokens << " (" << fixed << setprecision(1) << madlibs_per_story << " per story)" << endl;
}
