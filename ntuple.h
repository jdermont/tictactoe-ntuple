#ifndef NTUPLE_H
#define NTUPLE_H

#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <vector>
#include <cmath>

using namespace std;

class NTupleNetwork {
public:
    float learningRate = 0.01;

    explicit NTupleNetwork() {

    }

    void learn(const vector<int> &tuples, float target) {
        float score = 0;
        for (size_t i=0; i < tuples.size(); i++) {
            score += weights[i][tuples[i]];
        }
        score = tanh(score+bias);

        float error = target - score;
        float delta = error * tanh_prime(score);
        for (size_t i=0; i < tuples.size(); i++) {
            weights[i][tuples[i]] += learningRate * delta;
        }
        bias += learningRate * delta;
    }

    float predict(const vector<int> &tuples) {
        float output = 0;
        for (size_t i=0; i < tuples.size(); i++) {
            output += weights[i][tuples[i]];
        }
        return tanh(output+bias);
    }

    void learnSym(const vector<int> &tuples, float target) {
        float score = 0;
        score += weightsSym[0][tuples[0]];
        score += weightsSym[1][tuples[1]];
        score += weightsSym[0][tuples[2]];
        score += weightsSym[0][tuples[3]];
        score += weightsSym[1][tuples[4]];
        score += weightsSym[0][tuples[5]];
        score += weightsSym[2][tuples[6]];
        score += weightsSym[2][tuples[7]];
        score = tanh(score+biasSym);

        float error = target - score;
        float delta = error * tanh_prime(score);
        weightsSym[0][tuples[0]] += learningRate * delta;
        weightsSym[1][tuples[1]] += learningRate * delta;
        weightsSym[0][tuples[2]] += learningRate * delta;
        weightsSym[0][tuples[3]] += learningRate * delta;
        weightsSym[1][tuples[4]] += learningRate * delta;
        weightsSym[0][tuples[5]] += learningRate * delta;
        weightsSym[2][tuples[6]] += learningRate * delta;
        weightsSym[2][tuples[7]] += learningRate * delta;
        biasSym += learningRate * delta;
    }

    float predictSym(const vector<int> &tuples) {
        float output = 0;
        output += weightsSym[0][tuples[0]];
        output += weightsSym[1][tuples[1]];
        output += weightsSym[0][tuples[2]];
        output += weightsSym[0][tuples[3]];
        output += weightsSym[1][tuples[4]];
        output += weightsSym[0][tuples[5]];
        output += weightsSym[2][tuples[6]];
        output += weightsSym[2][tuples[7]];
        return tanh(output+biasSym);
    }

private:
    float weights[8][54] = {};
    float bias = 0;

    float weightsSym[3][54] = {};
    float biasSym = 0;

    float tanh_prime(float x) { // x already tanhed
        return 1 - x*x;
    }
};

#endif // NTUPLE_H
