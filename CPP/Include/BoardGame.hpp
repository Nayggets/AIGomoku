#pragma once
#include "Board.hpp"


class BoardGame{
    private:
        static BoardGame* instance;
        std::unique_ptr<Board> board;
        BoardGame();
        unsigned int numberOfMovesPlayed;

	public: 
        static BoardGame* getInstance();
        bool isFinished(Position* pos, idPlayer turn);
	    bool placeToken(idPlayer value, Position* position);
	    Board* getBoard();
	
};