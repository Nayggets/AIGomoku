#include "EvaluateFunction.hpp"
#include <vector>

class BasicEvaluateFunction : public EvaluateFunction{

	public:
        int evaluateBoard(Board* board,idPlayer id); 
	
	private: 
        int PosAssembling(std::vector<Position*> pos);	
};