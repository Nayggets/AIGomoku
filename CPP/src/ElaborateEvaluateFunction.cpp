#include "ElaborateEvaluateFunction.hpp"




int ElaborateEvaluateFunction::evaluateBoard(Board* board, idPlayer value) {
    bool visitedH[Board::BOARD_SIZE][Board::BOARD_SIZE] = {};
    bool visitedV[Board::BOARD_SIZE][Board::BOARD_SIZE] = {};
    bool visitedD1[Board::BOARD_SIZE][Board::BOARD_SIZE] = {};
    bool visitedD2[Board::BOARD_SIZE][Board::BOARD_SIZE] = {};
    std::vector<Pattern*> allyPattern;
    std::vector<Pattern*> ennemyPattern;
    for(int y = 0 ; y < Board::BOARD_SIZE ; y++) {
        for(int x = 0 ; x < Board::BOARD_SIZE ; x++) {
            if(board->getCell(x, y) != 0) {
                if(board->getCell(x, y) == value) {
                    std::vector<Pattern*> pos = getAllPattern(board,x,y,value,visitedH,visitedV,visitedD1,visitedD2);
                    allyPattern.insert(allyPattern.end(),pos.begin(),pos.end());
                }
                else {
                    std::vector<Pattern*> pos = getAllPattern(board,x,y,(value == idPlayer::p1) ? idPlayer::p2 : idPlayer::p1,visitedH,visitedV,visitedD1,visitedD2);
                    ennemyPattern.insert(ennemyPattern.end(),pos.begin(),pos.end());
                }
            }
            
        }
    }
    int valueCurrentPlayer = getValue(allyPattern);
    int valueEnnemyPlayer = getValue(ennemyPattern);
    
    return valueCurrentPlayer - valueEnnemyPlayer ;
}

int ElaborateEvaluateFunction::getValue(std::vector<Pattern*>& pattern) {
    int value = 0;
    int valueTotal = 0;
    int size = 0;
    Pattern* toCount = nullptr;
    for(int i = 0 ; i < pattern.size(); i++) {
        toCount = pattern[i];
        if(toCount->allTokens.size() >= 5) {
            return this->value[7]; 
        }
        if(toCount->closedLeft && toCount->closedRight) {
            continue;
        }
        size = toCount->allTokens.size();
        switch(size) 
        {
        /*
            case 1:
            {
                value +=  this.value[0];
                break;
            }
        */
            case 2:
            {
                if(toCount->closedLeft || toCount->closedRight) {
                    value = this->value[1];
                }
                else {
                    value = this->value[2];
                }
                break;
            }
            case 3:
            {
                if(toCount->closedLeft || toCount->closedRight) {
                    value = this->value[3];
                }else {
                    value = this->value[4];
                }
                break;
            }
            case 4:
            {
                if(toCount->closedLeft || toCount->closedRight) {
                    value = this->value[5];
                }
                else {
                    return this->value[6];
                }
                break;
            }
            
        }
        valueTotal += value;
    }
    return valueTotal;
}


std::vector<Pattern*> ElaborateEvaluateFunction::getAllPattern(Board* board, int x, int y,idPlayer value,bool visitedH[Board::BOARD_SIZE][Board::BOARD_SIZE],bool visitedV[Board::BOARD_SIZE][Board::BOARD_SIZE],bool visitedD1[Board::BOARD_SIZE][Board::BOARD_SIZE],bool visitedD2[Board::BOARD_SIZE][Board::BOARD_SIZE]) {
    std::vector<Pattern*> toReturn;
    Pattern* pattern = nullptr;
    
    if(!visitedH[y][x]) {
        pattern = getPatternLeftToRight(board, x, y, value,visitedH);
        if(pattern->allTokens.size() >= 5) {
            toReturn.clear();
            toReturn.push_back(pattern);
            return toReturn;
        }
        toReturn.push_back(pattern);
    }
    if(!visitedV[y][x]) {
        pattern = getPatternTopToBottom(board,x,y,value,visitedV);
        if(pattern->allTokens.size() >= 5) {
            toReturn.clear();
            toReturn.push_back(pattern);
            return toReturn;
        }
        toReturn.push_back(pattern);
    }
    if(!visitedD1[y][x]) {
        pattern = getPatternDiagTopToBottom(board, x, y, value,visitedD1);
        if(pattern->allTokens.size() >= 5) {
            toReturn.clear();
            toReturn.push_back(pattern);
            return toReturn;
        }
        toReturn.push_back(pattern);
    }
    if(!visitedD2[y][x]) {
        pattern = getPatternDiagBottomToTop(board, x, y, value,visitedD2);
        if(pattern->allTokens.size() >= 5) {
            toReturn.clear();
            toReturn.push_back(pattern);
            return toReturn;
        }
        toReturn.push_back(pattern);
    }

    return toReturn;
}

Pattern* ElaborateEvaluateFunction::getPatternLeftToRight(Board* board,int x,int y,idPlayer value,bool visitedH[Board::BOARD_SIZE][Board::BOARD_SIZE]) {
    Pattern* pattern = new Pattern();
    int i = 0;
    for(i = x ; i >= 0 && board->getCell(i, y) == value ; i--) {
        visitedH[y][i] = true;
        pattern->allTokens.push_back(new Position(i,y));
    }
    if(i < 0 || board->getCell(i, y) != 0) {
        pattern->closedLeft = true;
    }
    
    for(i = x+1; i < Board::BOARD_SIZE && board->getCell(i, y) == value; i++) {
        visitedH[y][i] = true;
        pattern->allTokens.push_back(new Position(i,y));
    }
    if(i == Board::BOARD_SIZE || board->getCell(i, y) != 0) {
        pattern->closedRight = true;
    }
    return pattern;
}



Pattern* ElaborateEvaluateFunction::getPatternTopToBottom(Board* board,int x,int y,idPlayer value,bool visitedV[Board::BOARD_SIZE][Board::BOARD_SIZE]) {
        Pattern* pattern = new Pattern();
        int i = 0;
        for(i = y ; i >= 0 && board->getCell(x, i) == value ; i--) {
            visitedV[i][x] = true;
            pattern->allTokens.push_back(new Position(x,i));
        }
        if(i < 0 || board->getCell(x, i) != 0) {
            pattern->closedLeft = true;
        }
        for(i = y+1; i < Board::BOARD_SIZE && board->getCell(x, i) == value; i++) {
            visitedV[i][x] = true;
            pattern->allTokens.push_back(new Position(x,i));
        }
        if(i == Board::BOARD_SIZE || board->getCell(x, i) != 0) {
            pattern->closedRight = true;
        }
        
        return pattern;
    }


Pattern* ElaborateEvaluateFunction::getPatternDiagTopToBottom(Board* board,int x, int y,idPlayer value,bool visitedD1[Board::BOARD_SIZE][Board::BOARD_SIZE]) {
    Pattern* pattern = new Pattern();
    int i = x;
    int j = y;
    for(; i >= 0 && j >= 0 && board->getCell(i, j) == value ; i--,j--) {
        pattern->allTokens.push_back(new Position(i,j));
        visitedD1[j][i] = true;
    }
    if(i < 0 || j < 0 || board->getCell(i, j) != 0) {
        pattern->closedLeft = true;
    }
    i = x+1;
    j = y+1;
    for(; i < Board::BOARD_SIZE && j < Board::BOARD_SIZE && board->getCell(i, j) == value; i++,j++) {
        pattern->allTokens.push_back(new Position(i,j));
        visitedD1[j][i] = true;
    }
    if(i == Board::BOARD_SIZE || j == Board::BOARD_SIZE || board->getCell(i, j) != 0) {
        pattern->closedRight = true;
    }
    return pattern;
}

Pattern* ElaborateEvaluateFunction::getPatternDiagBottomToTop(Board* board,int x, int y,idPlayer value,bool visitedD2[Board::BOARD_SIZE][Board::BOARD_SIZE]) {
    Pattern* pattern = new Pattern();
    int i = x;
    int j = y;
    for(; i >= 0 && j < Board::BOARD_SIZE && board->getCell(i, j) == value ; i--,j++) {
        pattern->allTokens.push_back(new Position(i,j));
        visitedD2[j][i] = true;
    }
    if(i < 0 || j == Board::BOARD_SIZE || board->getCell(i, j) != 0) {
        pattern->closedLeft = true;
    }
    i = x+1;
    j = y-1;
    for(; i < Board::BOARD_SIZE && j >= 0 && board->getCell(i, j) == value; i++,j--) {
        pattern->allTokens.push_back(new Position(i,j));
        visitedD2[j][i] = true;
    }
    if(i == Board::BOARD_SIZE || j < 0 || board->getCell(i, j) != 0) {
        pattern->closedRight = true;
    }
    return pattern;
}

