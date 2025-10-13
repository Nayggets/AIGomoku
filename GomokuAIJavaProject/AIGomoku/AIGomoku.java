package AIGomoku;

public abstract class AIGomoku {

	
	protected char playerNb;
	
	public AIGomoku(char playerNb) {
		this.playerNb = playerNb;
	}
	
	public abstract boolean playMove(BoardGame board);
	
	
	
	
}
