

#include <iostream>
#include "map.h"
#include "filelib.h"
#include "console.h"
#include "queue.h"
#include "strlib.h"
#include "vector.h"
#include "bitstream.h"
#include "random.h"
#include "simpio.h"

using namespace std;

void printWelcome();
void generateNgramMap(Map<Vector<string>, Vector<string>> &ngrams, int &ngramlength);
    Queue<string> readText();
void printNgrams( Map<Vector<string>, Vector<string>> ngrams, int numWords);

int main() {
    Map<Vector<string>, Vector<string>> ngrams;
    int ngramlength;

    printWelcome();
    generateNgramMap(ngrams, ngramlength);

    // create Ngrams until the user inputs 0
    int numWords = 1;
    while (numWords != 0) {
        numWords = getInteger("# of random words to generate (0 to quit)?");
        if (numWords == 0) break;
        while (numWords < ngramlength) {
            cout << "Must be at least " << ngramlength << " words." << endl;
            numWords = getInteger("# of random words to generate (0 to quit)?");
        }
        if (numWords == 0) break;
        printNgrams(ngrams, numWords);
    }
    cout << "Exiting." << endl;
    return 0;
}

void printWelcome() {
    cout << "Welcome to CS 106B/X Random Writer ('N-Grams')!" << endl;
    cout << "This program generates random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl;
    cout << endl;
}

// creates a queue out of the source file then builds the Ngram map
void generateNgramMap(Map<Vector<string>, Vector<string>> &ngrams, int &ngramlength) {
    Queue<string> sourcetext = readText();
    Vector<string> firstNgram;
    Vector<string> ngramKey;

    // prompt ngramlength
    ngramlength = getInteger("Value of N? ");
    while (ngramlength < 2) {
        cout << "N must be 2 or greater." << endl;
        ngramlength = getInteger("Value of N? ");
    }
    cout << endl;

    // initialize string vector
    for (int i = 0; i < ngramlength-1; i++) {
        ngramKey += sourcetext.dequeue();
    }
    // saves the first ngram for wrapping purposes
    firstNgram = ngramKey;

    // go through sourcetext
    // add ngram value
    // change key
    while (!sourcetext.isEmpty()) {
        ngrams[ngramKey] += sourcetext.peek();

        // change ngramkey
        ngramKey.insert(ngramlength-1,sourcetext.dequeue());
        ngramKey.remove(0);
    }
    // wraps ngram map from the end to beginning of the dataset
    for (int i = 0; i < ngramlength-1; i++) {
        ngrams[ngramKey] += firstNgram[i];
        ngramKey.insert(ngramlength-1,firstNgram[i]);
        ngramKey.remove(0);
    }
}

//prompts a file then returns a queue of the words in the fie
Queue<string> readText() {
    string filename = promptUserForFile("Input file name?");
    Queue<string> sourcetext;
    string text = readEntireFile(filename);

    // removes endline characters from input, and replaces them with a space
    for (int i = 0; i < text.size(); i++) {
        if (text[i] == '\n'){
            text[i] = ' ';
        }
    }

    //takes each word and stores them into a Vector of individual words
    Vector<string> split = stringSplit(text," ");

    //iterates through vector of individual words
    // trims the whitespace from the beginning and end of each word
    // discards any "words" from the vector that are spaces or empty strings *For Safety*
    for (string word : split){
        word = trim(word);
        if (word[0] != ' ' && word.size() != 0)
            sourcetext.enqueue(word);
    }
    return sourcetext;
}

//prints the ngram of the desired length
void printNgrams(Map<Vector<string>, Vector<string>> ngrams, int numWords) {
    Vector<string> currentKey = randomKey(ngrams);
    cout << ". . . ";

    // prints out the initial words of tehe ngram
    for (string word : currentKey) {
        cout << word << " ";
    }

    // prints the next word and then advacnes the ngram "window"
    for (int i = currentKey.size(); i <= numWords; i++) {
        string nextword = randomElement(ngrams[currentKey]);
        cout << nextword << " ";
        currentKey.insert(currentKey.size(),nextword);
        currentKey.remove(0);
    }
    cout << ". . . " << endl;
    cout << endl;
}

