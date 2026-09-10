#include "RandomAIGomoku.hpp"

bool RandomAIGomoku::playMove(BoardGame* board)
{
    std::random_device rd;
    static std::uniform_int_distribution<uint> d(0, 15);
    Position* position = new Position(0,0);

    int x =  d(rd);
    int y =  d(rd);
    position->setX(x);
    position->setY(y);
    while(!board->placeToken(playerNb, position)) {
        x =  d(rd);
        y =  d(rd);
        position->setX(x);
        position->setY(y);
    }
    bool finished = board->isFinished(position,playerNb);
    delete (position);
    return finished;
}