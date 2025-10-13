package AIGomoku;

import java.util.Random;


public final class Zobrist { // l'enfer sur terre

    private static final long[][][] ZOBRIST_TABLE; // [y][x][playerValue]
    private static final long[] PLAYER_HASH = new long[3]; // joueur courant (3 pour avoir pas de decalage d'index (opti dans l'opti))
    private static final Random random = new Random(123456789L);

    static {
        ZOBRIST_TABLE = new long[Board.BOARD_SIZE][Board.BOARD_SIZE][3];

        // Remplit la table aléatoire
        for (int y = 0; y < Board.BOARD_SIZE; y++) {
            for (int x = 0; x < Board.BOARD_SIZE; x++) {
                for (int p = 0; p < 3; p++) { //vide //1=joueur1, 2=joueur2
                    ZOBRIST_TABLE[y][x][p] = random.nextLong();
                }
            }
        }

        // inclure qu'elle est le joueur qui a calculer 
        for (int p = 0; p < 3; p++) {
            PLAYER_HASH[p] = random.nextLong();
        }
    }

    public static long compute(Board board) {
        long hash = 0L;
        for (int y = 0; y < Board.BOARD_SIZE; y++) {
            for (int x = 0; x < Board.BOARD_SIZE; x++) {
                int cell = board.getCell(y, x); // 0, 1, ou 2
                hash ^= ZOBRIST_TABLE[y][x][cell];
            }
        }
        return hash;
    }
    
    public static long update(long currentHash, int y, int x, int oldValue, int newValue) {
        long hash = currentHash;
        hash ^= ZOBRIST_TABLE[y][x][oldValue]; // old value xor to retire
        hash ^= ZOBRIST_TABLE[y][x][newValue]; // new value xor to add
        return hash;
    }

    public static long withPlayer(long hash, int player) {
        return hash ^ PLAYER_HASH[player]; // add random value of player to add currentPlayer to Hash
    }

}