#include "gtest/gtest.h"
#include "AVLtree.hpp"
#include <chrono>
#include <random>

TEST(test, test1) {

    std::mt19937 gen(static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> k(0, 100'000);
    AVLtree<int> tree;
    for (int i = 0; i < 10000; i++) {
        tree.insert(k(gen));
        std::cout << std::endl;
    }
}

