#include <iostream>
#include <set>
#include <vector>
#include <random>
#include <chrono>
#include "AVLtree.hpp"
#include "MySet.hpp"

int main() {

    std::mt19937 gen(static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> k(0, 100);
    AVLtree<int> tree;

    for (int i = 0; i < 10; i++) {
        //tree.insert(k(gen));
        int x;
        std::cin >> x;
        tree.insert(x);
    }

    tree.debug_iter_print();
    std::cout << std::endl;

    for (auto el : tree) {
        std::cout << el << ' ';
    }
    std::cout << std::endl;

    for (int i = 0; i < 5; i++) {
        int x;
        std::cin >> x;
        tree.erase(x);

        tree.debug_iter_print();
    }

    return 0;
}
