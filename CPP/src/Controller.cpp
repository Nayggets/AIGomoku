#include "Controller.hpp"

unsigned int Controller::getPos()
{
    int nb = -1;
    std::cin >> nb;
    nb--;
    while(nb < 0 || nb > (int)Board::BOARD_SIZE-1) {
        std::cout << "Select a number" << std::endl;
        std::cin.clear();
        std::cin >> nb;
        nb--;
    }
    
    return nb;
}
