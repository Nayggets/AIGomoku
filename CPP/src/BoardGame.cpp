#include "BoardGame.hpp"

BoardGame* BoardGame::instance = nullptr;;


BoardGame::BoardGame()
{
    this->board = std::make_unique<Board>();
    numberOfMovesPlayed = 0;
}

BoardGame* BoardGame::getInstance()
{
    if(instance == nullptr){
        instance = new BoardGame();
    }
    return instance;
}

bool BoardGame::isFinished(Position* pos, idPlayer turn)
{
        unsigned int x = pos->getX();
		unsigned int y = pos->getY();
        Position* position = new Position(*pos);
        std::cout << "Test : " << this->board->getCell(pos) << std::endl;
        if(this->board->getCell(pos) != turn){
            std::cout << "HUGE BUG" << std::endl;
        }
		int i = x;
		unsigned int nbTokenAllign = 0;
		while(i >= 0 && this->board->getCell(i,y) == turn) {
			nbTokenAllign++;
			i--;
            position->setX(i);
		}
		i = x+1;
		while(i < (int)Board::BOARD_SIZE && this->board->getCell(i,y) == turn) {
			nbTokenAllign++;
			i++;
		} 
		if(nbTokenAllign >= 5) {
			return true;
		}
		nbTokenAllign = 0;
		int j = y;
		while(j >= 0 && this->board->getCell(x,j) == turn) {
			nbTokenAllign++;
			j--;
		}
		j = y+1;
        
		while(j < (int)Board::BOARD_SIZE && this->board->getCell(x,j) == turn) {
			nbTokenAllign++;
			j++;
		}
        std::cout << nbTokenAllign << " value was " << std::endl;
		if(nbTokenAllign >= 5) {
			return true;
		}
		nbTokenAllign = 0;
		i = x;
		j = y;
		while(j >= 0 && i >= 0 && this->board->getCell(i,j) == turn) {
			nbTokenAllign++;
			j--;
			i--;
		}
		i = x+1;
		j = y+1;
		while(j < (int)Board::BOARD_SIZE && i < (int)Board::BOARD_SIZE && this->board->getCell(i,j) == turn) {
			nbTokenAllign++;
			i++;
			j++;
		}
		if(nbTokenAllign >= 5) {
			return true;
		}
		
		i = x;
		j = y;
		nbTokenAllign = 0;
		while(j >= 0 && i < (int)Board::BOARD_SIZE && this->board->getCell(i,j) == turn) {
			nbTokenAllign++;
			j--;
			i++;
		}
		i = x-1;
		j = y+1;
		while(j < (int)Board::BOARD_SIZE && i >= 0 && this->board->getCell(i,j) == turn) {
			nbTokenAllign++;
			j++;
			i--;
		}
		if(nbTokenAllign >= 5) {
			return true;
		}
		return false;
}

bool BoardGame::placeToken(idPlayer value, Position* position)
{
		if(this->board->getCell(position) != 0) {
			return false;
		}
		if(value != idPlayer::p1 && value != idPlayer::p2) {
			return false;
		}
		this->board->setCell(position, value);
		numberOfMovesPlayed++;
		return true;
}

Board* BoardGame::getBoard()
{
    return this->board.get();
}
