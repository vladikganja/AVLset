#pragma once
#include "AVLtree.hpp"

struct AVL;
struct RED_BLACK;
struct NONE;

template <typename ValT, typename BalT = AVL, typename CmpT = std::less<>>
class MySet : public AVLtree<ValT, CmpT> {
public:
    template <typename ValT = ValT>
    typename std::enable_if<std::is_same<AVL, BalT>::value == true>::type insert(const ValT& val) {
        AVLtree<ValT, CmpT>::insert(val);
    }


};
