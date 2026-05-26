#ifndef AVLTREE_H
#define AVLTREE_H

#include <algorithm>
#include <cstddef>
#include <utility>

#include "stack.h"

template<typename Key, typename Value>
class AVLTree {
private:
    struct Node {
        Key key;
        Value value;
        Node* left;
        Node* right;
        int height;

        Node(const Key& k, const Value& v)
            : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root{nullptr};

    int getHeight(Node* node) const {
        return node ? node->height : 0;
    }

    int getBalance(Node* node) const {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }

    void updateHeight(Node* node) {
        if (node) {
            node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
        }
    }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        updateHeight(x);
        updateHeight(y);

        return y;
    }

    Node* insert(Node* node, const Key& key, const Value& value) {
        if (!node) {
            return new Node(key, value);
        }

        if (key < node->key) {
            node->left = insert(node->left, key, value);
        } else if (key > node->key) {
            node->right = insert(node->right, key, value);
        } else {
            node->value = value;
            return node;
        }

        updateHeight(node);
        int balance = getBalance(node);

        if (balance > 1 && key < node->left->key) {
            return rotateRight(node);
        }
        if (balance < -1 && key > node->right->key) {
            return rotateLeft(node);
        }
        if (balance > 1 && key > node->left->key) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (balance < -1 && key < node->right->key) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    Node* findNode(Node* node, const Key& key) const {
        if (!node) return nullptr;
        if (key == node->key) return node;
        if (key < node->key) return findNode(node->left, key);
        return findNode(node->right, key);
    }

    Node* getMin(Node* node) const {
        while (node && node->left) node = node->left;
        return node;
    }

    Node* deleteNode(Node* node, const Key& key, bool& erased) {
        if (!node) return nullptr;

        if (key < node->key) {
            node->left = deleteNode(node->left, key, erased);
        } else if (key > node->key) {
            node->right = deleteNode(node->right, key, erased);
        } else {
            erased = true;
            if (!node->left) {
                Node* temp = node->right;
                delete node;
                return temp;
            } else if (!node->right) {
                Node* temp = node->left;
                delete node;
                return temp;
            }

            Node* minNode = getMin(node->right);
            node->key = minNode->key;
            node->value = minNode->value;
            node->right = deleteNode(node->right, minNode->key, erased);
        }

        if (!node) return nullptr;

        updateHeight(node);
        int balance = getBalance(node);

        if (balance > 1 && getBalance(node->left) >= 0) {
            return rotateRight(node);
        }
        if (balance > 1 && getBalance(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (balance < -1 && getBalance(node->right) <= 0) {
            return rotateLeft(node);
        }
        if (balance < -1 && getBalance(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    void clear(Node* node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

public:
    class Iterator {
        using NodePtr = Node*;
        Stack<NodePtr> st;
        NodePtr cur{nullptr};

        void pushLeft(NodePtr n) {
            while (n) {
                st.push(n);
                n = n->left;
            }
        }

        void settle() {
            if (st.empty()) {
                cur = nullptr;
                return;
            }
            cur = st.top();
        }

    public:
        using value_type = std::pair<const Key, Value>;

        Iterator() = default;
        explicit Iterator(NodePtr root) {
            pushLeft(root);
            settle();
        }

        std::pair<const Key&, Value&> operator*() const {
            return {cur->key, cur->value};
        }

        struct PairPtr {
            const Key* first;
            Value* second;
        };

        PairPtr operator->() const {
            return {&cur->key, &cur->value};
        }

        Iterator& operator++() {
            if (!cur) return *this;
            NodePtr n = st.pop();
            if (n->right) pushLeft(n->right);
            settle();
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const Iterator& other) const {
            return cur == other.cur;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
    };

    AVLTree() = default;

    ~AVLTree() {
        clear(root);
    }

    AVLTree(const AVLTree&) = delete;
    AVLTree& operator=(const AVLTree&) = delete;

    void swap(AVLTree& other) noexcept {
        std::swap(root, other.root);
    }

    friend void swap(AVLTree& a, AVLTree& b) noexcept {
        a.swap(b);
    }

    void insert(const Key& key, const Value& value) {
        root = insert(root, key, value);
    }

    Value* find(const Key& key) {
        Node* node = findNode(root, key);
        return node ? &node->value : nullptr;
    }

    const Value* find(const Key& key) const {
        Node* node = findNode(root, key);
        return node ? &node->value : nullptr;
    }

    bool erase(const Key& key) {
        bool erased = false;
        root = deleteNode(root, key, erased);
        return erased;
    }

    Iterator begin() { return Iterator(root); }
    Iterator end() { return Iterator(); }

    Iterator begin() const { return Iterator(root); }
    Iterator end() const { return Iterator(); }

    bool empty() const {
        return root == nullptr;
    }

    void clear() {
        clear(root);
        root = nullptr;
    }
};

#endif // AVLTREE_H