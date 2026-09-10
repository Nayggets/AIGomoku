#include "Position.hpp"

Position::Position(unsigned int x, unsigned int y)
{
    this->x = x;
    this->y = y;
}

Position::Position(const Position &position) 
{
    this->x = position.x;
    this->y = position.y;
}

unsigned int Position::getX() const
{
    return this->x;
}

unsigned int Position::getY() const
{
    return this->y;
}

void Position::setY(unsigned int y)
{
    this->y = y;
}

void Position::setX(unsigned int x)
{
    this->x = x;
}

bool Position::isNear(Position* pos)
{
    unsigned int posy = pos->getY();
    unsigned int posx = pos->getX();
    if((std::abs((int)(posx - x)) == 1 || std::abs((int)(posx - x)) == 0) && (std::abs((int)(posy - x)) == 1 || std::abs((int)(posy - y)) == 0)) {
        return true;
    }
    return false;
}

bool Position::operator==(const Position& p1) const
{
    if(this->x == p1.x && p1.y == this->y){
        return true;
    }
    return false;
}