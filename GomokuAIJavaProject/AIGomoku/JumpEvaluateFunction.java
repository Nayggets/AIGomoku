package AIGomoku;

import java.util.ArrayList;

public class JumpEvaluateFunction implements EvaluateFunction{
	
	private int[] value;

	
	public JumpEvaluateFunction(int[] value) {
		this.value = value;
	}


	@Override
	public int evaluateBoard(Board board, int value) {
		Long[] allLinesAlly = new Long[72];
		Long[] allLinesEnnemy = new Long[72];
        Integer nbOpenThree = 0;
        Integer nbFour = 0;
		long[] ally = board.getBoardByPlayer(value);
		long[] ennemy = board.getBoardByPlayer((value == 1) ? 2 : 1);
		int indexAlly = 0;
		int indexEnnemy = 0;
		for(int i = 0; i < Board.BOARD_SIZE ; i++){
			
			//add line
			allLinesAlly[indexAlly++] = board.getLine(ally, 0,i , Board.BOARD_SIZE, Board.Direction.HORIZONTAL);
			allLinesEnnemy[indexEnnemy++] = board.getLine(ennemy, 0,i , Board.BOARD_SIZE, Board.Direction.HORIZONTAL);

			//add vertical line
			allLinesAlly[indexAlly++] = board.getLine(ally, i,0 , Board.BOARD_SIZE, Board.Direction.VERTICAL);
			allLinesEnnemy[indexEnnemy++] = board.getLine(ennemy, i,0 , Board.BOARD_SIZE, Board.Direction.VERTICAL);

			if(Board.BOARD_SIZE-i >= 5) {
				//add diagonal bot
				if(Board.BOARD_SIZE-i == 15) {
					//down
					allLinesAlly[indexAlly++] = board.getLine(ally, 0,0, Board.BOARD_SIZE, Board.Direction.DIAGONAL_DOWN);
					allLinesEnnemy[indexEnnemy++] = board.getLine(ennemy, 0,0, Board.BOARD_SIZE, Board.Direction.DIAGONAL_DOWN);

					//up
					allLinesAlly[indexAlly++] = board.getLine(ally,0, 0, Board.BOARD_SIZE, Board.Direction.DIAGONAL_UP);
					allLinesEnnemy[indexEnnemy++] = board.getLine(ennemy,0, 0, Board.BOARD_SIZE, Board.Direction.DIAGONAL_UP);


				}
				else {
					//down
					allLinesAlly[indexAlly++] = board.getLine(ally, i,0, Board.BOARD_SIZE-i, Board.Direction.DIAGONAL_DOWN);
					allLinesAlly[indexAlly++] = board.getLine(ally, 0,i, Board.BOARD_SIZE-i, Board.Direction.DIAGONAL_DOWN);
					allLinesEnnemy[indexEnnemy++] = board.getLine(ennemy, i,0, Board.BOARD_SIZE-i, Board.Direction.DIAGONAL_DOWN);
					allLinesEnnemy[indexEnnemy++] = board.getLine(ennemy, 0,i, Board.BOARD_SIZE-i, Board.Direction.DIAGONAL_DOWN);
					//up
					allLinesAlly[indexAlly++] = board.getLine(ally, i,Board.BOARD_SIZE, Board.BOARD_SIZE-i,Board.Direction.DIAGONAL_UP);
					allLinesAlly[indexAlly++] = board.getLine(ally, 0,Board.BOARD_SIZE-i, Board.BOARD_SIZE-i,Board.Direction.DIAGONAL_UP);
					allLinesEnnemy[indexEnnemy++] = board.getLine(ennemy, i,Board.BOARD_SIZE, Board.BOARD_SIZE-i,Board.Direction.DIAGONAL_UP);
					allLinesEnnemy[indexEnnemy++] = board.getLine(ennemy, 0,Board.BOARD_SIZE-i, Board.BOARD_SIZE-i,Board.Direction.DIAGONAL_UP);

				}
			}

		}
		int valueAlly = 0;
		int valueEnnemy = 0;
		for(int i = 0 ; i < allLinesAlly.length ;i++) {
			valueAlly += this.evaluateLine(allLinesAlly[i],allLinesEnnemy[i],nbOpenThree,nbFour);
			if(valueAlly >100000) {
				return 100000;
			}
			valueEnnemy += this.evaluateLine(allLinesEnnemy[i],allLinesAlly[i],nbOpenThree,nbFour);
			if(valueEnnemy > 100000) {
				return -100000;
			}
		}
		return valueAlly - valueEnnemy;
	}
	
	
	
    public int evaluateLine(long playerBits, long opponentBits,Integer nbOpenThree,Integer nbFour) {
        if(playerBits == 0){
        	return 0;
        }
    	int score = 0;

        int i = 0;
        
        int start;
        int end;
        int length;
        boolean leftBlocked;
        boolean rightBlocked;
        boolean openLeft;
        boolean openRight;
        boolean openBoth;
        boolean openOne;
        while (i < 15) {
            // skip zeros
            while (i < 15 && ((playerBits >> i) & 1L) == 0) i++;
            if (i >= 15) break;

            // begin of sequence of 1
            start = i;
            while (i < 15 && ((playerBits >> i) & 1L) == 1L) i++;
            end = i - 1;
            length = end - start + 1;

            // is open or semi open or not
            leftBlocked = (start == 0) || (((opponentBits >> (start - 1)) & 1L) != 0);
            rightBlocked = (end == 14) || (((opponentBits >> (end + 1)) & 1L) != 0);

            openLeft = !leftBlocked && (start > 0);
            openRight = !rightBlocked && (end < 14);

            openBoth = openLeft && openRight;
            openOne = (openLeft ^ openRight);

            // score case
            if (length >= 5) score += value[5]; // WIN!!!!!
            else if (length == 4 && openBoth) {
            	score += value[5];
            	nbFour++;
            }
            else if (length == 4 && openOne) {
            	score += value[4];
            	nbFour++;
            }
            else if (length == 3 && openBoth) {
            	score += value[3];
            	nbOpenThree++;
            }
            else if (length == 3 && openOne) score += value[2];
            else if (length == 2 && openBoth) score += value[1];
            else if (length == 2 && openOne) score += value[0];
        }
        
        // Three Threat victory
        if(nbOpenThree >= 2 || nbFour >= 2) {
        	return value[5];
        } // Four thread victory
        if(nbOpenThree == 1 && nbFour == 1) {
        	return value[5];
        }
        return score;
    }



}
