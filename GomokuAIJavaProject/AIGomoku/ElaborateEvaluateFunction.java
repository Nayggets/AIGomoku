package AIGomoku;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Objects;

public class ElaborateEvaluateFunction implements EvaluateFunction{
	
	private int[] value;

	
	public ElaborateEvaluateFunction(int[] value) {
		this.value = value;
	}

	static class Pattern{
		ArrayList<Position> allTokens;
		boolean closedRight;
		boolean closedLeft;
		Pattern(){
			allTokens = new ArrayList<Position>();
			this.closedRight = false;
			this.closedLeft = false;
		}
		@Override
		public boolean equals(Object obj) {
		    if (this == obj) return true;
		    if (obj == null || getClass() != obj.getClass()) return false;
		    Pattern other = (Pattern) obj;
		    return new HashSet<>(allTokens).equals(new HashSet<>(other.allTokens));
		}

		@Override
		public int hashCode() {
		    return Objects.hash(new HashSet<>(allTokens));
		}
		@Override
		public String toString() {
			return "Pattern [allTokens=" + allTokens.toString() + ", closedRight=" + closedRight
					+ ", closedLeft=" + closedLeft + "]";
		}
		
		
	}
	
	@Override
	public int evaluateBoard(Board board, int value) {
		boolean[][] visitedH = new boolean[Board.BOARD_SIZE][Board.BOARD_SIZE];
		boolean[][] visitedV = new boolean[Board.BOARD_SIZE][Board.BOARD_SIZE];
		boolean[][] visitedD1 = new boolean[Board.BOARD_SIZE][Board.BOARD_SIZE];
		boolean[][] visitedD2 = new boolean[Board.BOARD_SIZE][Board.BOARD_SIZE];
		List<Pattern> allyPattern = new ArrayList<>();
		List<Pattern> ennemyPattern = new ArrayList<>();
		for(int y = 0 ; y < Board.BOARD_SIZE ; y++) {
			for(int x = 0 ; x < Board.BOARD_SIZE ; x++) {
				if(board.getCell(y, x) != 0) {
					if(board.getCell(y, x) == value) {
						allyPattern.addAll(getAllPattern(board,x,y,value,visitedH,visitedV,visitedD1,visitedD2));
					}
					else {
						ennemyPattern.addAll(getAllPattern(board,x,y,(value == 1) ? 2 : 1,visitedH,visitedV,visitedD1,visitedD2));
					}
				}
				
			}
		}
		int valueCurrentPlayer = getValue(allyPattern);
		int valueEnnemyPlayer = getValue(ennemyPattern);
		
		return valueCurrentPlayer - valueEnnemyPlayer ;
	}
	
	private int getValue(List<Pattern> pattern) {
		int value = 0;
		int valueTotal = 0;
		int size = 0;
		Pattern toCount = null;
		for(int i = 0 ; i < pattern.size(); i++) {
			toCount = pattern.get(i);
			if(toCount.allTokens.size() >= 5) {
				return this.value[7]; 
			}
			if(toCount.closedLeft && toCount.closedRight) {
				continue;
			}
			size = toCount.allTokens.size();
			switch(size) 
			{
			/*
				case 1:
				{
					value +=  this.value[0];
					break;
				}
			*/
				case 2:
				{
					if(toCount.closedLeft || toCount.closedRight) {
						value = this.value[1];
					}
					else {
						value = this.value[2];
					}
					break;
				}
				case 3:
				{
					if(toCount.closedLeft || toCount.closedRight) {
						value = this.value[3];
					}else {
						value = this.value[4];
					}
					break;
				}
				case 4:
				{
					if(toCount.closedLeft || toCount.closedRight) {
						value = this.value[5];
					}
					else {
						return this.value[6];
					}
					break;
				}
				
			}
			valueTotal += value;
		}
		return valueTotal;
	}
	
	private ArrayList<Pattern> getAllPattern(Board board, int x, int y,int value,boolean[][] visitedH,boolean[][] visitedV,boolean[][] visitedD1,boolean[][] visitedD2) {
		ArrayList<Pattern> toReturn = new ArrayList<>();
		Pattern pattern = null;
		
		if(!visitedH[y][x]) {
			pattern = getPatternLeftToRight(board, x, y, value,visitedH);
			if(pattern.allTokens.size() >= 5) {
				toReturn.clear();
				toReturn.add(pattern);
				return toReturn;
			}
			toReturn.add(pattern);
		}
		if(!visitedV[y][x]) {
			pattern = getPatternTopToBottom(board,x,y,value,visitedV);
			if(pattern.allTokens.size() >= 5) {
				toReturn.clear();
				toReturn.add(pattern);
				return toReturn;
			}
			toReturn.add(pattern);
		}
		if(!visitedD1[y][x]) {
			pattern = getPatternDiagTopToBottom(board, x, y, value,visitedD1);
			if(pattern.allTokens.size() >= 5) {
				toReturn.clear();
				toReturn.add(pattern);
				return toReturn;
			}
			toReturn.add(pattern);
		}
		if(!visitedD2[y][x]) {
			pattern = getPatternDiagBottomToTop(board, x, y, value,visitedD2);
			if(pattern.allTokens.size() >= 5) {
				toReturn.clear();
				toReturn.add(pattern);
				return toReturn;
			}
			toReturn.add(pattern);
		}

		return toReturn;
	}
	
	private static Pattern getPatternTopToBottom(Board board,int x,int y,int value,boolean[][] visitedV) {
		Pattern pattern = new Pattern();
		int i = 0;
		for(i = y ; i >= 0 && board.getCell(i, x) == value ; i--) {
			visitedV[i][x] = true;
			pattern.allTokens.add(new Position(i,x));
		}
		if(i < 0 || board.getCell(i, x) != 0) {
			pattern.closedLeft = true;
		}
		for(i = y+1; i < Board.BOARD_SIZE && board.getCell(i, x) == value; i++) {
			visitedV[i][x] = true;
			pattern.allTokens.add(new Position(i,x));
		}
		if(i == Board.BOARD_SIZE || board.getCell(i, x) != 0) {
			pattern.closedRight = true;
		}
		
		return pattern;
	}
	
	private static Pattern getPatternLeftToRight(Board board,int x,int y,int value,boolean[][] visitedH) {
		Pattern pattern = new Pattern();
		int i = 0;
		for(i = x ; i >= 0 && board.getCell(y, i) == value ; i--) {
			visitedH[y][i] = true;
			pattern.allTokens.add(new Position(y,i));
		}
		if(i < 0 || board.getCell(y, i) != 0) {
			pattern.closedLeft = true;
		}
		
		for(i = x+1; i < Board.BOARD_SIZE && board.getCell(y, i) == value; i++) {
			visitedH[y][i] = true;
			pattern.allTokens.add(new Position(y,i));
		}
		if(i == Board.BOARD_SIZE || board.getCell(y, i) != 0) {
			pattern.closedRight = true;
		}
		return pattern;
	}
	
	
	private static Pattern getPatternDiagTopToBottom(Board board,int x, int y,int value,boolean[][] visitedD1) {
		Pattern pattern = new Pattern();
		int i = x;
		int j = y;
		for(; i >= 0 && j >= 0 && board.getCell(j, i) == value ; i--,j--) {
			pattern.allTokens.add(new Position(j,i));
			visitedD1[j][i] = true;
		}
		if(i < 0 || j < 0 || board.getCell(j, i) != 0) {
			pattern.closedLeft = true;
		}
		i = x+1;
		j = y+1;
		for(; i < Board.BOARD_SIZE && j < Board.BOARD_SIZE && board.getCell(j, i) == value; i++,j++) {
			pattern.allTokens.add(new Position(j,i));
			visitedD1[j][i] = true;
		}
		if(i == Board.BOARD_SIZE || j == Board.BOARD_SIZE || board.getCell(j, i) != 0) {
			pattern.closedRight = true;
		}
		return pattern;
	}
	
	private static Pattern getPatternDiagBottomToTop(Board board,int x, int y,int value,boolean[][] visitedD2) {
		Pattern pattern = new Pattern();
		int i = x;
		int j = y;
		for(; i >= 0 && j < Board.BOARD_SIZE && board.getCell(j, i) == value ; i--,j++) {
			pattern.allTokens.add(new Position(j,i));
			visitedD2[j][i] = true;
		}
		if(i < 0 || j == Board.BOARD_SIZE || board.getCell(j, i) != 0) {
			pattern.closedLeft = true;
		}
		i = x+1;
		j = y-1;
		for(; i < Board.BOARD_SIZE && j >= 0 && board.getCell(j, i) == value; i++,j--) {
			pattern.allTokens.add(new Position(j,i));
			visitedD2[j][i] = true;
		}
		if(i == Board.BOARD_SIZE || j < 0 || board.getCell(j, i) != 0) {
			pattern.closedRight = true;
		}
		return pattern;
	}
	
	
	

}
