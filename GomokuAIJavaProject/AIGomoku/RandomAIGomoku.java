package AIGomoku;
import java.util.Random;

public class RandomAIGomoku extends AIGomoku{

	
	private Random rand;
	
	public RandomAIGomoku(int i) {
		super((char) i);
		rand = new Random();
	}

	@Override
	public boolean playMove(BoardGame board) {		
		int x =  rand.nextInt(0,Board.BOARD_SIZE);
		int y =  rand.nextInt(0,Board.BOARD_SIZE);

		while(!board.placeToken(playerNb, x, y)) {
			x =  rand.nextInt(0,Board.BOARD_SIZE);
			y =  rand.nextInt(0,Board.BOARD_SIZE);
		}
		System.out.println("x is " + x +" y is : " + y);
		return board.isFinished(new Position(y,x), playerNb);
	}
	
	
}
