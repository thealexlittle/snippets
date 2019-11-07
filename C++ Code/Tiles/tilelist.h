/**
 * Assignment: Tiles
 * Partners: Alex Little, Zackery Bangs
 * Section Leader: Evin Yang
 * File Name: tilelist.h
 * ---------------------
 * This file, with tilelist.cpp, completes the Tile assignment.
 */


#ifndef _tilelist_h
#define _tilelist_h

#include <iostream>
#include <string>
#include "gwindow.h"
#include "tilenode.h"
using namespace std;

class TileList {
/**
 * Initializes all public methods for TileList
 */
public:
    TileList();
    ~TileList();
    void addBack(int x, int y, int width, int height, string color);
    void addFront(int x, int y, int width, int height, string color);
    void clear();
    void debug();
    void drawAll(GWindow& window) const;
    TileNode* getBack() const;
    TileNode* getFront() const;
    bool highlight(int x, int y);
    bool lower(int x, int y);
    void merge(int x, int y);
    bool raise(int x, int y);
    bool remove(int x, int y);
    int removeAll(int x, int y);

/**
 * Initializes all private methods/variables for TileList
 */
private:
    TileNode* _front;
    TileNode* _back;
};

#endif // _tilelist_h
