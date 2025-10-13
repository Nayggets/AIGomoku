package AIGomoku;
import java.util.Scanner;

public class Controller {
	
	
	private static Controller instance;
	private Scanner scan;
	
	public static Controller getInstance() {
		if(instance == null) {
			instance = new Controller();
		}
		return instance;
	}
	
	
	private Controller() {
		scan = new Scanner(System.in);
	}

	public int getPos() {
		
		int nb = scan.nextInt()-1;
		while(nb < 0 || nb > Board.BOARD_SIZE-1) {
			System.out.println("Select a number");
			nb = scan.nextInt()-1;
		}
		
		return nb;
	}
}
