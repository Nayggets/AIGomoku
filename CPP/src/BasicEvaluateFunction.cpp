#include "BasicEvaluateFunction.hpp"

int BasicEvaluateFunction::evaluateBoard(Board* board, idPlayer id)
{
		std::vector<Position*> AllyPos;
		std::vector<Position*> EnnemyPos;
		for(int y = 0 ; y < Board::BOARD_SIZE ; y++) {
			for(int x = 0 ; x < Board::BOARD_SIZE ; x++) {
				if(board->getCell(y,x) == id) {
					AllyPos.push_back(new Position(x,y));
				}
				else if(board->getCell(y,x) != 0) {
					EnnemyPos.push_back(new Position(x,y));
				}
			}
		}		
		//System.out.println(AllyPos.size());
        int result = this->PosAssembling(AllyPos) - this->PosAssembling(EnnemyPos);
        for(auto p : AllyPos){
            delete(p);
        }
        for(auto p : EnnemyPos){
            delete(p);
        }
		return result;
}

int BasicEvaluateFunction::PosAssembling(std::vector<Position*> pos)
{
    int currentValue = pos.size();
    
    for(auto position : pos) {
        for(auto position2 : pos) {
            if((position == position2) && position->isNear(position2)) {
                currentValue++;
            }
        }
    }
    return currentValue;    
}
