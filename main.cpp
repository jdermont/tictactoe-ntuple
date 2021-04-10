#include <iostream>
#include <random>
#include <algorithm>

#include "game.h"
#include "gamesolver.h"
#include "cpu.h"
#include "ntuple.h"

using namespace std;

vector<pair<vector<int>,float>> trainingData;
NTupleNetwork network;

void generateTrainingData() {
    cout << "generating data from games..." << endl;
    GameSolver gameSolver;
    gameSolver.solveAllStates();
    auto solverData = gameSolver.getData();
    trainingData = vector<pair<vector<int>,float>>(solverData.begin(),solverData.end());
    cout << "there are " << trainingData.size() << " positions" << endl;
    gameSolver.solveAllStates();
}

void trainNetwork() {
    cout << "training network..." << endl;
    random_device rd;
    mt19937 mt = mt19937(rd());

    for (int epoch = 0; epoch < 100; epoch++) {
        shuffle(trainingData.begin(), trainingData.end(), mt);
        for (auto & trainRow : trainingData) {
            network.learn(trainRow.first, trainRow.second);
        }

        if ((epoch+1) % 5 == 0) {
            float error = 0;
            for (auto & trainRow : trainingData) {
                float score = network.predict(trainRow.first);
                error += (trainRow.second - score) * (trainRow.second - score);
            }
            cout << "epoch " << epoch+1 << ". error: " << error/trainingData.size() << endl;
        }
    }
}

void perfectCpuVsRandom() {
    int scores[3] = {};
    Cpu cpu1; cpu1.setPlayer(PLAYER_X);

    cout << "perfect cpu vs random: " << endl;
    cout << "CPU1 CPU2 DRAW: " << endl;
    for (int i = 0; i < 1000; i++) {
        cpu1.setPlayer(cpu1.getPlayer()^1);
        Game game;
        cpu1.setGame(&game);

        while (!game.isOver()) {
            if (game.currentPlayer == cpu1.getPlayer()) {
                auto move = cpu1.getBestMove(9);
                game.makeMove(move.move);
            } else {
                auto moves = game.getMoves();
                game.makeMove(moves[rand()%moves.size()]);
            }
        }

        int winner = game.getWinner();
        if (winner == cpu1.getPlayer()) {
            scores[0]++;
        } else if (winner != NONE) {
            scores[1]++;
        } else {
            scores[2]++;
        }

        if ((i+1) % 100 == 0) {
            cout << scores[0] << " " << scores[1] << " " << scores[2] << endl;
        }
    }
}

void networkCpuVsRandom() {
    int scores[3] = {};
    Cpu cpu1; cpu1.setPlayer(PLAYER_X);
    cpu1.setNetwork(&network);

    cout << "network cpu vs random: " << endl;
    cout << "CPU1 CPU2 DRAW: " << endl;
    for (int i = 0; i < 1000; i++) {
        cpu1.setPlayer(cpu1.getPlayer()^1);
        Game game;
        cpu1.setGame(&game);

        while (!game.isOver()) {
            if (game.currentPlayer == cpu1.getPlayer()) {
                auto move = cpu1.getBestMove(1);
                game.makeMove(move.move);
            } else {
                auto moves = game.getMoves();
                game.makeMove(moves[rand()%moves.size()]);
            }
        }

        int winner = game.getWinner();
        if (winner == cpu1.getPlayer()) {
            scores[0]++;
        } else if (winner != NONE) {
            scores[1]++;
        } else {
            scores[2]++;
        }

        if ((i+1) % 100 == 0) {
            cout << scores[0] << " " << scores[1] << " " << scores[2] << endl;
        }
    }
}

void networkCpuVsCpuPly3() {
    int scores[3] = {};
    Cpu cpu1; cpu1.setPlayer(PLAYER_X);
    cpu1.setNetwork(&network);
    Cpu cpu2; cpu2.setPlayer(PLAYER_O);

    cout << "network cpu vs cpu ply 3: " << endl;
    cout << "CPU1 CPU2 DRAW: " << endl;
    for (int i = 0; i < 1000; i++) {
        cpu1.setPlayer(cpu1.getPlayer()^1);
        cpu2.setPlayer(cpu2.getPlayer()^1);
        Game game;
        cpu1.setGame(&game);
        cpu2.setGame(&game);

        while (!game.isOver()) {
            if (game.currentPlayer == cpu1.getPlayer()) {
                auto move = cpu1.getBestMove(1);
                game.makeMove(move.move);
            } else {
                auto move = cpu2.getBestMove(3);
                game.makeMove(move.move);
            }
        }

        int winner = game.getWinner();
        if (winner == cpu1.getPlayer()) {
            scores[0]++;
        } else if (winner == cpu2.getPlayer()) {
            scores[1]++;
        } else {
            scores[2]++;
        }

        if ((i+1) % 100 == 0) {
            cout << scores[0] << " " << scores[1] << " " << scores[2] << endl;
        }
    }
}

void networkCpuVsPerfectCpu() {
    int scores[3] = {};
    Cpu cpu1; cpu1.setPlayer(PLAYER_X);
    cpu1.setNetwork(&network);
    Cpu cpu2; cpu2.setPlayer(PLAYER_O);

    cout << "network cpu vs perfect cpu: " << endl;
    cout << "CPU1 CPU2 DRAW: " << endl;
    for (int i = 0; i < 1000; i++) {
        cpu1.setPlayer(cpu1.getPlayer()^1);
        cpu2.setPlayer(cpu2.getPlayer()^1);
        Game game;
        cpu1.setGame(&game);
        cpu2.setGame(&game);

        while (!game.isOver()) {
            if (game.currentPlayer == cpu1.getPlayer()) {
                auto move = cpu1.getBestMove(1);
                game.makeMove(move.move);
            } else {
                auto move = cpu2.getBestMove(9);
                game.makeMove(move.move);
            }
        }

        int winner = game.getWinner();
        if (winner == cpu1.getPlayer()) {
            scores[0]++;
        } else if (winner == cpu2.getPlayer()) {
            scores[1]++;
        } else {
            scores[2]++;
        }

        if ((i+1) % 100 == 0) {
            cout << scores[0] << " " << scores[1] << " " << scores[2] << endl;
        }
    }
}

int main() {
    srand(time(NULL) ^ (uint64_t)(&main));

    generateTrainingData();
    trainNetwork();
    perfectCpuVsRandom();
    networkCpuVsRandom();
    networkCpuVsCpuPly3();
    networkCpuVsPerfectCpu();

//    Game game;
//    for (auto & m : game.getMoves()) {
//        game.makeMove(m);
//        cout << network.predict(game.getTuples()) << endl;
//        game.undoMove();
//    }

    return 0;
}
