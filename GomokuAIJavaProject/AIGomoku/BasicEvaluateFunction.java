package AIGomoku;
import java.util.ArrayList;

public class BasicEvaluateFunction implements EvaluateFunction{

	@Override
	public int evaluateBoard(Board board,int value) {
		ArrayList<Position> AllyPos = new ArrayList<Position>();
		ArrayList<Position> EnnemyPos = new ArrayList<Position>();
		for(int y = 0 ; y < Board.BOARD_SIZE ; y++) {
			for(int x = 0 ; x < Board.BOARD_SIZE ; x++) {
				if(board.getCell(y,x) == value) {
					AllyPos.add(new Position(y,x));
				}
				else if(board.getCell(y,x) != 0) {
					EnnemyPos.add(new Position(y,x));
				}
			}
		}		
		//System.out.println(AllyPos.size());
		return this.PosAssembling(AllyPos) - this.PosAssembling(EnnemyPos);
	}
	
	private int PosAssembling(ArrayList<Position> pos) {
		int currentValue = pos.size();
		
		for(Position position : pos) {
			for(Position position2 : pos) {
				if((!position.equals(position2)) && position.isNear(position2)) {
					currentValue++;
				}
			}
		}
		return currentValue;
		
	}
	
}
