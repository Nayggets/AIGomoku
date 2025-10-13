package AIGomoku;

import java.util.Scanner;

public class GameManager {
	
	
	public static void launchGame() {
		try (Scanner scan = new Scanner(System.in)) {
			System.out.println("Select a game mod");
			System.out.println("1. Two player");
			System.out.println("2. One player vs IA");
			System.out.println("3. Two IA");
			int code = scan.nextInt(); 
			while(code != 1 && code != 2 && code != 3) {
				System.out.println("Select a valid game mod");
				System.out.println("1. Two player");
				System.out.println("2. One player vs IA");
				System.out.println("3. Two IA");
				code = scan.nextInt();
			}
			if(code == 1) {
				//launch2playerMode
				BoardGame board = BoardGame.getInstance();
				Controller controller = Controller.getInstance();
				boolean gameFinish = false;
				boolean movePlayed = false;
				int turn = 1;
				int posX,posY;

				//boucle de jeu
				while(!gameFinish) {
					System.out.println("Player " + turn + " turn");
					while(!movePlayed) {
						System.out.println("Enter position x where you want to place the token");
						posX = controller.getPos();
						System.out.println("Enter position y where you want to place the token");
						posY = controller.getPos();
						movePlayed = board.placeToken(turn, posX, posY); 
						if(board.isFinished(new Position(posY,posX),turn)) {
							gameFinish = true;
						}
						board.getBoard().printBoard();
						if(!movePlayed) {
							System.out.println("Move not valid");
						}
					}
					if(gameFinish) {
						System.out.println("Player " + turn + " Win !");
					}
					
					movePlayed = false;
				}
			}
			else if(code == 2) {
				//Launch One player vs IA mode
				BoardGame board = BoardGame.getInstance();
				Controller controller = Controller.getInstance();
				int value[] = {1,10,100,1000,10000,100000};
				AIGomoku AI = new AIOpen((char) 2,4,new ElaborateEvaluateFunction(value));
				boolean playerTurn = false;
				boolean gameFinish = false;
				boolean movePlayed = false;
				int posX;
				int posY;
				while(!gameFinish) {
					if(playerTurn) {
						System.out.println("Player turn");
						while(!movePlayed) {
							System.out.println("Enter position x where you want to place the token");
							posX = controller.getPos();
							System.out.println("Enter position y where you want to place the token");
							posY = controller.getPos();
							movePlayed = board.placeToken(1, posX, posY); 
							if(board.isFinished(new Position(posY,posX),1)) {
								gameFinish = true;
							}
							if(!movePlayed) {
								System.out.println("Move not valid");
							}
							movePlayed = true;
						}
					}
					else {
						System.out.println("IA turn");
						gameFinish = AI.playMove(board);

					}
					board.getBoard().printBoard();

					playerTurn = !playerTurn;
					
					movePlayed = false;
					
				}
				if(playerTurn) {
					System.out.println("Player Win");
				}
				else{
					System.out.println("AI Win");
				}
				playerTurn = !playerTurn;
			}
			else {
				// Launch Two IA mode
				BoardGame board = BoardGame.getInstance();
				int value[] = {1,2,5,10,20,5000,100000};
				AIGomoku AI = new AIOpen((char)1,4,new JumpEvaluateFunction(value));
				AIGomoku AItwo = new RandomAIGomoku(2);
				int AITurn = 2;
				System.out.println(AITurn);
				
				System.out.println("AITurn is : " + AITurn);
				boolean gameFinish = false;
				while(!gameFinish) {
					if(AITurn == 1) {
						gameFinish = AItwo.playMove(board);
						AITurn++;
					}
					else {
						gameFinish = AI.playMove(board);
						AITurn--;
					}
					board.getBoard().printBoard();
				}
				if(AITurn == 2) {
					System.out.println("AI two win");
				}
				else {
					System.out.println("AI one win");
				}
			}
		}
		System.out.println("Number of move played is : " + BoardGame.numberOfMovesPlayed);
       }
     
	
}
