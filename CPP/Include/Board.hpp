#pragma once
#include "Position.hpp"
#include <memory>
#include <cmath>
#include <iterator>
#include <iostream>
#include <bit>


enum idPlayer
{
    p1 = 0x01,
    p2 = 0x02,
    nothing = 0x00
};

enum Direction {
    HORIZONTAL, 
    VERTICAL, 
    DIAGONAL_DOWN, 
    DIAGONAL_UP
};

class Board{
    public:
        //Constructor
        Board();
        ~Board();
        Board(Board& board); // clone

	    static const unsigned int BOARD_SIZE = 15;
        Position* getLastPos();
        int getLastValueMove();
        void setCell(Position* position,idPlayer id);
        idPlayer getCell(Position* position);
        idPlayer getCell(int x, int y); //Optimisation for multiple call where x and y are changing a lot
        bool lastMoveFinished();
        void printBoard();
        static Position* getDifference(Board& b1, Board& b2);
        long getLine(long* board, int startX, int startY, int length, Direction dir);
        
        // GETTER for board
        long* getBoardByPlayer(idPlayer player);
        long* getPlayer1Board() { return p1Board; }
        long* getPlayer2Board() { return p2Board; }

    private:
        static unsigned int longsNeeded;
        static unsigned int longTargets[BOARD_SIZE][BOARD_SIZE];
        static unsigned int shiftTargets[BOARD_SIZE][BOARD_SIZE];
        static void initTargets();
        inline static bool initialized = (initTargets(), true);
        long* p1Board;
        long* p2Board;
        std::unique_ptr<Position> lastPosition;
        idPlayer lastValueMove;
        bool checkDirection(long* playerBoard, Position& position, int dx, int dy);
        unsigned int countInDir(long* playerBoard, Position& position, int dx, int dy);

};

