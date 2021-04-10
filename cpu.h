#ifndef CPU_H
#define CPU_H

#include <unordered_map>
#include <algorithm>
#include <cmath>
#include "game.h"
#include "ntuple.h"

#define EXACT 0
#define LOWER 1
#define UPPER 2

#define INF 100000

using namespace std;

class Move {
public:
    int move;
    float score;

    explicit Move(int move) : move(move) {
        score = 0;
    }

    bool operator < (const Move& move) const {
        return score < move.score;
    }

    bool operator > (const Move& move) const {
        return score > move.score;
    }

    friend ostream& operator<< (ostream &out, Move &move) {
        out << move.score << ": " << move.move;
        return out;
    }
};

class TTEntry {
public:
    float value = 0;
    int flag = 0;
    int age = 0;
    int move;
};

class Cpu {
public:
    explicit Cpu() {

    }

    void setPlayer(int player) {
        this->player = player;
    }

    int getPlayer() {
        return player;
    }

    void setNetwork(NTupleNetwork *network) {
        this->network = network;
    }

    void setGame(Game *game) {
        this->game = game;
    }

    Move getBestMove(int levels) {
        vector<int> movesNum = game->getMoves();
        vector<Move> moves; moves.reserve(movesNum.size());
        for (auto & moveNum : movesNum) {
            Move move = Move(moveNum);
            game->makeMove(move.move);
            if (game->isOver()) {
                int winner = game->getWinner();
                if (winner == player) {
                    move.score = INF - 10 * game->rounds;
                } else if (winner == (player^1) ){
                    move.score = -INF + 10 * game->rounds;
                }
            } else {
                move.score = getScore();
            }
            game->undoMove();
            moves.push_back(move);
        }

        Move bestMoveSoFar = getBestMoveSoFar(moves);
        if (bestMoveSoFar.score > INF/2) {
            return bestMoveSoFar;
        }

        tt.clear();
        for (int i=1; i < levels; i++) {
            age = i;
            for (auto & move : moves) {
                if (move.score < -INF/2) {
                    continue;
                }
                game->makeMove(move.move);
                move.score = -getScore(-1, i-1, -INF, INF);
                game->undoMove();
                if (move.score > INF/2) {
                    return move;
                }
                if (move.score > bestMoveSoFar.score) {
                    bestMoveSoFar = move;
                }
            }
            bestMoveSoFar = getBestMoveSoFar(moves);
            if (bestMoveSoFar.score < -INF/2) {
                break;
            }
        }

        return bestMoveSoFar;
    }

private:
    int player;
    Game *game;
    unordered_map<int,TTEntry> tt;
    int age;
    NTupleNetwork *network = nullptr;

    Move getBestMoveSoFar(vector<Move> & moves) {
        sort(moves.begin(),moves.end(), [](const Move & a, const Move & b) -> bool
        {
            return a.score > b.score;
        });

        float score = moves[0].score;
        int n = 1;

        for (size_t i=1; i < moves.size(); i++) {
            if (moves[i].score < score) break;
            n++;
        }

//        for (auto & m : moves) {
//            cout << m.move << ": " << m.score << endl;
//        }

        return moves[rand() % n];
    }

    float getScore() {
        if (network != nullptr) {
            float s = network->predict(game->getTuples());
            return player == PLAYER_X ? s : -s;
        }
        return 0;
    }

    float getScore(int color, float level, float alpha, float beta) {
        float output = -INF;

        int state;
        int bestMove = -1;
        float alphaOrig = alpha;

        if (level > 0) {
            state = game->getHash();
            if (tt.find(state) != tt.end()) {
                auto & entry = tt[state];
                if (entry.age != age) {
                    bestMove = entry.move;
                } else {
                    if (entry.flag == EXACT) {
                        return entry.value;
                    } else if (entry.flag == LOWER) {
                        alpha = max(alpha, entry.value);
                    } else if (entry.flag == UPPER) {
                        beta = min(beta, entry.value);
                    }
                    if (alpha >= beta) {
                        return entry.value;
                    }
                }

            }
        }

        vector<int> moves = game->getMoves();

        if (level > 0) {
            if (bestMove != -1) {
                auto it = find(moves.begin(),moves.end(),bestMove);
                if (it != moves.end()) {
                    iter_swap(moves.begin(), it);
                }
            }
        }

        for (auto & move : moves) {
            float score = 0;
            game->makeMove(move);
            if (game->isOver()) {
                int winner = game->getWinner();
                if (winner == player) {
                    score = INF - 10 * game->rounds;
                } else if (winner == (player^1)) {
                    score = -INF + 10 * game->rounds;
                }
                score *= color;
            } else if (level > 0) {
                score = -getScore(-color, level-1, -beta, -alpha);
            } else {
                score = color * getScore();
            }
            game->undoMove();
            if (score > output) {
                bestMove = move;
            }
            output = max(score, output);
            alpha = max(alpha, output);
            if (alpha >= beta) {
                break;
            }
        }

        if (level > 0) {
            TTEntry entry;
            entry.value = output;
            entry.age = age;

            if (output <= alphaOrig) {
                entry.flag = UPPER;
            } else if (output >= beta) {
                entry.move = bestMove;
                entry.flag = LOWER;
            } else {
                entry.move = bestMove;
                entry.flag = EXACT;
            }

            tt[state] = entry;
        }

        return output;
    }
};

#endif // CPU_H
