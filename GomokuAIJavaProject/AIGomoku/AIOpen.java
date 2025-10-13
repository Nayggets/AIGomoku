package AIGomoku;

public class AIOpen extends AIGomoku {

	private int depth;
	private AIBoard AIboard;
	private EvaluateFunction func;

	
	public AIOpen(char playerNb,int startDepth,EvaluateFunction func) {
		super(playerNb);
		
		this.depth = startDepth;
		this.func = func;
	}

	@Override
	public boolean playMove(BoardGame board) {
		if(AIboard == null) {
			AIboard = new AIBoard();
		}
		long startTime = System.nanoTime();
		Board boardToPlay = AIboard.getBoardToPlay(depth, func,this.playerNb);
		long endTime = System.nanoTime();
		
		long duration = endTime - startTime;
		System.out.println("GetBoardToPlay took : " + (double)duration/1000000000 + " s");
		if(boardToPlay == null) {
			throw new RuntimeException("Min max algorithm crash");
		}
		// Creer fonction statique return la position de la difference entre 2 Board
		Position pos = Board.getDifference(board.getBoard(),boardToPlay);
		int y = pos.getY();
		int x = pos.getX();
		board.placeToken(this.playerNb, x, y);
		return board.isFinished(new Position(y,x), this.playerNb);
	}


}
