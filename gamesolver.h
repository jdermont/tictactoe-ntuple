#ifndef GAMESOLVER_H
#define GAMESOLVER_H

#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <vector>
#include <map>
#include <set>
#include "game.h"

#define UNSPECIFIED -420

using namespace std;

class GameSolver {
public:
    explicit GameSolver() {

    }

    void solveAllStates() {
        data.clear();
        tt.clear();
        getScore(1);
    }

    map<vector<int>,float> getData() {
        return data;
    }

private:
    Game game;
    map<vector<int>,float> data;
    map<int,float> tt;

    float getScore(int color) {
        int hash = game.getHash();
        if (tt.count(hash)) {
            return color * tt[hash];
        }

        float output = UNSPECIFIED;

        for (auto & move : game.getMoves()) {
            float score;
            game.makeMove(move);
            if (game.isOver()) {
                int winner = game.getWinner();
                if (winner == PLAYER_X) {
                    score = 1;// - 0.05 * game.rounds;
                } else if (winner == PLAYER_O) {
                    score = -1;// + 0.05 * game.rounds;
                } else {
                    score = 0;
                }
                score *= color;
            } else {
                score = -getScore(-color);
            }
            data[game.getTuples()] = color * score;
            tt[game.getHash()] = color * score;
            game.undoMove();
            if (output == UNSPECIFIED) {
                output = score;
            } else {
                output = max(output, score);
            }
        }

        data[game.getTuples()] = color * output;
        tt[game.getHash()] = color * output;

        return output;
    }

};

#endif // GAMESOLVER_H
