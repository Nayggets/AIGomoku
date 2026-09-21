#pragma once
#include "EvaluateFunction.hpp"
#include "BoardGame.hpp"

class JumpEvaluateFunction : public EvaluateFunction{
	
	private: 
        int value[6];

	
	public: 
		JumpEvaluateFunction(int value[6]) {
			for(int i = 0 ; i < 6 ; i++){
				this->value[i] = value[i];
			}	
		}


	int evaluateBoard(Board* board, idPlayer value);
	
	
	
    int evaluateLine(long playerBits, long opponentBits,int* nbOpenThree,int* nbFour);



};