package AIGomoku;

import javafx.application.Application;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import javafx.stage.Stage;

// partie graphique partiellement generer par IA (je suis très mauvais en design et proportion donc je laisse l'IA ou les sites tutos le faire pour moi)
public class GomokuGUI extends Application {

    private static final int CELL_SIZE = 30;

    private Button[][] buttons;
    private BoardGame game;
    private AIOpen ai;
    private Label status;
    public static boolean turn = false;

    @Override
    public void start(Stage primaryStage) {
        game = BoardGame.getInstance();
        status = new Label("Votre tour !");
        int value[] = {10,40,100,800,1000,100000};
        ai = new AIOpen((char) 2, 4, new JumpEvaluateFunction(value));

        int size = Board.BOARD_SIZE;
        buttons = new Button[size][size];

        GridPane grid = new GridPane();
        grid.setAlignment(Pos.CENTER);
        grid.setHgap(1);
        grid.setVgap(1);

        for (int y = 0; y < size; y++) {
            for (int x = 0; x < size; x++) {
                Button cell = new Button(" ");
                cell.setPrefSize(CELL_SIZE, CELL_SIZE);
                final int fx = x;
                final int fy = y;
                cell.setOnAction(e -> handlePlayerMove(fx, fy));
                buttons[y][x] = cell;
                grid.add(cell, x, y);
            }
        }

        Button reset = new Button("Nouvelle partie");
        reset.setOnAction(e -> resetGame());

        HBox top = new HBox(10, status, reset);
        top.setAlignment(Pos.CENTER);

        BorderPane root = new BorderPane();
        root.setTop(top);
        root.setCenter(grid);

        Scene scene = new Scene(root);
        primaryStage.setScene(scene);
        primaryStage.setTitle("Gomoku - Joueur vs IA");
        primaryStage.show();
    }

    private void handlePlayerMove(int x, int y) {
        if (!game.placeToken(1, x, y)) return; // si case déjà prise
        buttons[y][x].setText("X");
        buttons[y][x].setDisable(true);

        if (game.isFinished(new Position(y, x), 1)) {
            status.setText("Vous avez gagné !");
            disableBoard();
            return;
        }

        status.setText("Tour de l'IA...");
        playAIMove();
    }

    private void playAIMove() {
        boolean iaWin = ai.playMove(game);

        // rafraîchir tout le plateau graphique
        Board board = game.getBoard();
        for (int y = 0; y < Board.BOARD_SIZE; y++) {
            for (int x = 0; x < Board.BOARD_SIZE; x++) {
                int val = board.getCell(y, x);
                if (val == 1) buttons[y][x].setText("X");
                else if (val == 2) buttons[y][x].setText("O");
                if (val != 0) buttons[y][x].setDisable(true);
            }
        }

        if (iaWin) {
            status.setText("L’IA gagne !");
            disableBoard();
        } else {
            status.setText("Votre tour !");
        }
    }

    private void disableBoard() {
        for (Button[] row : buttons)
            for (Button b : row)
                b.setDisable(true);
    }

    
    private void resetGame() {
    	GomokuGUI.turn = !GomokuGUI.turn;
        game = BoardGame.getInstance(); 
        status.setText("Nouvelle partie");

        Board board = game.getBoard();
        for (int y = 0; y < Board.BOARD_SIZE; y++) {
            for (int x = 0; x < Board.BOARD_SIZE; x++) {
                board.setCell(y, x, 0);
                buttons[y][x].setText(" ");
                buttons[y][x].setDisable(false);
            }
        }
        BoardGame.numberOfMovesPlayed = 0;
        if(GomokuGUI.turn) {
            playAIMove();
        }
    }

    public static void main(String[] args) {
        launch(args);
    }
}