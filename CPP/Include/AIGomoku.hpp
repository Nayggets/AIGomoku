#pragma once
#include "BoardGame.hpp"

class AIGomoku {

	
	protected: 
        idPlayer playerNb;
	
	public: 
        AIGomoku(idPlayer playerNb) {
            this->playerNb = playerNb;
        }
        virtual bool playMove(BoardGame* board) = 0;
	
	
	
};