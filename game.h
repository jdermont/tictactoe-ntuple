#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <vector>

#define NONE -1
#define PLAYER_X 0
#define PLAYER_O 1

using namespace std;

const vector<int> winss = {
    0b0000000000000111,
    0b0000000000111000,
    0b0000000111000000,
    0b0000000100100100,
    0b0000000010010010,
    0b0000000001001001,
    0b0000000100010001,
    0b0000000001010100
};

static vector<int> generateWins() {
    vector<int> wins(512,0);
    for (int i=0;i<512;i++) {
        bool broken = false;
        for (auto & win : winss) {
            if ((win & i) == win) {
                wins[i] = 1;
                broken = true;
                break;
            }
        }
        if (broken) continue;
        wins[i] = 0;
    }

    return wins;
}

static const vector<int> winConditions = generateWins();

class Board {
public:
    int gameState;
    int movesCount = 0;

    explicit Board() {
        gameState = 0;
        movesCount = 0;
    }

    void makeMove(int player, int move) {
        if (player == PLAYER_X) {
            gameState ^= 1 << move;
        } else {
            gameState ^= 1 << (move+9);
        }
        movesCount++;
    }

    void undoMove(int player, int move) {
        if (player == PLAYER_X) {
            gameState ^= 1 << move;
        } else {
            gameState ^= 1 << (move+9);
        }
        movesCount--;
    }

    bool isOver() {
        return movesCount == 9 || (getWinner() != NONE);
    }

    vector<int> getMoves() {
        vector<int> moves; moves.reserve(9);
        int empty = ~(gameState|(gameState>>9));
        for (int i=0; i < 9; i++) {
            int p = 1 << i;
            if (p&empty) {
                moves.push_back(i);
            }
        }

        return moves;
    }

    int getWinner() {
        int xState = gameState & 0x01FF;
        if (winConditions[xState]) {
            return PLAYER_X;
        }
        int oState = (gameState>>9) & 0x01FF;
        if (winConditions[oState]) {
            return PLAYER_O;
        }
        return NONE;
    }
};

class Game {
public:
    int currentPlayer;
    int rounds;

    explicit Game() {
        currentPlayer = PLAYER_X;
        rounds = 0;
    }

    void makeMove(int move) {
        board.makeMove(currentPlayer, move);
        history.push_back(move);
        currentPlayer ^= 1;
        rounds++;
    }

    void undoMove() {
        int move = history.back();
        history.pop_back();
        currentPlayer ^= 1;
        board.undoMove(currentPlayer, move);
        rounds--;
    }

    bool isOver() {
        return board.isOver();
    }

    int getWinner() {
        return board.getWinner();
    }

    vector<int> getMoves() {
        return board.getMoves();
    }

    int getHash() {
        return board.gameState;
    }

    void print() {
        for (int i=0; i < 9; i++) {
            int p = 1 << i;
            if (p&board.gameState) {
                cout << 'X';
            } else if (p&(board.gameState>>9)) {
                cout << 'O';
            } else {
                cout << '.';
            }
            if ((i+1)%3 == 0) {
                cout << endl;
            }
        }
    }

    vector<int> getTuples() {
        vector<int> tuples(8);
        int n = 0;

        for (int i=0; i < 3; i++) {
            int tuple = 0;
            int power = 1;
            for (int j=0; j < 3; j++) {
                int p = 1 << (3*i + j);
                if (p&board.gameState) {
                    tuple += 1 * power;
                } else if (p & (board.gameState>>9)) {
                    tuple += 2 * power;
                }
                power *= 3;
            }
            tuples[n++] = tuple + (currentPlayer == PLAYER_X ? 0 : 27);
        }

        for (int i=0; i < 3; i++) {
            int tuple = 0;
            int power = 1;
            for (int j=0; j < 3; j++) {
                int p = 1 << (i + 3*j);
                if (p&board.gameState) {
                    tuple += 1 * power;
                } else if (p & (board.gameState>>9)) {
                    tuple += 2 * power;
                }
                power *= 3;
            }
            tuples[n++] = tuple + (currentPlayer == PLAYER_X ? 0 : 27);
        }

        {
            int tuple = 0;
            int power = 1;
            for (int i=0; i < 3; i++) {
                int p = 1 << (0 + 4*i);
                if (p&board.gameState) {
                    tuple += 1 * power;
                } else if (p & (board.gameState>>9)) {
                    tuple += 2 * power;
                }
                power *= 3;
            }
            tuples[n++] = tuple + (currentPlayer == PLAYER_X ? 0 : 27);
        }

        {
            int tuple = 0;
            int power = 1;
            for (int i=0; i < 3; i++) {
                int p = 1 << (2 + 2*i);
                if (p&board.gameState) {
                    tuple += 1 * power;
                } else if (p & (board.gameState>>9)) {
                    tuple += 2 * power;
                }
                power *= 3;
            }
            tuples[n++] = tuple + (currentPlayer == PLAYER_X ? 0 : 27);
        }

        return tuples;
    }

    void x(int y) {
        board.gameState = y;
    }

private:
    Board board;
    vector<int> history;
};


#endif // GAME_H
