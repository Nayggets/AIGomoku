package AIGomoku;

public interface EvaluateFunction {
	public static int MaxValue = 1000000;

	public abstract int evaluateBoard(Board board,int value);
}
