#pragma once
#include <iostream>
#include "BoardGame.hpp"
#include "Controller.hpp"
#include "Position.hpp"
#include "RandomAIGomoku.hpp"
#include "AIOpen.hpp"
#include "BasicEvaluateFunction.hpp"
#include "ElaborateEvaluateFunction.hpp"
#include "JumpEvaluateFunction.hpp"

enum IdMode{
    PLAYER_VS_PLAYER = 1,
    PLAYER_VS_IA = 2,
    IA_VS_IA = 3
};

class GameManager{
    public: 
        void LaunchGame();

    private:
        void printMenu();
        void playerVersusPlayer();
        void playerVersusIA();
        void IAVersusIA();
};