/**
 * Assignment: Fractals
 * Partners: Zackery Bangs, Alex Little
 * Section Leader: Evin Yang
 * File Name: fractals.cpp
 * ------------------------------------
 *
 * This file runs two seperate programs, Draw Sierpinski Triangle and Flood Fill.
 *
 *      Draw Sierpinski Triangle does what it sounds like; using recursion, the program creates a pattern of
 *      triangles and draws them on the screen, following the fractal pattern by Waclaw Sierpinski.
 *
 *      Flood Fill allows the user to change the color of a rectangle from a certain color to their preference.
 *      The program runs recursively, checking the same cases for each pixel.
 *
 *      Both of these programs use graphics as visual representations of recursion.
 *
 * This file only works parallel to fractalgui.cpp
 */

#include "fractals.h"
#include "gwindow.h"
#include "gobjects.h"

using namespace std;

void drawTriangle(GWindow& window, double x, double y, double size);

/**
 * @brief drawSierpinskiTriangle - This function draws a Sierpinski triangle at the specified coordinates, of the
 *          specified size, and of the specified order (as determined by the parameters)
 * @param window - the program window, where the triangle is displayed
 * @param x - x-coordinate position of the triangle
 * @param y - y-coordinate position of the triangle
 * @param size - side length of the triangle
 * @param order - proportionate to the triangles that will reside within this instance of triangle
 */
void drawSierpinskiTriangle(GWindow& window, double x, double y, double size, int order) {
    if(order < 0){
        throw "Invalid number.";
    } else if(order == 0) {
        window.drawLine(0, 0, 0, 0);
    } else if(order == 1) {
        drawTriangle(window, x, y, size);
    } else {
        drawSierpinskiTriangle(window, x, y, size/2, order - 1);
        drawSierpinskiTriangle(window, x + size/2, y, size/2, order - 1);
        drawSierpinskiTriangle(window, x + size/4, y + (sqrt(3)/4)*size, size/2, order - 1);
    }

}

/**
 * @brief drawTriangle - draws a single triangle at the specified coordinate point, of the specified
 *          size
 * @param window - the program window, where the triangle is displayed
 * @param x - x-coordinate of the triangle
 * @param y - y-coordinate of the triangle
 * @param size - side length of the triangle
 */
void drawTriangle(GWindow& window, double x, double y, double size){
    double xCoordA = x;
    double yCoordA = y;

    double xCoordB = x + size;
    double yCoordB = y;

    double xCoordC = x + 0.5 * size;
    double yCoordC = y + (sqrt(3.0)/2) * size;

    window.setFillColor(2);
    window.drawLine(xCoordA, yCoordA, xCoordB, yCoordB);
    window.drawLine(xCoordB, yCoordB, xCoordC, yCoordC);
    window.drawLine(xCoordC, yCoordC, xCoordA, yCoordA);
}

//------------------------------------------------------------------------------------------------
int floodFillRec(GWindow& window, int x, int y, int color, int origColor, int& count);

/**
 * @brief floodFill - changes the color of rectangles within the window to the specified new color
 * @param window - the program window, where the rectangles are displayed
 * @param x - x-coordinate of pointer
 * @param y - y-coordinate of pointer
 * @param color - the color pixels are being changed to
 * @return total number of pixels changed in the rectangle
 */
int floodFill(GWindow& window, int x, int y, int color) {
      int count = 0;
      int origColor = window.getPixel(x, y);
      return floodFillRec(window, x, y, color, origColor, count);
}

/**
 * @brief floodFillRec - changes the color of rectangles within the window to the specified new color
 * @param window - the program window, where the rectangles are displayed
 * @param x - x-coordinate of pointer
 * @param y - y-coordinate of pointer
 * @param color - the color pixels are being changed to
 * @param count - number of pixels changed in the rectangle
 * @return count
 */
int floodFillRec(GWindow& window, int x, int y, int color, int origColor, int& count){
    bool north = window.inCanvasBounds(x, y - 1);
    if(north && !(window.getPixel(x, y - 1) == color) && (window.getPixel(x, y - 1) == origColor)){
        window.setPixel(x, y - 1, color);
        count++;
        floodFillRec(window, x, y - 1, color, origColor, count);
    }

    bool south = window.inCanvasBounds(x, y + 1);
    if(south && !(window.getPixel(x, y + 1) == color) && (window.getPixel(x, y + 1) == origColor)){
        window.setPixel(x, y + 1, color);
        count++;
        floodFillRec(window, x, y + 1, color, origColor, count);
    }

    bool west = window.inCanvasBounds(x - 1, y);
    if(west && !(window.getPixel(x - 1, y) == color) && (window.getPixel(x - 1, y) == origColor)){
        window.setPixel(x - 1, y, color);
        count++;
        floodFillRec(window, x - 1, y, color, origColor, count);
    }

    bool east = window.inCanvasBounds(x + 1, y);
    if(east && !(window.getPixel(x + 1, y) == color) && window.getPixel(x + 1, y) == origColor){
        window.setPixel(x + 1, y, color);
        count++;
        floodFillRec(window, x + 1, y, color, origColor, count);
    }

    return count;
}
