#include "AIBoard.hpp"

int AIBoard::radius = 2;
EvaluateFunction* AIBoard::evaluate;
std::unordered_map<long, int> AIBoard::TranspositionTable;

Board* AIBoard::getBoardToPlay(int depth, EvaluateFunction* func, idPlayer id)
{
    this->evaluate = func;
    TranspositionTable.clear();
    std::cout << "ID is : " << id << std::endl;
    try {
        return this->minimaxAlgorithm(BoardGame::getInstance()->getBoard(),depth,id);
    } catch (std::runtime_error e) { // not suppose to
        std::cout << e.what() << std::endl;
    }
    return nullptr;

}

std::vector<Position *> AIBoard::generateMoves(Board* board)
{
    std::vector<Position*> moves;

    for (int y = 0; y < Board::BOARD_SIZE; y++) {
        for (int x = 0; x < Board::BOARD_SIZE; x++) {
            
            // empty
            if (board->getCell(x, y) == 0) {

                // If rock is near
                if (isRelevantMove(x, y, board)) {
                    moves.push_back(new Position(x, y));
                }
            }
        }
    }

    return moves;
}

bool AIBoard::isRelevantMove(int x, int y, Board* board)
{
    for (int dy = -radius; dy <= radius; dy++) {
        for (int dx = -radius; dx <= radius; dx++) {
            int nx = x + dx;
            int ny = y + dy;

            if (inBounds(nx,ny) && board->getCell(nx, ny) != 0) {
                return true;
            }
        }
    }
    return false;
}

bool AIBoard::inBounds(int x, int y)
{
    return (x >= 0 && x < Board::BOARD_SIZE &&
            y >= 0 && y < Board::BOARD_SIZE);
}

Board* AIBoard::minimaxAlgorithm(Board *board, int depth, idPlayer nbPlayer)
{
    std::cout << "Launch with " << depth << std::endl;
    if(depth == 0) {
        throw std::runtime_error("Error in depth selection");
    }
    unsigned int nproc = std::thread::hardware_concurrency();

    ThreadPool pool(nproc);
    std::vector<std::future<Result*>> futures;
    int lastValue = std::numeric_limits<int>::min();
    Board* boardToModify = new Board(*board);
    Board* toReturn = nullptr;
    std::vector<Position*> pos = AIBoard::generateMoves(board);
    if(pos.empty()) {
        std::cout << "Is empty" << std::endl;
        Position* positionNull = new Position(7,7);
        boardToModify->setCell(positionNull, nbPlayer);
        return boardToModify;
    }
    long baseHash = Zobrist::compute(board);
    int x = 0;
    int y = 0;
    for(Position* p : pos) {
        y = p->getY();
        x = p->getX();
        if(boardToModify->getCell(x, y) == idPlayer::nothing) {
            Board* boardCopy = new Board(*boardToModify);
            boardCopy->setCell(p,nbPlayer);
            long newHash = Zobrist::update(baseHash, y, x, 0, nbPlayer);
            futures.push_back(pool.submit(
                [this, boardCopy, depth, nbPlayer,newHash]()
                {
                    
                    int value = -minimaxAlgorithmAux(boardCopy, depth-1,std::numeric_limits<int>::min(),std::numeric_limits<int>::max(),(nbPlayer == idPlayer::p1) ? idPlayer::p2 : idPlayer::p1,newHash);
                    return new Result(value,boardCopy);
                }
            ));
        }
    }
    for (std::future<Result*>& f : futures) {
        f.wait();
    }

    if(futures.empty()) {
        std::cout << "PROBLEMS WITH THREEADING" << std::endl;
    }
    for (std::future<Result*>& f : futures) {
        Result* score = f.get();   // Future.get() attend le résultat
        if(score->value > lastValue || (score->value == lastValue && evaluate->evaluateBoard(score->board, nbPlayer) > evaluate->evaluateBoard(toReturn, nbPlayer))) {
            toReturn = new Board(*(score->board));
            lastValue = score->value;
        }
        delete(score);
    }

    if(toReturn == nullptr) {
        std::cout << "Huge mistakes" << std::endl;
    }
    std::cout << "Last value is : " << lastValue << std::endl;
    
    return toReturn;
}

int AIBoard::minimaxAlgorithmAux(Board *board, int depth, int alpha, int beta, idPlayer nbPlayer, long hash)
{
    long key = Zobrist::withPlayer(hash, nbPlayer);
    if(depth == 0 || board->lastMoveFinished()) {
        if(TranspositionTable.find(key) != TranspositionTable.end()){
            return TranspositionTable[key];
        }
        int value = evaluate->evaluateBoard(board,nbPlayer);
        TranspositionTable[key] = value;
        return value;
    }
    std::vector<Position*> pos = AIBoard::generateMoves(board);
    int value = std::numeric_limits<int>::min();
    int y = 0;
    int x = 0;
    long newHash = 0;
    for(Position* p : pos) {

        if(board->getCell(p) == idPlayer::nothing) {
            board->setCell(p,nbPlayer);
            newHash = Zobrist::update(hash, y, x, 0, nbPlayer) ;
            value = std::max(value,-AIBoard::minimaxAlgorithmAux(board,depth-1,-beta,-alpha,(nbPlayer == idPlayer::p1) ? idPlayer::p2 : idPlayer::p1,newHash));
            
            board->setCell(p, idPlayer::nothing);
            alpha = std::max(alpha, value);
            if(alpha >= beta) {
                break;
            }
        }
    }
    if(TranspositionTable.find(key) != TranspositionTable.end()){
        return TranspositionTable[key];
    }

    TranspositionTable[key] = value;
    return value;
}

Result::~Result()
{
    delete(this->board);
}
