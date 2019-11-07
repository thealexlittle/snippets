/**
 * Partners: Zackery Bangs, Alex Little
 * Section Leader: Evin Yang
 * File Name: boggle.cpp
 * -------------------------------------
 * This file completes the Boggle assignment.
 * Boggle.cpp, working alongside the other project files, allows the user to play the game of Boggle. The player's objective
 * is to beat the computer by finding as many words on the Boggle board as possible. After the player indicates the end of
 * their turn, the computer will iterate through the board to find all of the possible words on the board. At the end of the
 * game, the computer will indicate the winner and allow the user to either play again or end the session.
 */
#include "boggle.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "lexicon.h"
#include "random.h"
#include "set.h"
#include "shuffle.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"
#include "gui.h"
using namespace std;

// function prototype declarations
void intro();
void makeBoard(Grid<char>& board);
bool checkBoardEntry(string entry);

bool humanWordSearchHelper(Grid<char>& board, string word, int row, int col);
void computerWordSearchHelper(Grid<char> board, Lexicon &dictionary, Set<string>& computerWords,
             Set<string> &humanWords, int &row, int &col, string soFar, int& score);

void playGame(Grid<char>& board);
void validWord(Grid<char>& board, Set<string>& playerWords, string& playerWord, int& playerScore);
void endTurn(Grid<char>& board, Lexicon& dict, Set<string>& playerWords, Set<string>& computerWords,
             int& compScore, int& playerScore);

int checkValid(string playerWord, Set<string> playerWords, Lexicon dict);
int scoreWord(string playerWord);

//----------------------------------------------------------------------------------------------------------------------------------------------
/**
 * @brief qMain - this function runs the game 'Boggle' in its entirety.
 * @return int - 0;
 */
int main() {
  gui::initialize(BOARD_SIZE, BOARD_SIZE);
  Grid<char> board(BOARD_SIZE, BOARD_SIZE);

  intro();
  makeBoard(board);
  playGame(board);

  while(getYesOrNo("Play again?")){
      cout << endl;
      gui::initialize(BOARD_SIZE, BOARD_SIZE);
      makeBoard(board);
      playGame(board);
  }

  cout << "Have a nice day." << endl;
  return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------
/**
 * @brief intro - this function prints out an introduction for the user to the game
 */
void intro() {
  cout << "Welcome to CS 106B Boggle!" << endl;
  cout << "This game is a search for words on a 2-D board of letter cubes." << endl;
  cout << "The good news is that you might improve your vocabulary a bit." << endl;
  cout << "The bad news is that you're probably going to lose miserably to" << endl;
  cout << "this little dictionary-toting hunk of silicon." << endl;
  cout << "If only YOU had 16 gigs of RAM!" << endl;
  cout << endl;
  getLine("Press Enter to begin the game ...");
}

//----------------------------------------------------------------------------------------------------------------------------------------------
/**
 * @brief makeBoard - This method creates a board by either randomly generating
 *      a board or by taking in a string input from a user.
 * @param board - A grid of characters that is filled.
 */
void makeBoard(Grid<char>& board) {
  if (getYesOrNo("Generate a random board? ")) {
      shuffle(LETTER_CUBES);
      for (int i = 0; i < BOARD_SIZE*BOARD_SIZE; i++) {
          string cube = shuffle(LETTER_CUBES[i]);
          char ch = cube[0];
          board[i / BOARD_SIZE][i % BOARD_SIZE] =  ch;
      }
  } else {
      int numEntries = BOARD_SIZE*BOARD_SIZE;
      string strNum = to_string(numEntries);
      string entryPrompt = "Type the " + strNum + " letters on the board.";
      string entry = getLine(entryPrompt);

      while(!checkBoardEntry(entry)){
          cout << "Invalid board string. Try again." << endl;
          entry = getLine(entryPrompt);
      }

      for (int i = 0; i < entry.size(); i++) {
          char ch = toupper(entry[i]);
          board[i / BOARD_SIZE][i % BOARD_SIZE] = ch;
      }
  }
  gui::labelCubes(board);
}

//----------------------------------------------------------------------------------------------------------------------------------------------
/**
 * @brief checkBoardEntry - verifies if the string input from the user can be put into the grid
 * @param entry - A string that came from a user
 * @return - Will return true if the entry is of a certain length and every character is a letter
 */
bool checkBoardEntry(string entry){
    if(entry.length() == BOARD_SIZE*BOARD_SIZE){
        for(char letter : entry) {
            if(isalpha(letter)){
                return true;
            }
        }
    }

    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------
/**
 * @brief humanWordSearch - this function searches the board for the word the user input
        and indicate whether the word is on the board or not
 * @param board - the game board
 * @param word - the word entered by the user
 * @return bool - a true or false response to the user's word validity
 */
bool humanWordSearch(Grid<char>& board, string word) {
  Grid<char> newBoard = board;
  char firstLetter = word[0];

  for(int row = 0; row < BOARD_SIZE; row++) {
      for(int col = 0; col < BOARD_SIZE; col++) {
          if(board[row][col] == firstLetter) {
              if(humanWordSearchHelper(newBoard, word, row, col)) {
                  return true;
              }
          }
          gui::clearHighlighting();
      }
  }
  return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------
/**
* @brief humanWordSearchHelper - recursively calls itself in order to find if the given word is on the board
* @param board - the game board
* @param word - the word given by the user
* @param row - the current row that is being examined
* @param col - the current column that is being examined
* @return - will return true if the word can be found on the board
*/
bool humanWordSearchHelper(Grid<char>& board, string word, int row, int col) {
   if(word.length() == 1) {
       return true;
   }

   gui::setHighlighted(row, col, true);

   board[row][col] = '*';
   char nextLetter = word[1];

   for(int x = row - 1; x <= row + 1; x++) {
       for(int y = col - 1; y <= col + 1; y++) {
           if (board.inBounds(x,y)) {
               gui::setHighlighted(x, y, true);
               if(board[x][y] == nextLetter) {
                   if(humanWordSearchHelper(board, word.substr(1), x, y)) {
                       return true;
                   }
                   if (board[x][y] != '*') {
                       gui::setHighlighted(x, y, false);
                   }
               }

           }
       }
   }
   return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------
/**
* @brief computerWordSearch - this function searches the board for all of the words the user
        did not guess, then returns a set of these words
* @param board - the game board
* @param dictionary - a Lexicon of all of the valid English words
* @param humanWords - a set of the words previously entered by the user
* @return Set<string> - a set of the computer's words
*/
Set<string> computerWordSearch(Grid<char>& board, Lexicon& dictionary, Set<string>& humanWords) {
   Set<string> words;
   Grid<char> newBoard = board;
   int score = 0;

  for (int row = 0; row < BOARD_SIZE; row++ ) {
      for (int col = 0; col < BOARD_SIZE; col++) {
          computerWordSearchHelper(newBoard, dictionary, words, humanWords, row, col,"", score);
      }
  }
   return words;
}

//----------------------------------------------------------------------------------------------------------------------------------------------
/**
 * @brief computerWordSearchHelper - This method locates all of the words on the board that the human did not find
 * @param board - The game board
 * @param dictionary - The list of valid words that exist in the game
 * @param computerWords - a list of all computer's found words
 * @param humanWords - all of the user's past input words
 * @param row - the first coordinate point for the currently selected letter
 * @param col - the second coordinate point for the currently selected letter
 * @param soFar - a string of all previously validated letters
 * @param score - the updated score for all valid, computer-input words
 */
void computerWordSearchHelper(Grid<char> board, Lexicon& dictionary, Set<string>& computerWords, Set<string>& humanWords, int& row, int& col, string soFar, int& score) {

   gui::setHighlighted(row, col, true);
   soFar += board[row][col];
   board[row][col] = ' ';

   if(!dictionary.containsPrefix(soFar)) {
       return;
   }

   if (dictionary.contains(soFar) && !computerWords.contains(soFar) && !humanWords.contains(soFar) && soFar.size() >= MIN_WORD_LENGTH) {
       computerWords.add(soFar);
       gui::recordWord("computer", soFar);
       score += scoreWord(soFar);
       gui::setScore("computer", score);
   }

   for (int x = row - 1; x <= row + 1; x++) {
       for (int y = col - 1; y <= col + 1; y++) {
           if (board.inBounds(x,y) && board[x][y] != ' ') {
               computerWordSearchHelper(board, dictionary, computerWords, humanWords, x, y, soFar, score);
               gui::setHighlighted(x, y, false);
           }
       }
   }
}

//----------------------------------------------------------------------------------------------------------------------------------------------
/**
* @brief playGame - allows the user to play the game on the game board
* @param board - the game board
*/
void playGame(Grid<char>& board) {
  Lexicon dict(DICTIONARY_FILE);
  Set<string> playerWords;
  int playerScore = 0;
  Set<string> computerWords;
  int compScore = 0;

  cout << "It's your turn!" << endl;
  cout << "Your words: " << playerWords << endl;
  cout << "Your score: " << playerScore << endl;

  string enteredWord = " ";

  while(enteredWord != ""){
      enteredWord = getLine("Type a word (or Enter to stop): ");
      string playerWord = "";
      for(char c : enteredWord){
          playerWord += toupper(c);
      }

      int test1 = checkValid(playerWord, playerWords, dict);


      if(test1 == 0){
          validWord(board, playerWords, playerWord, playerScore);
      } else if (test1 == 1) {
          endTurn(board, dict, playerWords, computerWords, compScore, playerScore);
      } else if (test1 == 2) {
          cout << "This is not long enough to waste my time. Try again." << endl;
      } else if (test1 == 3) {
          cout << "You have already found that word." << endl;
      } else if(test1 == 4) {
          cout << "That word is not found in the dictionary" << endl;
      }
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------
/**
 * @brief validWord - this method is a response to a valid word input by the user
 * @param board - the game board
 * @param playerWord - the player's input word
 * @param playerScore - the player's score
 */
void validWord(Grid<char>& board, Set<string>& playerWords, string& playerWord, int& playerScore) {
    if(humanWordSearch(board, playerWord)){
        playerWords += playerWord;
        playerScore += scoreWord(playerWord);
        gui::recordWord("human", playerWord);
        gui::setScore("human", playerScore);
        cout << "You found a new word! \"" + playerWord + "\"" << endl;
        cout << endl;
        cout << "Your words: " << playerWords << endl;
        cout << "Your score: " << playerScore << endl;
    } else {
        cout << "This word is not on the board." << endl;
    }
    gui::clearHighlighting();
}

//----------------------------------------------------------------------------------------------------------------------------------------------
/**
 * @brief endTurn - this method is a response to the user finshing their turn
 * @param board - the game board
 * @param dict - a list of all valid English words
 * @param playerWords - all of the user's valid input words
 * @param computerWords - all of the computer's valid words
 * @param compScore - the computer's updated score
 * @param playerScore - the player's updated score
 */
void endTurn(Grid<char>& board, Lexicon& dict, Set<string>& playerWords, Set<string>& computerWords,
             int& compScore, int& playerScore){
    cout << endl;
    cout << "It's my turn!" << endl;
    computerWords = computerWordSearch(board, dict, playerWords);
    cout << "My words: " << computerWords << endl;

    for (string word : computerWords) {
        compScore += scoreWord(word);
    }

    gui::setScore("computer", compScore);
    cout << "My score: " << compScore << endl;

    if(compScore > playerScore){
        cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
    } else if (compScore == playerScore) {
        cout << "Dang, we tied." << endl;
    } else {
        cout << "WOW, you defeated me! Congratulations!" << endl;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------
/**
* @brief checkValid - this method looks at the user's input and designates it a number (0-4) to indicate what
*       conditions it has or hasn't met. If the method returns 0, then the word is valid. Otherwise, there
*       is a problem with the word.
* @param board - the game board
* @param playerWord - the user's input word
* @param playerWords - a list of the user's past valid word inputs
* @param dict - a list of all valid English words
* @return int - a code designated to the possible problems with the player's given word
*/
int checkValid(string playerWord, Set<string> playerWords, Lexicon dict) {
 if(playerWord == ""){
      return 1;
  } else if(playerWord.size() < 4) {
      return 2;
  } else if (playerWords.contains(playerWord)) {
      return 3;
  } else if (!dict.contains(playerWord)){
      return 4;
  }
  return 0;

}

//----------------------------------------------------------------------------------------------------------------------------------------------
/**
* @brief scoreWord - a scoring system for the words based on word length
* @param word - the given word
* @return int - the score designated to the given word
*/
int scoreWord(string word){

  int size = word.size();

  if(size == 4){
      return 1;
  } else if (size == 5) {
      return 2;
  } else if (size == 6) {
      return 3;
  } else if (size == 7) {
      return 5;
  }
  return 11;
}
