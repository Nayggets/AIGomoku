#include "BoardGame.hpp"
#include <random>
class Zobrist { // l'enfer sur terre

    private:
    static long ZOBRIST_TABLE[Board::BOARD_SIZE][Board::BOARD_SIZE][3]; // [y][x][playerValue]
    static long PLAYER_HASH[3]; // joueur courant (3 pour avoir pas de decalage d'index (opti dans l'opti))
    static void initZobristTable();

    inline static bool initialized = (initZobristTable(), true);


    public: 
    static long compute(Board* board);
    
    static long update(long currentHash, int y, int x, int oldValue, int newValue);

    static long withPlayer(long hash, int player);

};