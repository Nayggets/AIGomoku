package AIGomoku;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;



public class AIBoard implements Cloneable{
	private static EvaluateFunction evaluate;
	private static final ConcurrentHashMap<Long, Integer> TranspositionTable = new ConcurrentHashMap<>();
	private static int radius = 2;
	public AIBoard() {
	}
	

	@Override
	protected Object clone() throws CloneNotSupportedException {
		AIBoard futureBoard = (AIBoard) super.clone();
		return futureBoard;
	}

	



	
	public Board getBoardToPlay(int depth, EvaluateFunction func,int nbPlayer) {
		AIBoard.evaluate = func;
		TranspositionTable.clear();
		try {
			return this.minimaxAlgorithm(BoardGame.getInstance().getBoard(),depth,nbPlayer);
		} catch (CloneNotSupportedException e) { // not suppose to
			e.printStackTrace();
		} catch (InterruptedException e) {
			e.printStackTrace();
		} catch (ExecutionException e) {
			e.printStackTrace();
		}
		return null;
		
	}
	
	static class Result {
        int value;
        Board board;
        Result(int value, Board board) {
            this.value = value;
            this.board = board;
        }
    }
	
	private static List<Position> generateMoves(Board board) {
	    List<Position> moves = new ArrayList<>();

	    for (int y = 0; y < Board.BOARD_SIZE; y++) {
	        for (int x = 0; x < Board.BOARD_SIZE; x++) {
	            
	            // empty
	            if (board.getCell(y, x) == 0) {

	                // If rock is near
	                if (isRelevantMove(x, y, board)) {
	                    moves.add(new Position(y, x));
	                }
	            }
	        }
	    }

	    return moves;
	}
	
	private static boolean inBounds(int x, int y) {
	    return (x >= 0 && x < Board.BOARD_SIZE &&
	           y >= 0 && y < Board.BOARD_SIZE);
	}
	
	public static boolean isRelevantMove(int x, int y, Board board) {
	    for (int dy = -radius; dy <= radius; dy++) {
	        for (int dx = -radius; dx <= radius; dx++) {
	            int nx = x + dx;
	            int ny = y + dy;

	            if (inBounds(nx,ny) && board.getCell(ny, nx) != 0) {
	                return true;
	            }
	        }
	    }
	    return false;
	}
	
	
	public Board minimaxAlgorithm(Board board,int depth,int nbPlayer) throws CloneNotSupportedException, InterruptedException, ExecutionException {
		if(depth == 0) {
			throw new RuntimeException("Error in depth selection");
		}
		int nproc = Runtime.getRuntime().availableProcessors();
	    ExecutorService executor = Executors.newFixedThreadPool(nproc);
	    List<Future<Result>> futures = new ArrayList<>();
		int lastValue = Integer.MIN_VALUE;
		Board boardToModify = (Board) board.clone();
		Board toReturn = null;
		List<Position> pos = AIBoard.generateMoves(board);
		if(pos.isEmpty()) {
			System.out.println("Is empty");
			boardToModify.setCell(7, 7, nbPlayer);
			return boardToModify;
		}
		long baseHash = Zobrist.compute(board);
		int x = 0;
		int y = 0;
		for(Position p : pos) {
			y = p.getY();
			x = p.getX();
			if(boardToModify.getCell(y, x) == 0) {
				Board boardCopy = (Board) boardToModify.clone();
				boardCopy.setCell(y,x,nbPlayer);
				final Board boardTask = boardCopy;
				long newHash = Zobrist.update(baseHash, y, x, 0, nbPlayer);
				futures.add(executor.submit(() -> {
                	int score = -AIBoard.minimaxAlgorithmAux(boardTask, depth - 1,Integer.MIN_VALUE, Integer.MAX_VALUE, (nbPlayer == 1) ? 2 : 1,newHash);
                	return new Result(score,boardTask);
				}));
			}
		}
        executor.shutdown();
        executor.awaitTermination(1, TimeUnit.HOURS);

	
		if(futures.isEmpty()) {
			System.out.println("PROBLEMS WITH THREEADING");
		}
		for (Future<Result> f : futures) {
            Result score = f.get();   // Future.get() attend le résultat
            if(score.value > lastValue || (score.value == lastValue && evaluate.evaluateBoard(score.board, nbPlayer) > evaluate.evaluateBoard(toReturn, nbPlayer))) {
            	toReturn = score.board;
            	lastValue = score.value;
            }
           
        }
		if(toReturn == null) {
			System.out.println("Huge mistakes");
		}
		System.out.println("Last value is : " + lastValue);
		
		return toReturn;

	}
	
	private static int minimaxAlgorithmAux(Board board,int depth,int alpha,int beta,int nbPlayer,long hash) {
		long key = Zobrist.withPlayer(hash, nbPlayer);
		if(depth == 0 || board.lastMoveFinished()) {
			Integer cachedScore = TranspositionTable.get(key);
			if(cachedScore != null){
				return cachedScore.intValue();
			}
			int value = evaluate.evaluateBoard(board,nbPlayer);
			TranspositionTable.put(key, value);
			return value;
		}
		List<Position> pos = AIBoard.generateMoves(board);
		int value = Integer.MIN_VALUE;
		int y = 0;
		int x = 0;
		long newHash = 0;
		for(Position p : pos) {
			y = p.getY();
			x = p.getX();
			if(board.getCell(y, x) == 0) {
				board.setCell(y,x,nbPlayer);
				newHash = Zobrist.update(hash, y, x, 0, nbPlayer) ;
				value = Math.max(value,-AIBoard.minimaxAlgorithmAux(board,depth-1,-beta,-alpha,(nbPlayer == 1) ? 2 : 1,newHash));
				
				board.setCell(y, x, 0);
				alpha = Math.max(alpha, value);
				if(alpha >= beta) {
					break;
				}
			}
		}
		Integer cachedScore = TranspositionTable.get(key);
		if(cachedScore != null){
			return cachedScore.intValue();
		}
		TranspositionTable.put(key, value);
		return value;
	}
	


	
	
	

	
	
	
	
	
	
	
	
	
	
	
	
}
