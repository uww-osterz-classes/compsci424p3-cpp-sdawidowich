#pragma once

#include <iostream>
#include <random>

class Random {
private:
    std::random_device rd;
    std::mt19937 rng;

public:
    Random() : rng(rd()) { }
        
    int randomInt(int lower, int upper) {
        std::uniform_int_distribution<int> dist(lower, upper);

        return dist(this->rng);
    }
};