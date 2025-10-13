package AIGomoku;
import java.util.Objects;

public class Position {
	private int x;
	private int y;
	
	public Position(int y, int x) {
		super();
		this.x = x;
		this.y = y;
	}	
	
	public int getX() {
		return x;
	}

	public int getY() {
		return y;
	}

	public boolean isNear(Position pos) {
		
		int posy = pos.getY();
		int posx = pos.getX();
		if((Math.abs(posx - x) == 1 || Math.abs(posx - x) == 0) && (Math.abs(posy - x) == 1 || Math.abs(posy - y) == 0)) {
			return true;
		}
		
		return false;
	}

	@Override
	public int hashCode() {
		return Objects.hash(x, y);
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		Position other = (Position) obj;
		return x == other.x && y == other.y;
	}

	@Override
	public String toString() {
		return "Position [y=" + (y+1) + ", x=" + (x+1) + "]";
	}
	
	
	
	
	
}
