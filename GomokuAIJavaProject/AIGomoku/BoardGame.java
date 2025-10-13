package AIGomoku;

public class BoardGame {

	
	private static BoardGame instance;
	private Board board;
	public static int numberOfMovesPlayed = 0;
	
	public static BoardGame getInstance(){
		if(instance == null) {
			instance = new BoardGame();
		}
		return instance;
	}
	
	private BoardGame() {
		board = new Board();
	}
	

	
	
	public boolean isFinished(Position pos, int turn) {
		
		int x = pos.getX();
		int y = pos.getY();
		int i = x;
		int nbTokenAllign = 0;
		while(i >= 0 && this.board.getCell(y,i) == turn) {
			nbTokenAllign++;
			i--;
		}
		i = x+1;
		while(i < Board.BOARD_SIZE && this.board.getCell(y,i) == turn) {
			nbTokenAllign++;
			i++;
		}
		if(nbTokenAllign >= 5) {
			return true;
		}
		nbTokenAllign = 0;
		int j = y;
		while(j >= 0 && this.board.getCell(j,x) == turn) {
			nbTokenAllign++;
			j--;
		}
		j = y+1;
		while(j < Board.BOARD_SIZE && this.board.getCell(j,x) == turn) {
			nbTokenAllign++;
			j++;
		}
		if(nbTokenAllign >= 5) {
			return true;
		}
		nbTokenAllign = 0;
		i = x;
		j = y;
		while(j >= 0 && i >= 0 && this.board.getCell(j,i) == turn) {
			nbTokenAllign++;
			j--;
			i--;
		}
		i = x+1;
		j = y+1;
		while(j < Board.BOARD_SIZE && i < Board.BOARD_SIZE && this.board.getCell(j,i) == turn) {
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
		while(j >= 0 && i < Board.BOARD_SIZE && this.board.getCell(j,i) == turn) {
			nbTokenAllign++;
			j--;
			i++;
		}
		i = x-1;
		j = y+1;
		while(j < Board.BOARD_SIZE && i >= 0 && this.board.getCell(j,i) == turn) {
			nbTokenAllign++;
			j++;
			i--;
		}
		if(nbTokenAllign >= 5) {
			return true;
		}
		return false;
	}
	
	
	public boolean placeToken(int value, int positionX, int positionY){
		
		if(this.board.getCell(positionY,positionX) != 0) {
			return false;
		}
		if(value != 1 && value != 2) {
			return false;
		}
		this.board.setCell(positionY,positionX, value);
		numberOfMovesPlayed++;
		return true;
	}
	
	
	
	public Board getBoard() {
		return board;
	}
	
	

	
	
}
