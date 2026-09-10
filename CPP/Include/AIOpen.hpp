#include "AIGomoku.hpp"
#include "AIBoard.hpp"
#include "EvaluateFunction.hpp"
#include <chrono>

class AIOpen : public AIGomoku {

	private :
    int depth;
	AIBoard* AIboard;
	EvaluateFunction* func;

	
	public: 
	AIOpen(idPlayer playerNb,int startDepth,EvaluateFunction* func) : AIGomoku(playerNb) {
		
		this->depth = startDepth;
		this->func = func;
	}

	bool playMove(BoardGame* board) {
		if(AIboard == nullptr) {
			AIboard = new AIBoard();
		}
		auto startTime = std::chrono::steady_clock::now();
		Board* boardToPlay = AIboard->getBoardToPlay(depth, func,this->playerNb);
		auto endTime = std::chrono::steady_clock::now();
		boardToPlay->printBoard();
		auto elapsed =std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();		
		std::cout << "GetBoardToPlay took : " << elapsed/1000000000 << " s" << std::endl;
		if(boardToPlay == nullptr) {
			throw std::runtime_error("Min max algorithm crash");
		}
		// Creer fonction statique return la position de la difference entre 2 Board
		Position* pos = Board::getDifference(*board->getBoard(),*boardToPlay);
		delete(boardToPlay);
		board->placeToken(this->playerNb, pos);
		bool result = board->isFinished(pos, this->playerNb);
		delete(pos);
		return result;
	}


};