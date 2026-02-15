#include <iostream>
#include <fstream>
#include <string>

using namespace std;

enum Color { RED, BLACK };

template <typename T>
class RedBlackTree {
private:
    struct Node {
        T data;
        Color color;
        Node *parent, *left, *right;
        int size; 

        Node(T val) : data(val), color(RED), parent(nullptr), 
                      left(nullptr), right(nullptr), size(1) {}
    };

    Node *root;
    Node *T_Nil;

    
    void updateSize(Node* x) {
        if (x != T_Nil) {
            x->size = x->left->size + x->right->size + 1;
        }
    }

    void rotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left != T_Nil) y->left->parent = x;
        y->parent = x->parent;
        if (x->parent == nullptr) root = y;
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;
        y->left = x;
        x->parent = y;
        
        
        updateSize(x);
        updateSize(y);
    }

    void rotateRight(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        if (x->right != T_Nil) x->right->parent = y;
        x->parent = y->parent;
        if (y->parent == nullptr) root = x;
        else if (y == y->parent->left) y->parent->left = x;
        else y->parent->right = x;
        x->right = y;
        y->parent = x;

        updateSize(y);
        updateSize(x);
    }

    void fixInsert(Node* k) {
        while (k != root && k->parent->color == RED) {
            if (k->parent == k->parent->parent->left) {
                Node* u = k->parent->parent->right;
                if (u->color == RED) {
                    u->color = BLACK;
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->right) {
                        k = k->parent;
                        rotateLeft(k);
                    }
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    rotateRight(k->parent->parent);
                }
            } else {
                Node* u = k->parent->parent->left;
                if (u->color == RED) {
                    u->color = BLACK;
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->left) {
                        k = k->parent;
                        rotateRight(k);
                    }
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    rotateLeft(k->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    void transplant(Node* u, Node* v) {
        if (u->parent == nullptr) root = v;
        else if (u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;
        v->parent = u->parent;
    }

    Node* minimum(Node* node) {
        while (node->left != T_Nil) node = node->left;
        return node;
    }

    void fixDelete(Node* x) {
        while (x != root && x->color == BLACK) {
            if (x == x->parent->left) {
                Node* s = x->parent->right;
                if (s->color == RED) {
                    s->color = BLACK;
                    x->parent->color = RED;
                    rotateLeft(x->parent);
                    s = x->parent->right;
                }
                if (s->left->color == BLACK && s->right->color == BLACK) {
                    s->color = RED;
                    x = x->parent;
                } else {
                    if (s->right->color == BLACK) {
                        s->left->color = BLACK;
                        s->color = RED;
                        rotateRight(s);
                        s = x->parent->right;
                    }
                    s->color = x->parent->color;
                    x->parent->color = BLACK;
                    s->right->color = BLACK;
                    rotateLeft(x->parent);
                    x = root;
                }
            } else {
                Node* s = x->parent->left;
                if (s->color == RED) {
                    s->color = BLACK;
                    x->parent->color = RED;
                    rotateRight(x->parent);
                    s = x->parent->left;
                }
                if (s->right->color == BLACK && s->left->color == BLACK) {
                    s->color = RED;
                    x = x->parent;
                } else {
                    if (s->left->color == BLACK) {
                        s->right->color = BLACK;
                        s->color = RED;
                        rotateLeft(s);
                        s = x->parent->left;
                    }
                    s->color = x->parent->color;
                    x->parent->color = BLACK;
                    s->left->color = BLACK;
                    rotateRight(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

    void deleteTree(Node* node) {
        if (node != T_Nil) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

public:
    RedBlackTree() {
        T_Nil = new Node(0);
        T_Nil->color = BLACK;
        T_Nil->size = 0;
        T_Nil->left = T_Nil->right = T_Nil->parent = nullptr;
        root = T_Nil;
    }

    ~RedBlackTree() {
        deleteTree(root);
        delete T_Nil;
    }

    bool insert(T key) {
        Node* z = root;
        Node* y = nullptr;
        while (z != T_Nil) {
            y = z;
            if (key == z->data) return false; 
            if (key < z->data) z = z->left;
            else z = z->right;
        }

        Node* node = new Node(key);
        node->parent = y;
        node->left = node->right = T_Nil;

        if (y == nullptr) root = node;
        else if (node->data < y->data) y->left = node;
        else y->right = node;

        //update sizes
        Node* temp = node->parent;
        while (temp != nullptr) {
            updateSize(temp);
            temp = temp->parent;
        }

        fixInsert(node);
        return true;
    }

bool remove(T key) {
        Node* z = root;
        while (z != T_Nil && z->data != key) {
            if (key < z->data) z = z->left;
            else z = z->right;
        }
        if (z == T_Nil) return false;

        Node* y = z;
        Node* x;
        Color y_original_color = y->color;
        Node* fixupStart = nullptr; // where to start size update
        
        if (z->left == T_Nil) {
            x = z->right;
            fixupStart = z->parent;
            transplant(z, z->right);
        } else if (z->right == T_Nil) {
            x = z->left;
            fixupStart = z->parent;
            transplant(z, z->left);
        } else {
            y = minimum(z->right);
            y_original_color = y->color;
            x = y->right;
            if (y->parent == z) {
                x->parent = y;
                fixupStart = y; // z's position after transplant
            } else {
                fixupStart = y->parent;
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
            if (fixupStart == z->parent) fixupStart = y;
        }

        delete z;

        // Update sizes from the point of deletion 
        Node* temp = fixupStart;
        while (temp != nullptr) {
            updateSize(temp);
            temp = temp->parent;
        }

        if (y_original_color == BLACK) fixDelete(x);
        return true;
    }

    bool search(T key) {
        Node* curr = root;
        while (curr != T_Nil) {
            if (key == curr->data) return true;
            if (key < curr->data) curr = curr->left;
            else curr = curr->right;
        }
        return false;
    }

    int getCountLess(T key) {
        Node* curr = root;
        int count = 0;
        while (curr != T_Nil) {
            if (key > curr->data) {
                count += curr->left->size + 1;
                curr = curr->right;
            } else {
                curr = curr->left;
            }
        }
        return count;
    }
};

int main() {
    ifstream fin("input_rbt.txt");
    ofstream fout("output.txt");
    
    int N; 
    if (!(fin >> N)) return 0;
    
    RedBlackTree<int> tree;
    fout << N << endl;

    for (int i = 0; i < N; i++) {
        int e, x;
        fin >> e >> x;
        if (e == 1) {
            bool success = tree.insert(x);
            fout << "1 " << x << " " << (success ? 1 : 0) << endl;
        } else if (e == 0) {
            bool success = tree.remove(x);
            fout << "0 " << x << " " << (success ? 1 : 0) << endl;
        } else if (e == 2) {
            bool found = tree.search(x);
            fout << "2 " << x << " " << (found ? 1 : 0) << endl;
        } else if (e == 3) {
            int result = tree.getCountLess(x);
            fout << "3 " << x << " " << result << endl;
        }
    }
    return 0;
}