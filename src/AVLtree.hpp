#pragma once
#include <iostream>
#include <queue>
#include <stack>
#include <cassert>

template <typename ValT, typename CmpT = std::less<>>
class AVLtree {
private:
    template <typename ValT>
    class Node {
    public:
        Node(Node* parent, Node* left, Node* right, const ValT& val) :
            parent_(parent),
            left_(left),
            right_(right),
            height_(1),
            subtree_size_(1) {
            val_ = std::make_shared<ValT>(val);
        }
        Node* parent_;
        Node* left_;
        Node* right_;
        std::shared_ptr<ValT> val_;

        unsigned int height_;
        unsigned int subtree_size_;
    };

public:
    template <typename ValT>
    class primitive_AVLiterator {
    private:
        Node<ValT>* ptr_;

    public:

        primitive_AVLiterator(Node<ValT>* node = nullptr) : ptr_(node) {}
        primitive_AVLiterator(const primitive_AVLiterator& other) : ptr_(other.ptr_) {}

        primitive_AVLiterator& operator++() noexcept {
            assert(ptr_ != nullptr);
            if (ptr_->right_ == nullptr) {
                if (ptr_->parent_ != nullptr && ptr_->parent_->left_ == ptr_) {
                    ptr_ = ptr_->parent_;
                }
                else {
                    while (ptr_->parent_ != nullptr) {
                        if (ptr_->parent_->left_ == ptr_) {
                            break;
                        }
                        ptr_ = ptr_->parent_;
                    }
                    ptr_ = ptr_->parent_;
                }
            }
            else {
                ptr_ = ptr_->right_;
                while (ptr_->left_ != nullptr) {
                    ptr_ = ptr_->left_;
                }
            }
            return *this;
        }

        primitive_AVLiterator operator++(int) noexcept {
            primitive_AVLiterator tmp(*this);
            ++(*this);
            return tmp;
        }

        primitive_AVLiterator& operator--() noexcept {
            if (ptr_->left_ == nullptr) {
                if (ptr_->parent_ != nullptr && ptr_->parent_->right_ == ptr_) {
                    ptr_ = ptr_->parent_;
                }
                else {
                    while (ptr_->parent_ != nullptr) {
                        if (ptr_->parent_->right_ == ptr_) {
                            break;
                        }
                        ptr_ = ptr_->parent_;
                    }
                    ptr_ = ptr_->parent_;
                }
            }
            else {
                ptr_ = ptr_->left_;
                while (ptr_->right_ != nullptr) {
                    ptr_ = ptr_->right_;
                }
            }
            return *this;
        }

        primitive_AVLiterator operator--(int) noexcept {
            primitive_AVLiterator tmp(*this);
            --(*this);
            return tmp;
        }

        ValT& operator*() const noexcept {
            return *ptr_->val_;
        }

        bool operator==(const primitive_AVLiterator& other) const noexcept {
            return ptr_ == other.ptr_;
        }

        bool operator!=(const primitive_AVLiterator& other) const noexcept {
            return ptr_ != other.ptr_;
        }
    };

private:
    Node<ValT>* root_;
    Node<ValT>* begin_;
    Node<ValT>* rbegin_;
    size_t size_;

    size_t get_height(Node<ValT>* node) const {
        if (node == nullptr) {
            return 0;
        }
        else {
            return node->height_;
        }
    }

    size_t get_subtree_size(Node<ValT>* node) const {
        if (node == nullptr) {
            return 0;
        }
        else {
            return node->subtree_size_;
        }
    }

    void little_left_rotation(Node<ValT>* current) {
        Node<ValT>* a = current;
        Node<ValT>* b = current->right_;

#ifndef NDEBUG
        if (std::is_arithmetic<ValT>::value == true) {
            std::cout << "little left rotation;\ta.val = " << *a->val_ << ";\tb.val = " << *b->val_ << '\n';
        }
#endif

        if (a != root_) {
            if (a == a->parent_->right_) {
                a->parent_->right_ = b;
            }
            else if (a == a->parent_->left_) {
                a->parent_->left_ = b;
            }
        }
        else {
            root_ = b;
        }

        b->parent_ = a->parent_;
        a->parent_ = a->right_;
        a->right_ = b->left_;
        b->left_ = a;

        if (a->right_ != nullptr) {
            a->right_->parent_ = a;
        }

        a->height_ = std::max(get_height(a->left_), get_height(a->right_)) + 1;
        b->height_ = std::max(get_height(b->left_), get_height(b->right_)) + 1;
        a->subtree_size_ = get_subtree_size(a->left_) + get_subtree_size(a->right_) + 1;
        b->subtree_size_ = get_subtree_size(b->left_) + get_subtree_size(b->right_) + 1;
    }

    void big_left_rotation(Node<ValT>* current) {
        Node<ValT>* a = current;
        Node<ValT>* b = current->right_;
        Node<ValT>* c = current->right_->left_;

#ifndef NDEBUG
        if (std::is_arithmetic<ValT>::value == true) {
            std::cout << "big left rotation;\ta.val = " << *a->val_ << ";\tb.val = " << *b->val_ << ";\tc.val = " << *c->val_ << '\n';
        }
#endif

        if (a != root_) {
            if (a == a->parent_->right_) {
                a->parent_->right_ = c;
            }
            else if (a == a->parent_->left_) {
                a->parent_->left_ = c;
            }            
        }
        else {
            root_ = c;
        }

        c->parent_ = a->parent_;
        b->parent_ = c;
        a->parent_ = c;
        a->right_ = c->left_;
        b->left_ = c->right_;
        c->left_ = a;
        c->right_ = b;

        if (a->right_ != nullptr) {
            a->right_->parent_ = a;
        }
        if (b->left_ != nullptr) {
            b->left_->parent_ = b;
        }

        a->height_ = std::max(get_height(a->left_), get_height(a->right_)) + 1;
        b->height_ = std::max(get_height(b->left_), get_height(b->right_)) + 1;
        c->height_ = std::max(get_height(a), get_height(b)) + 1;
        a->subtree_size_ = get_subtree_size(a->left_) + get_subtree_size(a->right_) + 1;
        b->subtree_size_ = get_subtree_size(b->left_) + get_subtree_size(b->right_) + 1;
        c->subtree_size_ = get_subtree_size(c->left_) + get_subtree_size(c->right_) + 1;
    }

    void little_right_rotation(Node<ValT>* current) {
        Node<ValT>* a = current;
        Node<ValT>* b = current->left_;

#ifndef NDEBUG
        if (std::is_arithmetic<ValT>::value == true) {
            std::cout << "little right rotation;\ta.val = " << *a->val_ << ";\tb.val = " << *b->val_ << '\n';
        }        
#endif

        if (a != root_) {
            if (a == a->parent_->right_) {
                a->parent_->right_ = b;
            }
            else if (a == a->parent_->left_) {
                a->parent_->left_ = b;
            }
        }
        else {
            root_ = b;
        }

        b->parent_ = a->parent_;
        a->parent_ = a->left_;
        a->left_ = b->right_;
        b->right_ = a;

        if (a->left_ != nullptr) {
            a->left_->parent_ = a;
        }

        a->height_ = std::max(get_height(a->left_), get_height(a->right_)) + 1;
        b->height_ = std::max(get_height(b->left_), get_height(b->right_)) + 1;
        a->subtree_size_ = get_subtree_size(a->left_) + get_subtree_size(a->right_) + 1;
        b->subtree_size_ = get_subtree_size(b->left_) + get_subtree_size(b->right_) + 1;
    }

    void big_right_rotation(Node<ValT>* current) {
        Node<ValT>* a = current;
        Node<ValT>* b = current->left_;
        Node<ValT>* c = current->left_->right_;

#ifndef NDEBUG
        if (std::is_arithmetic<ValT>::value == true) {
            std::cout << "big right rotation;\ta.val = " << *a->val_ << ";\tb.val = " << *b->val_ << ";\tc.val = " << *c->val_ << '\n';
        }
#endif

        if (a != root_) {
            if (a == a->parent_->right_) {
                a->parent_->right_ = c;
            }
            else if (a == a->parent_->left_) {
                a->parent_->left_ = c;
            }
        }
        else {
            root_ = c;
        }

        c->parent_ = a->parent_;
        b->parent_ = c;
        a->parent_ = c;
        a->left_ = c->right_;
        b->right_ = c->left_;
        c->left_ = b;
        c->right_ = a;

        if (a->left_ != nullptr) {
            a->left_->parent_ = a;
        }
        if (b->right_ != nullptr) {
            b->right_->parent_ = b;
        }

        a->height_ = std::max(get_height(a->left_), get_height(a->right_)) + 1;
        b->height_ = std::max(get_height(b->left_), get_height(b->right_)) + 1;
        c->height_ = std::max(get_height(a), get_height(b)) + 1;
        a->subtree_size_ = get_subtree_size(a->left_) + get_subtree_size(a->right_) + 1;
        b->subtree_size_ = get_subtree_size(b->left_) + get_subtree_size(b->right_) + 1;
        c->subtree_size_ = get_subtree_size(c->left_) + get_subtree_size(c->right_) + 1;
    }

    void subtrees_sizes_change(Node<ValT>* cur, int val) noexcept {
        while (cur != nullptr) {
            cur->subtree_size_ += val;
            cur = cur->parent_;
        }
    }

    void balance(Node<ValT>* cur) {
        while (cur != nullptr) {
            cur->height_ = std::max(get_height(cur->left_), get_height(cur->right_)) + 1;

            size_t left_subtree_height = get_height(cur->left_);
            size_t right_subtree_height = get_height(cur->right_);

            if (right_subtree_height == left_subtree_height + 2) {
                size_t sub_subtree_C = get_height(cur->right_->left_);
                size_t sub_subtree_R = get_height(cur->right_->right_);
                if (sub_subtree_C <= sub_subtree_R) {
                    little_left_rotation(cur);
                }
                else {
                    big_left_rotation(cur);
                }
            }
            else if (left_subtree_height == right_subtree_height + 2) {
                size_t sub_subtree_L = get_height(cur->left_->left_);
                size_t sub_subtree_C = get_height(cur->left_->right_);
                if (sub_subtree_C <= sub_subtree_L) {
                    little_right_rotation(cur);
                }
                else {
                    big_right_rotation(cur);
                }
            }

            cur = cur->parent_;
        }
    }

    // Returns either the pointer to the searched node or to the node right above or nullprt if tree is empty
    Node<ValT>* upper_bound(const ValT& val) const {
        if (size_ == 0) {
            return nullptr;
        }

        CmpT cmp = CmpT();

        Node<ValT>* cur = root_;
        while (true) {
            if (val == *cur->val_) {
                return cur;
            }

            if (cmp(val, *cur->val_)) {
                if (cur->left_ != nullptr) {
                    cur = cur->left_;
                }
                else {
                    return cur;
                }
            }
            else {
                if (cur->right_ != nullptr) {
                    cur = cur->right_;
                }
                else {
                    return cur;
                }
            }
        }
    }

public:

    AVLtree() : root_(nullptr), begin_(nullptr), rbegin_(nullptr), size_(0) {}
    ~AVLtree() {

    }

    primitive_AVLiterator<ValT> begin() const {
        return begin_;
    }

    primitive_AVLiterator<ValT> end() const {
        return primitive_AVLiterator<ValT>();
    }

    template <typename ValT = ValT>
    typename std::enable_if<std::is_arithmetic<ValT>::value == true>::type debug_iter_print() {
        if (size_ == 0) {
            return;
        }
        Node<ValT>* cur = root_;
        std::stack<Node<ValT>*> returns;
        while (!returns.empty() || cur != nullptr) {
            if (cur != nullptr) {
                returns.push(cur);
                cur = cur->left_;
            }
            else {
                cur = returns.top();
                returns.pop();
                std::cout << *cur->val_ << '\t' << cur->height_ << '\t' << cur->subtree_size_ << '\n';
                cur = cur->right_;
            }
        }
    }

    void insert(const ValT& val) {
        if (size_ == 0) {
            root_ = begin_ = new Node<ValT>(nullptr, nullptr, nullptr, val);
            size_++;
            return;
        }

        CmpT cmp = CmpT();

        Node<ValT>* cur = root_;
        bool beg = true; // check if inserted value is at begin() position
        bool rbeg = true; // check if inserted value is at rbegin() position
        while (true) {
            if (val == *cur->val_) {
                return;
            }

            if (cmp(val, *cur->val_)) {
                rbeg = false;
                if (cur->left_ != nullptr) {
                    cur = cur->left_;                    
                }
                else {
                    cur->left_ = new Node<ValT>(cur, nullptr, nullptr, val);
                    cur->height_++;
                    size_++;
                    break;
                }
            }
            else {
                beg = false;
                if (cur->right_ != nullptr) {
                    cur = cur->right_;
                }
                else {
                    cur->right_ = new Node<ValT>(cur, nullptr, nullptr, val);
                    cur->height_++;
                    size_++;
                    break;
                }
            }
        }

        if (beg) {
            begin_ = cur->left_;
        }
        else if (rbeg) {
            rbegin_ = cur->right_;
        }

        subtrees_sizes_change(cur, 1);
        balance(cur);
    }

    primitive_AVLiterator<ValT> find(const ValT& val) const {
        Node<ValT>* finder = upper_bound(val);
        if (*finder->val_ == val) {
            return primitive_AVLiterator<ValT>(finder);
        }
        return primitive_AVLiterator<ValT>();
    }

    void erase(const ValT& val) {
        Node<ValT>* finder = upper_bound(val);
        if (*finder->val_ == val) {
            Node<ValT>* cur = finder;
            if (finder->left_ != nullptr) {
                cur = finder->left_;
                while (cur->right_ != nullptr) {
                    cur = cur->right_;
                }
                finder->val_.swap(cur->val_);

                if (cur == finder->left_) {
                    delete cur;
                    finder->left_ = nullptr;
                }
                else {
                    cur = cur->parent_;
                    delete cur->right_;
                    cur->right_ = nullptr;
                }
            }
            else {
                if (finder == root_) {
                    root_ = finder->right_;
                    root_->parent_ = nullptr;
                    cur = root_;
                }
                else {
                    cur = finder->parent_;
                    if (finder->parent_->right_ == finder) {
                        finder->parent_->right_ = finder->right_;
                        if (finder->right_ != nullptr) {
                            finder->right_->parent_ = finder->parent_;
                        }
                    }
                    else {
                        finder->parent_->left_ = finder->right_;
                        if (finder->right_ != nullptr) {
                            finder->right_->parent_ = finder->parent_;
                        }
                    }
                }                
                
                delete finder;
            }

            size_--;
            subtrees_sizes_change(cur, -1);
            balance(cur);
        }
    }

    size_t size() const {
        return size_;
    }

    ValT& get_element_by_order(size_t k) const {
        if (k >= size_) {
            return *rbegin_->val_;
        }
        Node<ValT>* cur = root_;
        while (k != 1 || cur->left_ != nullptr) {
            if (k > get_subtree_size(cur->left_)) {
                if (k == get_subtree_size(cur->left_) + 1) {
                    break;
                }
                k -= (get_subtree_size(cur->left_) + 1);
                cur = cur->right_;
            }
            else {
                cur = cur->left_;
            }
        }
        return *cur->val_;
    }

    size_t count_elements_less(const ValT& val) const {
        CmpT cmp = CmpT();
        if (cmp(val, *begin_->val_)) {
            return 0;
        }
        else if (cmp(*rbegin_->val_, val)) {
            return size_;
        }

        Node<ValT>* cur = root_;
        size_t res = 0;
        while (cur != nullptr) {
            if (val == *cur->val_) {
                res += get_subtree_size(cur->left_);
                break;
            }
            else if (cmp(*cur->val_, val)) {
                res += get_subtree_size(cur->left_) + 1;
                cur = cur->right_;
            }
            else {
                cur = cur->left_;
            }
        }
        return res;
    }

    size_t count_elements_greater(const ValT& val) const {
        CmpT cmp = CmpT();
        if (cmp(val, *begin_->val_)) {
            return size_;
        }
        else if (cmp(*rbegin_->val_, val)) {
            return 0;
        }

        Node<ValT>* cur = root_;
        size_t res = size_;
        while (cur != nullptr) {
            if (val == *cur->val_) {
                res -= get_subtree_size(cur->left_) + 1;
                break;
            }
            else if (cmp(*cur->val_, val)) {
                res -= get_subtree_size(cur->left_) + 1;
                cur = cur->right_;
            }
            else {
                cur = cur->left_;
            }
        }
        return res;
    }

    void clear() {

    }

};




