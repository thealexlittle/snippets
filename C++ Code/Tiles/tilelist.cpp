/**
 * Assignment: Tiles
 * Partners: Alex Little, Zackery Bangs
 * Section Leader: Evin Yang
 * File: tilelist.cpp
 * ------------------
 * This file, with tilelist.h, completes the Tiles assignment.
 *
 * Tiles implements the TileList class of object TileNode. The user will use
 * the functions within the file to modify TileNode. Running the file will allow
 * the user to interact with TileNodes, changing and modifying the List and the
 * Nodes, from moving Nodes to the front and back of the z-order, to adding and
 * removing them from the List and z-order themselves.
 */
#include "tilelist.h"
#include <algorithm>
#include "strlib.h"
using namespace std;

/**
 * @brief TileList::TileList - initializes the front and back pointers of
        TileList
 */
TileList::TileList() {
    _front = nullptr;
    _back = nullptr;
}

/**
 * @brief TileList::~TileList - clears the entire TileList at the exiting of the
        program 
 */
TileList::~TileList() {
    clear();
}

/**
 * @brief TileList::addBack - adds a TileNode to the back of the Z- Ordering
 * @param x - the x-position of the new TileNode
 * @param y - the y-position of the new TileNode
 * @param width - the width of the new TileNode
 * @param height - the height of the new TileNode
 * @param color - the color of the new TileNode
 */
void TileList::addBack(int x, int y, int width, int height, string color) {
    if (_front == nullptr) {
        _front = new TileNode(x, y, width, height, color);
        _back = _front;
    } else if (_front == _back && _front != nullptr) {
        _back = new TileNode(x, y, width, height, color);
        _front->next = _back;
        _back->prev = _front;
    } else {
        _back->next = new TileNode(x, y, width, height, color);
        _back->next->prev = _back;
        _back = _back->next;
    }

}

/**
 * @brief TileList::addFront - adds a TileNode to the front of the Z-Ordering
 * @param x - x coordinate of TileNode
 * @param y - y coordinate of TileNode
 * @param width - the width of the new TileNode
 * @param height - the height of the new TileNode
 * @param color - the color of the new TileNode
 */
void TileList::addFront(int x, int y, int width, int height, string color) {
    if (_back == nullptr) {
        _front = new TileNode(x, y, width, height, color);
        _back = _front;
    } else if (_front == _back && _back != nullptr) {
        _front = new TileNode(x, y, width, height, color);
        _back->prev = _front;
        _front->next = _back;
    } else {
        _front->prev = new TileNode(x, y, width, height, color);
        _front->prev->next = _front;
        _front = _front->prev;
    }
}

/**
 * @brief TileList::clear - deletes the entire linked list and sets the front
        and back pointers to nulllptr
 */
void TileList::clear() {
    if (_back != nullptr) {
       TileNode* curr = _back;
       TileNode* elim = curr;
       curr = curr->prev;
       delete elim;
       while (curr->prev != nullptr) {
           TileNode* elim = curr;
           curr = curr->prev;
           delete elim;
       }
       _front = nullptr;
       _back = nullptr;
    }
}

/**
 * @brief TileList::debug
 */
void TileList::debug() {

}

/**
 * @brief TileList::drawAll - adds all TileNodes in TileList to the screen
 * @param window - the given GWindow
 */
void TileList::drawAll(GWindow& window) const {
    if (_back != nullptr) {
       TileNode* curr = _back;
       curr->draw(window);
       while (curr->prev != nullptr) {
           curr->draw(window);
           curr = curr->prev;
       }
       curr->draw(window);
   }
}

/**
 * @brief TileList::getBack - returns the last TileNode in TileList
 * @return _back
 */
TileNode* TileList::getBack() const {
    return _back;
}
/**
 * @brief TileList::getFront - returns the first TileNode in TileList
 * @return _front
 */
TileNode* TileList::getFront() const {
    return _front;
}

/**
 * @brief TileList::highlight - changes the color of the chosen TileNode to
        yellow
 * @param x - the x-position of the mouse click
 * @param y - the y-position of the mouse click
 * @return true or false
 */
bool TileList::highlight(int x, int y) {
    TileNode* tile = _front;
    while (tile != nullptr) {
        if (tile->contains(x,y)) {
            tile->color = "yellow";
            return true;
        }
        tile = tile->next;
    }
    return false;
}

/**
 * @brief TileList::lower - moves the selected tile to the back of the stack of
        Tiles, or (in other words) to the end of the TileList
 * @param x - the x-position of the mouse click
 * @param y - the y-position of the mouse click
 * @return true or false
 */
bool TileList::lower(int x, int y) {
    TileNode* tile = _front;
    TileNode* temp;
    while(tile -> next != nullptr){
        if(tile -> contains(x,y)){
            if(tile != _front){
                temp = tile;
                tile -> prev -> next = tile -> next;
                tile -> next -> prev = tile -> prev;
                delete(tile);
                addBack(temp -> x, temp -> y, temp -> width, temp -> height, temp -> color);
                return true;
            } else {
                temp = tile;
                tile -> next -> prev = nullptr;
                _front = tile -> next;
                delete(tile);
                addBack(temp -> x, temp -> y, temp -> width, temp -> height, temp -> color);
                return true;
            }
        }
        tile = tile -> next;
    }
    return false;   // remove this
}

/**
 * @brief TileList::merge - takes the highest and lowest bounds of all Tiles at
        the selected coordinate point, then replaces them with a larger square
        with the outermost dimensions
 * @param x - the x-position of the mouse click
 * @param y - the y-position of the mouse click
 */
void TileList::merge(int x, int y) {
    TileNode* tile = _front;
    int maxX, maxY, minY, minX = 0;
    string color;
    bool first = true;

    while(tile != nullptr){
        if(tile -> contains(x,y)){
            if(first){
                minX = tile -> x;
                minY = tile -> y;
                maxX = tile -> x + tile -> width;
                maxY = tile -> y + tile -> height;
                color = tile -> color;
                first = false;
            }

            if(tile -> x < minX) minX = tile -> x;
            if(tile -> y < minY) minY = tile -> y;
            if((tile -> x + tile -> width) > maxX) maxX = tile -> x + tile -> width;
            if((tile -> y + tile -> height) > maxY) maxY = tile -> y + tile -> height;
        }
        tile = tile -> next;
    }

    int width = maxX-minX;
    int height = maxY-minY;
    removeAll(x,y);
    addFront(minX, minY, width, height, color);
}

/**
 * @brief TileList::raise - moves the selected tile to the front of the stack of
        Tiles, or (in other words) to the front of the TileList
 * @param x - x coordinate of mouse click
 * @param y - y coordinate of mouse click
 * @return
 */
bool TileList::raise(int x, int y) {
    TileNode* tile = _front;
    TileNode* temp;
    while(tile -> next != nullptr){
        if(tile -> contains(x,y)){
            if(tile != _back){
                temp = tile;
                tile -> next -> prev = tile -> prev;
                tile -> prev -> next = tile -> next;
                delete(tile);
                addFront(temp -> x, temp -> y, temp -> width, temp -> height, temp -> color);
                return true;
            } else {
                temp = tile;
                tile -> prev -> next = nullptr;
                _back = tile -> prev;
                delete(tile);
                addFront(temp -> x, temp -> y, temp -> width, temp -> height, temp -> color);
                return true;
            }
        }
        tile = tile -> next;
    }
    return false;   // remove this
}

/**
 * @brief TileList::remove - removes the tile on the top of the z-ordering at a certain coordinate
 * @param x - the x coordinate of the mouse click
 * @param y - the y coordinate of the mouse click
 * @return will return true if the x and y coordinates were on a tile
 */
bool TileList::remove(int x, int y) {
    TileNode* tile = _front;
    while (tile != nullptr) {
        if (tile->contains(x,y)) {
            if (_front == _back) {
                delete tile;
                _front = nullptr;
                _back = nullptr;
                return true;
            }
            if (tile == _front) {
                _front = _front->next;
                _front->prev = nullptr;
            } else if (tile == _back) {
                _back = _back->prev;
                _back->next = nullptr;
            } else {
                tile->next->prev = tile->prev;
                tile->prev->next = tile->next;
            }
            delete tile;
            return true;
        }
        tile = tile->next;
    }
    return false;
}

/**
 * @brief TileList::removeAll removes all the of the  tiles at a given coordinate
 * @param x - x coordinate of mouse click
 * @param y - y coordinate of mouse click
 * @return - the amount of tiles removed
 */
int TileList::removeAll(int x, int y) {
    int removed = 0;
    while (remove(x,y)) {
        removed++;
    }
    return removed;
}
