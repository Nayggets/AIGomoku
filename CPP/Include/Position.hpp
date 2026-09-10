#pragma once
#include <cstdlib>
#include <functional>

class Position{
    public:
        Position(unsigned int x, unsigned int y);
        Position(const Position& position);
        unsigned int getX() const;
        unsigned int getY() const;
        void setY(unsigned int y);
        void setX(unsigned int x);
        bool isNear(Position* pos);
	    static const unsigned int BOARD_SIZE = 15;
 
    bool operator==(const Position& p1) const;
    private:
        unsigned int x;
        unsigned int y;
};

template <>
struct std::hash<Position> {
    size_t operator()(const Position& p) const noexcept {
        return std::hash<unsigned int>{}(p.getX()) ^
                (std::hash<unsigned int>{}(p.getY()) << 1);
    }
};