#include "Board.hpp"



unsigned int Board::longsNeeded = (unsigned int) std::ceil((double) (Board::BOARD_SIZE * Board::BOARD_SIZE) / 64);

unsigned int Board::longTargets[BOARD_SIZE][BOARD_SIZE];
unsigned int Board::shiftTargets[BOARD_SIZE][BOARD_SIZE];


Board::Board()
{
    this->p1Board = new long[longsNeeded];
    this->p2Board = new long[longsNeeded];
    this->lastPosition = nullptr;
    this->lastValueMove = idPlayer::nothing;
}

Board::~Board()
{
    delete this->p1Board;
    delete this->p2Board;
}

void Board::initTargets()
{
    for (unsigned int y = 0; y < Board::BOARD_SIZE; y++) {
        for (unsigned int x = 0; x < Board::BOARD_SIZE; x++) {
            int cellIndex = y * Board::BOARD_SIZE + x;
            Board::longTargets[y][x] = cellIndex / 64;
            Board::shiftTargets[y][x] = cellIndex % 64;
        }
    }
}



Board::Board(Board &board)
{ 

    this->p1Board = new long[longsNeeded];
    this->p2Board = new long[longsNeeded];
    std::copy(board.p1Board,board.p1Board + longsNeeded,this->p1Board);
    std::copy(board.p2Board,board.p2Board + longsNeeded,this->p2Board);

    this->lastPosition = std::make_unique<Position>(*board.lastPosition.get());
    this->lastValueMove = board.lastValueMove;
}

Position *Board::getLastPos()
{
    return nullptr;
}

int Board::getLastValueMove()
{
    return this->lastValueMove;
}

void Board::setCell(Position *position, idPlayer id)
{
    lastValueMove = id;

    int l = longTargets[position->getY()][position->getX()];
    int shift = shiftTargets[position->getY()][position->getX()];
    long mask = 1L << shift;

    // Efface la case sur les deux plateaux
    p1Board[l] &= ~mask;
    p2Board[l] &= ~mask;

    // Ajoute la pierre du joueur correspondant
    if (id == idPlayer::p1) p1Board[l] |= mask;
    else if (id == idPlayer::p2) p2Board[l] |= mask;
    this->lastPosition = std::make_unique<Position>(*position);
}

idPlayer Board::getCell(Position *position)
{
    int l = longTargets[position->getY()][position->getX()];
    int shift = shiftTargets[position->getY()][position->getX()];
    long mask = 1L << shift;

    if ((p1Board[l] & mask) != idPlayer::nothing) return idPlayer::p1;
    if ((p2Board[l] & mask) != idPlayer::nothing) return idPlayer::p2;
    return idPlayer::nothing;
}

idPlayer Board::getCell(int x, int y)
{
    int l = longTargets[y][x];
    int shift = shiftTargets[y][x];
    long mask = 1L << shift;

    if ((p1Board[l] & mask) != 0) return idPlayer::p1;
    if ((p2Board[l] & mask) != 0) return idPlayer::p2;
    return idPlayer::nothing;
}

bool Board::lastMoveFinished()
{
    if (lastPosition == nullptr) return false;

    int player = lastValueMove;

    long* board = (player == idPlayer::p1) ? p1Board : p2Board;

    // 4 directions : horizontal, vertical, diag, anti-diag
    return checkDirection(board,*lastPosition.get(), 1, 0)  // horizontal
        || checkDirection(board,*lastPosition.get(), 0, 1)  // vertical
        || checkDirection(board,*lastPosition.get(), 1, 1)  // diagonale ↘
        || checkDirection(board,*lastPosition.get(), 1, -1); // diagonale ↗
        
}

void Board::printBoard()
{
    Position* pos = new Position(0,0);

    std::cout << "   1  2  3  4  5  6  7  8  9  10 11 12 13 14 15" << "\n";
        for (unsigned y = 0; y < Board::BOARD_SIZE; y++) {
            std::cout << (y+1) << " ";
            if(y < 10-1){
                std::cout << " ";
            }
            pos->setY(y);
            for (unsigned x = 0; x < Board::BOARD_SIZE; x++) {
                pos->setX(x);
                int cell = this->getCell(pos);
                switch (cell) {
                    case 1:  {std::cout << "X "; break;}
                    case 2:  {std::cout << "O "; break;}
                    default: {std::cout << ". "; break;}
                };

                std::cout << " ";
            }
            std::cout << std::endl;
        }
}

Position* Board::getDifference(Board &b1, Board &b2)
{
    // Compare player 1 boards
    for (unsigned i = 0; i < longsNeeded; i++) {
        long diff = b1.p1Board[i] ^ b2.p1Board[i];
        if (diff != 0) {
            int bitIndex = std::countr_zero((uint64_t)diff);
            int posIndex = i * 64 + bitIndex;
            int y = posIndex / BOARD_SIZE;
            int x = posIndex % BOARD_SIZE;
            return new Position(x, y);
        }
    }
 
    // Compare player 2 boards
    for (unsigned i = 0; i < longsNeeded; i++) {
        long diff = b1.p2Board[i] ^ b2.p2Board[i];
        if (diff != 0) {
            int bitIndex = std::countr_zero((uint64_t)diff);
            int posIndex = i * 64 + bitIndex;
            int y = posIndex / BOARD_SIZE;
            int x = posIndex % BOARD_SIZE;
            return new Position(x, y);
        }
    }

    return nullptr; // aucun changement
}

long Board::getLine(long *board, int startX, int startY, int length, Direction dir)
{
    long line = 0L;
    int x = startX;
    int y = startY;
    int bitIndex = 0,indexLong = 0,bitPos = 0;
    for (int i = 0; i <= length; i++) {
        x = startX;
        y = startY;
        switch (dir) {
            case HORIZONTAL: x += i; break;
            case VERTICAL:   y += i; break;
            case DIAGONAL_DOWN: x += i; y += i; break;
            case DIAGONAL_UP:   x += i; y -= i; break;
        }

        // out of border
        if (x < 0 || x >= 15 || y < 0 || y >= 15) continue;

        bitIndex = y * 15 + x;
        indexLong = bitIndex / 64;
        bitPos = bitIndex % 64;
        if ((board[indexLong] & (1L << bitPos)) != 0) {
            line = line | (1L << i);
        }
    }

    return line;
}

long *Board::getBoardByPlayer(idPlayer player)
{
    if(player == idPlayer::p1){
        return this->p1Board;
    }
    else if(player == idPlayer::p2){
        return this->p2Board;
    }
    return nullptr;
}

bool Board::checkDirection(long *playerBoard, Position &position, int dx, int dy)
{
    int count = 1;
    count += countInDir(playerBoard,position, dx, dy);
    count += countInDir(playerBoard,position, -dx, -dy);
    return count >= 5;
}

unsigned int Board::countInDir(long *playerBoard, Position &position, int dx, int dy)
{
    int c = 0;
    int cx = position.getX() + dx, cy = position.getY() + dy;
    while (cx >= 0 && cy >= 0 && cx < (int)BOARD_SIZE && cy < (int)BOARD_SIZE) {
        int l = longTargets[cy][cx];
        int shift = shiftTargets[cy][cx];
        long mask = 1L << shift;
        if ((playerBoard[l] & mask) == 0) break;
        c++;
        cx += dx;
        cy += dy;
    }
    return c;
}
