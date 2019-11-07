// This is the CPP file you will edit and turn in. (TODO: Remove this comment!)

#include "map.h"
#include "vector.h"
#include "strlib.h"
#include "random.h"
#include "grammarsolver.h"
using namespace std;

Map<string, Vector<string>> buildgrammarRules(istream &input);
void creategrammarRulesLine(Map<string, Vector<string>>& grammarRules,string line );
void grammarGenerateHelper(Map<string, Vector<string> > &grammarRules, string symbol, string &grammarline);

Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    Vector<string> output;   // this is only here so it will compile
    string grammarline;

    // read input grammar rules line by line
    Map<string, Vector<string>> grammarRules = buildgrammarRules(input);

    // generate grammar
    for (int i = 0; i < times; i++) {
        grammarGenerateHelper(grammarRules,symbol, grammarline);
        output += grammarline;
        grammarline = "";
    }

    return output;           // this is only here so it will compile
}

Map<string, Vector<string>> buildgrammarRules(istream& input) {
    Map<string, Vector<string>> grammarRules;

    string line;
    while (getline(input, line)) {
        creategrammarRulesLine(grammarRules, line);
    }
    return grammarRules;
}

void creategrammarRulesLine(Map<string, Vector<string>>& grammarRules,string line ) {
    // Find separater index [ ::= ]
    int index = stringIndexOf(line, ":");

    // Everything before the separator is the key
    string key = line.substr(0,index);

    // Everything after are the rule values
    string rules = line.substr(index+3);

    // Turn the rules into a vector of strings
    Vector<string> ruleSet = stringSplit(rules, "|");

    grammarRules[key] += ruleSet;
}

void grammarGenerateHelper(Map<string, Vector<string>>& grammarRules, string symbol, string& grammarline) {

    Vector<string> symbols = stringSplit(symbol," ");

    for (int i = 0; i < symbols.size(); i++) {
        if (!grammarRules.containsKey(symbols[i])) {
            grammarline += " " + symbols[i];
        } else {
            string curr = symbols[i];
            grammarGenerateHelper(grammarRules, randomElement(grammarRules[curr]), grammarline);
        }
    }
}






















