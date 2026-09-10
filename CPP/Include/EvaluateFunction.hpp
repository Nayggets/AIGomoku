#pragma once
#include "Board.hpp"

class EvaluateFunction {
	public: 
    static const int MaxValue = 1000000;

	virtual int evaluateBoard(Board* board,idPlayer id) = 0;
};