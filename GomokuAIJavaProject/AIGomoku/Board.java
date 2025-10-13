package AIGomoku;

import java.util.Arrays;

public class Board {
    private long[] p1Board;
    private long[] p2Board;
    private Position lastPos;
    private int lastValueMove;

    public Position getLastPos() {
		return lastPos;
	}

	public static final int BOARD_SIZE = 15;

    // Pré-calculs pour accéder aux bits rapidement
    static int[][] longTargets = new int[BOARD_SIZE][BOARD_SIZE];
    static int[][] shiftTargets = new int[BOARD_SIZE][BOARD_SIZE];

    static {
        for (int y = 0; y < BOARD_SIZE; y++) {
            for (int x = 0; x < BOARD_SIZE; x++) {
                int cellIndex = y * BOARD_SIZE + x;
                longTargets[y][x] = cellIndex / 64; // 64 cases par long
                shiftTargets[y][x] = cellIndex % 64;
            }
        }
    }

    public Board() {
        int longsNeeded = (int) Math.ceil((double) (BOARD_SIZE * BOARD_SIZE) / 64);
        this.p1Board = new long[longsNeeded];
        this.p2Board = new long[longsNeeded];
        this.lastPos = null;
        this.lastValueMove = -1;
    }

    public Board(Board other) {
        this.p1Board = other.p1Board.clone();
        this.p2Board = other.p2Board.clone();
        this.lastPos = other.lastPos;
        this.lastValueMove = other.lastValueMove;
    }

    /** 
     * Place une pierre sur le plateau 
     * player = 1 ou 2
     */
    public void setCell(int y, int x, int player) {
        lastPos = new Position(y, x);
        lastValueMove = player;

        int l = longTargets[y][x];
        int shift = shiftTargets[y][x];
        long mask = 1L << shift;

        // Efface la case sur les deux plateaux
        p1Board[l] &= ~mask;
        p2Board[l] &= ~mask;

        // Ajoute la pierre du joueur correspondant
        if (player == 1) p1Board[l] |= mask;
        else if (player == 2) p2Board[l] |= mask;
    }

    /**
     * Retourne 0 si vide, 1 si P1, 2 si P2
     */
    public int getCell(int y, int x) {
        int l = longTargets[y][x];
        int shift = shiftTargets[y][x];
        long mask = 1L << shift;

        if ((p1Board[l] & mask) != 0) return 1;
        if ((p2Board[l] & mask) != 0) return 2;
        return 0;
    }

    /**
     * Vérifie si le dernier coup a formé 5 alignés (dans 4 directions)
     */
    public boolean lastMoveFinished() {
        if (lastPos == null) return false;

        int x = lastPos.getX();
        int y = lastPos.getY();
        int player = lastValueMove;

        long[] board = (player == 1) ? p1Board : p2Board;

        // 4 directions : horizontal, vertical, diag, anti-diag
        return checkDirection(board, x, y, 1, 0)  // horizontal
            || checkDirection(board, x, y, 0, 1)  // vertical
            || checkDirection(board, x, y, 1, 1)  // diagonale ↘
            || checkDirection(board, x, y, 1, -1); // diagonale ↗
    }

    private boolean checkDirection(long[] playerBoard, int x, int y, int dx, int dy) {
        int count = 1;
        count += countInDir(playerBoard, x, y, dx, dy);
        count += countInDir(playerBoard, x, y, -dx, -dy);
        return count >= 5;
    }

    private int countInDir(long[] playerBoard, int x, int y, int dx, int dy) {
        int c = 0;
        int cx = x + dx, cy = y + dy;
        while (cx >= 0 && cy >= 0 && cx < BOARD_SIZE && cy < BOARD_SIZE) {
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

    public void printBoard() { // affichage bug pas corrigé TODO
        System.out.println("   1 2 3 4 5 6 7 8 9 10 11 12 13 14 15");
        for (int y = 0; y < BOARD_SIZE; y++) {
            System.out.printf("%2d ", y + 1);
            for (int x = 0; x < BOARD_SIZE; x++) {
                int cell = getCell(y, x);
                System.out.print(switch (cell) {
                    case 1 -> "X ";
                    case 2 -> "O ";
                    default -> ". ";
                });
            }
            System.out.println();
        }
    }
    
    public static Position getDifference(Board b1, Board b2) {
        // Compare player 1 boards
        for (int i = 0; i < b1.p1Board.length; i++) {
            long diff = b1.p1Board[i] ^ b2.p1Board[i];
            if (diff != 0) {
                int bitIndex = Long.numberOfTrailingZeros(diff);
                int posIndex = i * 64 + bitIndex;
                int y = posIndex / BOARD_SIZE;
                int x = posIndex % BOARD_SIZE;
                return new Position(y, x);
            }
        }

        // Compare player 2 boards
        for (int i = 0; i < b1.p2Board.length; i++) {
            long diff = b1.p2Board[i] ^ b2.p2Board[i];
            if (diff != 0) {
                int bitIndex = Long.numberOfTrailingZeros(diff);
                int posIndex = i * 64 + bitIndex;
                int y = posIndex / BOARD_SIZE;
                int x = posIndex % BOARD_SIZE;
                return new Position(y, x);
            }
        }

        return null; // aucun changement
    }
    
    public long[] getBoardByPlayer(int player) {
    	if(player == 1) {
    		return getPlayer1Board();
    	}
    	return getPlayer2Board();
    }

    
    
    @Override
    public Board clone() {
        Board copy = new Board();

        // copie profonde des deux bitboards
        copy.p1Board = this.p1Board.clone();
        copy.p2Board = this.p2Board.clone();
        return copy;
    }
    
    public enum Direction {
        HORIZONTAL, VERTICAL, DIAGONAL_DOWN, DIAGONAL_UP
    }
    
    public long getLine(long[] board, int startX, int startY, int length, Direction dir) {
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


	// Getters
    public long[] getPlayer1Board() { return p1Board; }
    public long[] getPlayer2Board() { return p2Board; }

    @Override
    public String toString() {
        return "Board[p1=" + Arrays.toString(p1Board) + ", p2=" + Arrays.toString(p2Board) + "]";
    }
}