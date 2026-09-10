#include "Zobrist.hpp"


long Zobrist::ZOBRIST_TABLE[Board::BOARD_SIZE][Board::BOARD_SIZE][3]; // [y][x][playerValue]
long Zobrist::PLAYER_HASH[3]; // joueur courant (3 pour avoir pas de decalage d'index (opti dans l'opti))


void Zobrist::initZobristTable(){
    std::random_device random;
    // Remplit la table aléatoire
    for (int y = 0; y < Board::BOARD_SIZE; y++) {
        for (int x = 0; x < Board::BOARD_SIZE; x++) {
            for (int p = 0; p < 3; p++) { //vide //1=joueur1, 2=joueur2
                ZOBRIST_TABLE[y][x][p] = (random() << 32) | random();
            }
        }
    }

    // inclure qu'elle est le joueur qui a calculer 
    for (int p = 0; p < 3; p++) {
        PLAYER_HASH[p] = (random() << 32) | random();
    }
}

long Zobrist::compute(Board* board) {
    long hash = 0L;
    for (int y = 0; y < Board::BOARD_SIZE; y++) {
        for (int x = 0; x < Board::BOARD_SIZE; x++) {
            int cell = board->getCell(x, y); // 0, 1, ou 2
            hash ^= ZOBRIST_TABLE[y][x][cell];
        }
    }
    return hash;
}

long Zobrist::update(long currentHash, int y, int x, int oldValue, int newValue) {
    long hash = currentHash;
    hash ^= ZOBRIST_TABLE[y][x][oldValue]; // old value xor to retire
    hash ^= ZOBRIST_TABLE[y][x][newValue]; // new value xor to add
    return hash;
}

long Zobrist::withPlayer(long hash, int player) {
    return hash ^ PLAYER_HASH[player]; // add random value of player to add currentPlayer to Hash
}

