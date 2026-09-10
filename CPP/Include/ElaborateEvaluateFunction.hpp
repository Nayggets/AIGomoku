#pragma once
#include "EvaluateFunction.hpp"
#include "Position.hpp"
#include <memory>
#include <unordered_set>
#include <vector>



class Pattern{

    public:
        std::vector<Position*> allTokens;
        bool closedRight;
        bool closedLeft;
    Pattern(){
        this->closedRight = false;
        this->closedLeft = false;
    }

    ~Pattern(){
        for(Position* p : allTokens){
            delete(p);
        }
    }


    bool operator==(const Pattern& other) const
    {
        std::unordered_set<Position> set1;
        std::unordered_set<Position> set2;

        for (Position* p : allTokens) {
            set1.insert(*p);
        }
        for (Position* p : other.allTokens) {
            set2.insert(*p);
        }
        return set1 == set2;
    }
    
};

class ElaborateEvaluateFunction : public EvaluateFunction{

    private:
        int value[8];


    public:
    ElaborateEvaluateFunction(int value[8]) : EvaluateFunction(){
        for(int i = 0 ; i < 8 ; i++){
            this->value[i] = value[i];
        }
    }


    int evaluateBoard(Board* board, idPlayer value);

    int getValue(std::vector<Pattern*>& pattern);

    std::vector<Pattern*> getAllPattern(Board* board, int x, int y,idPlayer value,bool visitedH[Board::BOARD_SIZE][Board::BOARD_SIZE],bool visitedV[Board::BOARD_SIZE][Board::BOARD_SIZE],bool visitedD1[Board::BOARD_SIZE][Board::BOARD_SIZE],bool visitedD2[Board::BOARD_SIZE][Board::BOARD_SIZE]);

    private:

        static Pattern* getPatternTopToBottom(Board* board,int x,int y,idPlayer value,bool visitedV[Board::BOARD_SIZE][Board::BOARD_SIZE]);
        static Pattern* getPatternLeftToRight(Board* board,int x,int y,idPlayer value,bool visitedH[Board::BOARD_SIZE][Board::BOARD_SIZE]);
        static Pattern* getPatternDiagTopToBottom(Board* board,int x, int y,idPlayer value,bool visitedD1[Board::BOARD_SIZE][Board::BOARD_SIZE]);
        static Pattern* getPatternDiagBottomToTop(Board* board,int x, int y,idPlayer value,bool visitedD2[Board::BOARD_SIZE][Board::BOARD_SIZE]);



};