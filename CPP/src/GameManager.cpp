#include "GameManager.hpp"

void GameManager::LaunchGame()
{
    printMenu();
    unsigned int choice;
    std::cin >> choice;
    while(choice != PLAYER_VS_PLAYER && choice != PLAYER_VS_IA && choice != IA_VS_IA){
        std::cout << "Choose a valid Number" << std::endl;
        printMenu();
        std::cin >> choice;
    }

    switch(choice)
    {
        case PLAYER_VS_PLAYER:
        {
            this->playerVersusPlayer();
            break;
        }
        case PLAYER_VS_IA:
        {
            this->playerVersusIA();
            break;
        }
        case IA_VS_IA:
        {
            this->IAVersusIA();
            break;
        }
    }
}

void GameManager::printMenu()
{
    std::cout << "Select a game mod"   << "\n" 
              << "1. Two player"       << "\n" 
			  << "2. One player vs IA" << "\n"
			  << "3. Two IA"           << std::endl;
}

void GameManager::playerVersusPlayer()
{
    BoardGame* board = BoardGame::getInstance();
    bool gameFinish = false;
    bool movePlayed = false;
    idPlayer turn = idPlayer::p1;
    Position* pos = new Position(0,0);
    board->getBoard()->printBoard();
    //boucle de jeu
    while(!gameFinish) {
        std::cout << "Player " << turn << " turn" << std::endl;
        while(!movePlayed) {
            std::cout << "Enter position x where you want to place the token" << std::endl;
            pos->setX(Controller::getPos());
            std::cout << "Enter position y where you want to place the token" << std::endl;
            pos->setY(Controller::getPos());
            movePlayed = board->placeToken(turn,pos); 
            board->getBoard()->printBoard();
            std::cout << "x is : " << pos->getX() << " y is : " << pos->getY() << "and move is : " << movePlayed << std::endl;

            if(!movePlayed) {
                std::cout << "Move not valid" << std::endl;
            }
            else{
                if(board->isFinished(pos,turn)) {
                    gameFinish = true;
                }
            }
        }
        if(gameFinish) {
            std::cout<< "Player " << turn << " Win !" << std::endl;
        }
        if(turn == idPlayer::p2){
            turn = idPlayer::p1;
        }
        else{
            turn = idPlayer::p2;
        }
        movePlayed = false;
    }
}

void GameManager::playerVersusIA()
{
    //Launch One player vs IA mode
    BoardGame* board = BoardGame::getInstance();
    int value[] = {1,10,100,1000,10000,100000,1000000};
    int value2[] = {1,10,100,1000,10000,100000};
    BasicEvaluateFunction* func = new BasicEvaluateFunction();
    ElaborateEvaluateFunction* func2 = new ElaborateEvaluateFunction(value);
    JumpEvaluateFunction* func3 = new JumpEvaluateFunction(value2);
    AIGomoku* AI = new AIOpen(idPlayer::p2,5,func2);
    idPlayer playerTurn = idPlayer::p1;
    bool gameFinish = false;
    bool movePlayed = false;
    int posX;
    int posY;
    Position* pos = new Position(0,0);
    board->getBoard()->printBoard();
    while(!gameFinish) {
        if(playerTurn == idPlayer::p1) {
            std::cout << "Player turn" << std::endl;
            while(!movePlayed) {
                std::cout << "Enter position x where you want to place the token" << std::endl;
                pos->setX(Controller::getPos());
                std::cout << "Enter position y where you want to place the token" << std::endl;
                pos->setY(Controller::getPos());
                movePlayed = board->placeToken(playerTurn,pos); 
                board->getBoard()->printBoard();
                std::cout << "x is : " << pos->getX() << " y is : " << pos->getY() << "and move is : " << movePlayed << std::endl;

                if(!movePlayed) {
                    std::cout << "Move not valid" << std::endl;
                }
                else{
                    if(board->isFinished(pos,playerTurn)) {
                        gameFinish = true;
                    }
                }
            }
        }
        else {
            std::cout << "IA turn" << std::endl;
            gameFinish = AI->playMove(board);

        }
        board->getBoard()->printBoard();

        if(playerTurn == idPlayer::p2){
            playerTurn = idPlayer::p1;
        }
        else{
            playerTurn = idPlayer::p2;
        }
        movePlayed = false;
        
    }
    if(playerTurn) {
        std::cout << "Player Win" << std::endl;
    }
    else{
        std::cout << "AI Win" << std::endl;
    }
}

void GameManager::IAVersusIA()
{

}