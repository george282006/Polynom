//
// Created by ilya on 08.04.2026.
//

#pragma once

//Бинарное сбалансированное дерево поиска
//Каждая вершина покрашена или в красный или черный
//Корень чёрный
//Вершины бывают обычные и пустые у обычной ровно два сына, а все пустые - черные и без ключей
//Не бывает двух красных подряд
//На любом пути от корня до листа одинаковое количество черных вершин

template<typename Key, typename Value>
class RedBlackTree {
public:
    enum class Color {
        RED,
        BLACK
    };
private:
    struct Node
    {
        Key key;
        Value value;
        Color color;
        Node* left;
        Node* right;
        Node* parent;

        Node(Key k, Value v, Node* nil)
            : key(k), value(v), color(Color::RED),
              left(nil), right(nil), parent(nil) {}

        Node() {}
    };


    Node* root;
    Node* nil_;
    size_t size_;

    void leftRotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left != nil_)
            y->left->parent = x;

        y->parent = x->parent;

        if (x->parent == nil_)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;

        y->left = x;
        x->parent = y;
    }

    void rightRotate(Node* x) {
        Node* y = x->left;
        x->left = y->right;
        if (y->right != nil_)
            y->right->parent = x;

        y->parent = x->parent;

        if (x->parent == nil_)
            root = y;
        else if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;

        y->right = x;
        x->parent = y;
    }

    void fixInsert(Node* z) {
        while (z->parent->color == Color::RED) {

            //папа левый сыночек дедули
            if (z->parent == z->parent->parent->left) {

                Node* y = z->parent->parent->right;//Дядя

                //Красный дядя
                if (y->color == Color::RED) {
                    z->parent->color = Color::BLACK;
                    y->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;

                    z = z->parent->parent;//Проверяем дальше
                }
                else { //Чёрный дядя

                    if (z == z->parent->right) { //Лежим ли мы по другую сторону от папа чем папа от дедули
                        z = z->parent;
                        leftRotate(z);
                    }

                    z->parent->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    rightRotate(z->parent->parent);
                }
            }
            //папа правый сыночек дедули
            else {

                Node* y = z->parent->parent->left; //Дядя

                //Красный дядя
                if (y->color == Color::RED) {
                    z->parent->color = Color::BLACK;
                    y->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;

                    z = z->parent->parent;
                }
                else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rightRotate(z);
                    }

                    z->parent->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    leftRotate(z->parent->parent);
                }
            }
        }

        root->color = Color::BLACK;
    }

    void deleteFix(Node* x) {
    while (x != root && x->color == Color::BLACK) {

        if (x == x->parent->left) {
            Node* w = x->parent->right;// брат

            // брат красный
            if (w->color == Color::RED) {
                w->color = Color::BLACK;
                x->parent->color = Color::RED;
                leftRotate(x->parent);
                w = x->parent->right;
            }

            // брат чёрный и оба его сына чёрные
            if (w->left->color == Color::BLACK &&
                w->right->color == Color::BLACK) {

                w->color = Color::RED;
                x = x->parent;
            }
            else {
                // брат чёрный, левый сын красный, правый чёрный
                if (w->right->color == Color::BLACK) {
                    w->left->color = Color::BLACK;
                    w->color = Color::RED;
                    rightRotate(w);
                    w = x->parent->right;
                }

                // брат чёрный, правый сын красный
                w->color = x->parent->color;
                x->parent->color = Color::BLACK;
                w->right->color = Color::BLACK;
                leftRotate(x->parent);

                x = root;
            }
        }

        // x — правый сын
        else {
            Node* w = x->parent->left;

            if (w->color == Color::RED) {
                w->color = Color::BLACK;
                x->parent->color = Color::RED;
                rightRotate(x->parent);
                w = x->parent->left;
            }

            if (w->right->color == Color::BLACK &&
                w->left->color == Color::BLACK) {

                w->color = Color::RED;
                x = x->parent;
            }
            else {
                if (w->left->color == Color::BLACK) {
                    w->right->color = Color::BLACK;
                    w->color = Color::RED;
                    leftRotate(w);
                    w = x->parent->left;
                }

                w->color = x->parent->color;
                x->parent->color = Color::BLACK;
                w->left->color = Color::BLACK;
                rightRotate(x->parent);

                x = root;
            }
        }
    }

    x->color = Color::BLACK;
}

    void transplant(Node* u, Node* v) {
        if (u->parent == nil_) {
            root = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    Node* minimum(Node* x) {
        while (x->left != nil_)
            x = x->left;
        return x;
    }

    void cleaner(Node* node) {
        if (node != nil_) {
            cleaner(node->left);
            cleaner(node->right);
            delete node;
        }
    }

public:
    RedBlackTree()
    {
        nil_ = new Node();
        root = nil_;
        size_ = 0;
        nil_->color = Color::BLACK;
        nil_->parent = nil_;
        nil_->left = nil_;
        nil_->right = nil_;
    }

    ~RedBlackTree() {
        cleaner(root);
        delete nil_;
    }

    RedBlackTree(const RedBlackTree&) = delete;
    RedBlackTree& operator=(const RedBlackTree&) = delete;

    Node* find(const Key& k) const {
    Node* current = root;
    while (current != nil_) {
        if (k == current->key) {
            return current;
        } else if (k < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return nullptr;
}

    void insert(Key k, Value v) {
        Node* y = nil_;
        Node* x = root;

        while (x != nil_) {
            y = x;

            if (k == x->key) {
                x->value = v;
                return;
            } else if (k < x->key) {
                x = x->left;
            } else {
                x = x->right;
            }
        }
        
        Node* newNode = new Node(k, v, nil_);
        newNode->parent = y;

        if (y == nil_) {
            root = newNode;
        } else if (k < y->key) {
            y->left = newNode;
        } else {
            y->right = newNode;
        }


        fixInsert(newNode);
        ++size_;
    }

    void erase(Node* z) {

        if (z == nil_ || z == nullptr)
        {
            return;
        }
        Node* y = z;
        Node* x;
        Color yStColor = y->color;

        // Нет левого ребеночка
        if (z->left == nil_) {
            x = z->right;
            transplant(z, z->right);
        }
        // Нет правого ребёночка
        else if (z->right == nil_) {
            x = z->left;
            transplant(z, z->left);
        }
        // Два ребеночка
        else {
            y = minimum(z->right);
            yStColor = y->color;
            x = y->right;

            if (y->parent == z) {
                x->parent = y;
            } else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        delete z;

        if (yStColor == Color::BLACK) {
            deleteFix(x);
        }

        size_--;
    }


};
