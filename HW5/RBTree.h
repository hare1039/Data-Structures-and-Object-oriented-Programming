#pragma GCC optimize("Ofast")
#ifndef RBTREE_H
#define RBTREE_H

#include <iostream>
#include <memory>
#include <string>

enum Color {
    BLACK,
    RED
}; // (true, false)
enum GENDER { MALE = 0,
    FEMALE,
    TOTAL };
static std::array<std::string, TOTAL> GENDER_NAME = { { "male", "female" } };

// ===================== Node ======================

struct Node : public std::enable_shared_from_this<Node> {
    int key, height, weight;
    bool sex;
    Color color = RED;
    std::shared_ptr<Node> left = nullptr, right = nullptr, p = nullptr;
    // Constructor
    Node(int key, const std::string& gender, int height, int weight)
        : key(key)
        , height(height)
        , weight(weight)
        , sex{ (gender == "female") ? true : false }
    {
    }
    inline const std::string& getGender() const { return GENDER_NAME[sex]; }
    inline const int getHeight() const { return height; }
    inline const int getWeight() const { return weight; }
};

// ===================== RBT =======================

class RBTree {
private:
    std::shared_ptr<Node> root = nullptr;
    inline void left_rotate(std::shared_ptr<Node>& x)
    {
        std::shared_ptr<Node> y = x->right;
        x->right = y->left;

        if (x->right != nullptr)
            x->right->p = x;

        y->p = x->p;

        if (x->p == nullptr)
            root = y;

        else {
            if (x == x->p->left)
                x->p->left = y;
            else
                x->p->right = y;
        }
        y->left = x;
        x->p = y;
    }
    inline void right_rotate(std::shared_ptr<Node>& x)
    {
        std::shared_ptr<Node> y = x->left;
        x->left = y->right;

        if (x->left != nullptr)
            x->left->p = x;

        y->p = x->p;

        if (x->p == nullptr)
            root = y;

        else {
            if (x == x->p->left)
                x->p->left = y;
            else
                x->p->right = y;
        }
        y->right = x;
        x->p = y;
    }
    // This function fixes violations caused by BST insertion
    inline void fix_violation(std::shared_ptr<Node>& x)
    {
        std::shared_ptr<Node> p_ptr = nullptr;
        std::shared_ptr<Node> grand_p_ptr = nullptr;

        while ((x != root) && (x->color != BLACK)
            && (x->p->color == RED)) {
            p_ptr = x->p;
            grand_p_ptr = x->p->p;

            //  Case A:
            //  p at grandp's left

            if (p_ptr == grand_p_ptr->left) {
                std::shared_ptr<Node> uncle_ptr = grand_p_ptr->right;

                //  Case 1:
                //  uncle is also RED

                if (uncle_ptr != nullptr && uncle_ptr->color == RED) {
                    grand_p_ptr->color = RED;
                    p_ptr->color = BLACK;
                    uncle_ptr->color = BLACK;
                    x = grand_p_ptr;
                }

                else {

                    // Case 2:
                    // x is right child of its p
                    // Left-rotation required

                    if (x == p_ptr->right) {
                        left_rotate(p_ptr);
                        x = p_ptr;
                        p_ptr = grand_p_ptr;
                    }

                    // Case 3:
                    // x is left child of its p
                    // Right-rotation required

                    right_rotate(grand_p_ptr);
                    std::swap(p_ptr->color, grand_p_ptr->color);
                    x = p_ptr;
                }
            }

            // Case B:
            // p at grandp's right

            else {
                std::shared_ptr<Node> uncle_ptr = grand_p_ptr->left;

                // Case 1:
                // uncle also RED

                if ((uncle_ptr != nullptr) && (uncle_ptr->color == RED)) {
                    grand_p_ptr->color = RED;
                    p_ptr->color = BLACK;
                    uncle_ptr->color = BLACK;
                    x = grand_p_ptr;
                }

                else {

                    // Case 2:
                    // x at p's Left

                    if (x == p_ptr->left) {
                        right_rotate(p_ptr);
                        x = p_ptr;
                        p_ptr = x->p;
                    }

                    // Case 3:
                    // x at p's x_right

                    left_rotate(grand_p_ptr);
                    std::swap(p_ptr->color, grand_p_ptr->color);
                    x = p_ptr;
                }
            }
        }
        root->color = BLACK;
    }

public:
    // Constructor
    RBTree() {}
    // BSTree insertion
    inline bool insert(int key, const std::string& gender, int height, int weight)
    {
        try {
            std::shared_ptr<Node> t = std::make_shared<Node>(key, gender, height, weight);
            std::shared_ptr<Node> x = root;
            std::shared_ptr<Node> y = nullptr;
            while (x != nullptr) {
                y = x;
                if (key == x->key)
                    return false;
                else if (key < x->key)
                    x = x->left;
                else
                    x = x->right;
            }
            t->p = y;
            if (y == nullptr)
                root = t;
            else {
                if (key == y->key)
                    return false;
                else if (t->key < y->key)
                    y->left = t;
                else
                    y->right = t;
            }
            fix_violation(t);
            return true;
        } catch (...) {
            return false;
        }
    }
    //  operator []
    inline Node& operator[](int key)
    {
        std::shared_ptr<Node> x = root;
        while (x->key != key && x != nullptr) {
            if (x->key > key)
                x = x->left;
            else
                x = x->right;
        }
        return *x;
    }
    inline const Node operator[](int key) const
    {
        std::shared_ptr<Node> x = root;
        while (x->key != key && x != nullptr) {
            if (x->key > key)
                x = x->left;
            else
                x = x->right;
        }
        return *x;
    }
};

#endif
