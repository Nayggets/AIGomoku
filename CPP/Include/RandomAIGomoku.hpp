#include "AIGomoku.hpp"
#include <random>

class RandomAIGomoku : public AIGomoku{

		
	public :
        RandomAIGomoku(idPlayer i) : AIGomoku(i){}

	    bool playMove(BoardGame* board);
	
	
};