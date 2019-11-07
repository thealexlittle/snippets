/***********************************************************************************
 * Assignment: Huffman Coding
 * Partners: Alex Little, Zackery Bangs
 * Section Leader: Evin Yang
 * Filename: encoding.cpp
 * ----------------------
 * This file, with secretmessage.txt.huf, completes the Huffman Coding assignment.
 *
 * Encoding.cpp reads in a file, encoding it and converting it (in the end), into
 * a header. This header can later be decoded and converted back into the original
 * message/code/file contents.
 *
 * In more detail, the encoding is possible because of a few steps. First, the
 * characters are counted and added into a map, where the key is the char and
 * the value is the number of times it occurs in the file. Secondly, a binary tree
 * of nodes, with each leaf containing a character from the map, is built. Next,
 * this tree is flattened into a single string (header), and character positions are
 * converted into an integer. The header and integer number are written into the
 * output file.
 *
 * The decoding occurs as the process is reversed: the header is converted into
 * a tree, the integer number finds their position, the message is reconstructed
 * and the decoding is complete.
 */
#include "encoding.h"
#include "priorityqueue.h"
#include "deque.h"
#include "filelib.h"
#include "strlib.h"

/*************************************************************************************************
 * @brief buildFrequencyTable - takes the input, reads the characters out individually,
            then builds a map of these characters with character count as the value.
 * @param input - the istream of file contents
 * @return freqTable - a map of character keys and character count values
 */
Map<char, int> buildFrequencyTable(istream& input) {
    Map<char, int> freqTable;
    char ch;
    while (input.get(ch)){
        freqTable[ch]++;
    }
    return freqTable;
}

/***************************************************************************************************
 * @brief buildEncodingTree - takes in a freqency table and creates a binary tree of
            Huffman Nodes, organized with the leafs containing characters and nullptrs
 * @param freqTable - a map of character keys and character count values
 * @return top - a pointer to the top of the encoding tree
 */
HuffmanNode* buildEncodingTree(Map<char, int>& freqTable) {

    PriorityQueue<HuffmanNode*> hQueue;

    for (char ch : freqTable) {
        HuffmanNode* leaf = new HuffmanNode(ch);
        hQueue.add(leaf ,freqTable[ch]);
    }

    while (hQueue.size() > 1) {
        HuffmanNode* newNode;
        double leg1_priority = hQueue.peekPriority();
        HuffmanNode* leg1 = hQueue.dequeue();
        double leg2_priority = hQueue.peekPriority();
        HuffmanNode* leg2 = hQueue.dequeue();
        newNode = new HuffmanNode(leg1, leg2);
        hQueue.enqueue(newNode, leg1_priority+leg2_priority);
    }
    HuffmanNode* top = hQueue.dequeue();
    return top;
}

/*****************************************************************************************************
 * @brief flattenTreeToHeader - takes the encoding tree and reduces it to a single string that can
            later be decoded
 * @param t - a HuffmanNode pointing to the next node in the tree
 * @return header - a string conversion of the binary tree
 */
string flattenTreeToHeader(HuffmanNode* t) {
    if (t->isLeaf()) {
        return "." + string(1, t->ch);
    }
    return "(" + flattenTreeToHeader(t->zero) + flattenTreeToHeader(t->one) + ")";
}

/******************************************************************************************************
 * @brief recreateTreeHelper - takes in a deque of characters representing a node and creates the node
 * @param nodeToken - the nodeToken that is being passed in
 * @return - interior or leaf node containing the right information
 */
HuffmanNode* recreateTreeHelper( Deque<char> nodeToken) {
    if (nodeToken.removeFront() == '.') {
        char leaf = nodeToken.removeFront();
        return new HuffmanNode(leaf);
    }

     HuffmanNode* left = nullptr;
     HuffmanNode* right = nullptr;
     Deque<char> newNodeToken;
     nodeToken.removeBack();

    if (nodeToken.peekFront() == '(') {
        newNodeToken.addBack(nodeToken.removeFront());

        int balance = 1;
        while (balance != 0) {
            char ch = nodeToken.removeFront();
            newNodeToken.addBack(ch);
            if ( ch == '(' ) balance++;
            if (ch == ')') balance--;
        }
    } else {
      newNodeToken.addBack(nodeToken.removeFront());
      newNodeToken.addBack(nodeToken.removeFront());
    }

    left = recreateTreeHelper(newNodeToken);
    right = recreateTreeHelper(nodeToken);

    return new HuffmanNode(left, right);
}

/********************************************************************************************************
 * @brief recreateTreeFromHeader - reads in a string header and returns the root HuffmanNode tree
 * @param str - a string representing the header of a HuffmanNodeTree
 * @return - the root HuffmanNode of a tree
 */
HuffmanNode* recreateTreeFromHeader(string str) {
    Deque<char> dq;
    for (char ch : str) {
        dq.enqueue(ch);
    }
    return recreateTreeHelper(dq);
}

/**********************************************************************************************************
 * @brief buildEncodingMapHelper - populates a map of characters with their path in bits (as a string)
            based on the node passed in
 * @param node - The node that a given char is beeing mapped from
 * @param found - The map of chars and bit paths (represented as a string)
 * @param code
 */
void buildEncodingMapHelper(HuffmanNode* node, Map<char, string>& found, string code = "" ) {
    if (node->isLeaf()) {
        char letter = node->ch;
        if (!found.containsKey(letter))
            found[letter] = code;
        return;
    }
    if (node->isLeaf()) {
        char letter = node->ch;
        if (!found.containsKey(letter))
            found[letter] = code;
    }

    code += "0";
    buildEncodingMapHelper(node->zero, found, code);

    code.pop_back();
    code+= "1";
    buildEncodingMapHelper(node->one, found, code);
}

/***********************************************************************************************************
 * @brief buildEncodingMap - builds a map of chars that correspond to their string represention in bits
 * @param encodingTree - the root of the encoding tree
 * @return Map of a char to its binary represention in relation to the tree
 */
Map<char, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<char, string> found;
    buildEncodingMapHelper(encodingTree, found);
    return found;
}

/***********************************************************************************************************
 * @brief freeTree - delete a HuffmanNode that is passed in
 * @param t - a HuffmanNode pointer
 */
void freeTree(HuffmanNode* t) {
    if (t->zero == nullptr && t->one == nullptr) {
        delete t;
    } else {
        freeTree(t->zero);
        freeTree(t->one);
        delete t;
    }
}

/**************************************************************************************************************
 * @brief compress - compresses a text file into a .huf file
 * @param input - The original text stream
 * @param output - The .huf compression of the text file
 */
void compress(istream& input, HuffmanOutputFile& output) {
    Map<char,int> freqTable = buildFrequencyTable(input);
    rewindStream(input);
    HuffmanNode* tree = buildEncodingTree(freqTable);
    Map<char, string> codedLetters = buildEncodingMap(tree);
    string flat = flattenTreeToHeader(tree);
    output.writeHeader(flat);

    char ch;
    while (input.get(ch)) {
       string code = codedLetters[ch];
       for (char ch : code) {
           int bit = charToInteger(ch);
           output.writeBit(bit);
        }
    }
}

/*******************************************************************************************************************
 * @brief decompress - decompresses a .huf file into a text file
 * @param input - .huf file
 * @param output - an ouput stream that contains uncompressed text
 */
void decompress(HuffmanInputFile& input, ostream& output) {
  string header = input.readHeader();
  HuffmanNode* tree = recreateTreeFromHeader(header);
  Map<char, string> encodingMap = buildEncodingMap(tree);
  buildEncodingMapHelper(tree, encodingMap);

  int bit = input.readBit();
  while (bit != -1) {
      HuffmanNode* curr = tree;
      while (!curr->isLeaf()) {
          if (bit == 0) {
              curr = curr->zero;
          } else {
              curr = curr->one;
          }
          bit = input.readBit();
      }
      output << curr->ch;
  }

}
