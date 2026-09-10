#pragma once
#include "EvaluateFunction.hpp"
#include "BoardGame.hpp"
#include "ThreadPool.hpp"
#include "Zobrist.hpp"
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <thread>
#include <future>

class Result {
    public:
        int value;
        Board* board;
        Result(int value, Board* board) {
            this->value = value;
            this->board = board;
        }
        ~Result();
};

class AIBoard {

    
	public:
    AIBoard() {
	}
    Board* getBoardToPlay(int depth, EvaluateFunction* func,idPlayer id);

	
    private: 
        static EvaluateFunction* evaluate;
	    static std::unordered_map<long, int> TranspositionTable;
	    static int radius;
        static std::vector<Position*> generateMoves(Board* board);
	    static bool isRelevantMove(int x, int y, Board* board);
	
	
    private:
        static bool inBounds(int x, int y);
        Board* minimaxAlgorithm(Board* board,int depth,idPlayer nbPlayer);
        
        static int minimaxAlgorithmAux(Board* board,int depth,int alpha,int beta,idPlayer nbPlayer,long hash);
	
};