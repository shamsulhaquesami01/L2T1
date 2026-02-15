#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <string>
#include <sstream>

using namespace std;

template<typename K, typename V>
class AVLNode {
public:
    K key;
    V value;
    AVLNode<K, V>* left, * right;
    int height;

    AVLNode(K k, V v) : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
};

template<typename K, typename V>
class AVLTree {
private:
    AVLNode<K, V>* root;

    int height(AVLNode<K, V>* n) { return n ? n->height : 0; }
    int getBalance(AVLNode<K, V>* n) { return n ? height(n->left) - height(n->right) : 0; }

    AVLNode<K, V>* rightRotate(AVLNode<K, V>* y) {
        AVLNode<K, V>* x = y->left;
        AVLNode<K, V>* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;
        return x;
    }

    AVLNode<K, V>* leftRotate(AVLNode<K, V>* x) {
        AVLNode<K, V>* y = x->right;
        AVLNode<K, V>* T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;
        return y;
    }

    AVLNode<K, V>* insert(AVLNode<K, V>* node, K key, V value, bool& success) {
        if (!node) {
            success = true;
            return new AVLNode<K, V>(key, value);
        }
        if (key < node->key) node->left = insert(node->left, key, value, success);
        else if (key > node->key) node->right = insert(node->right, key, value, success);
        else {
            success = false;
            return node;
        }

        node->height = 1 + max(height(node->left), height(node->right));
        int balance = getBalance(node);

        if (balance > 1 && key < node->left->key) return rightRotate(node);
        if (balance < -1 && key > node->right->key) return leftRotate(node);
        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }

    AVLNode<K, V>* minValueNode(AVLNode<K, V>* node) {
        AVLNode<K, V>* current = node;
        while (current->left) current = current->left;
        return current;
    }

    AVLNode<K, V>* deleteNode(AVLNode<K, V>* root, K key, bool& success) {
        if (!root) {
            success = false;
            return root;
        }
        if (key < root->key) root->left = deleteNode(root->left, key, success);
        else if (key > root->key) root->right = deleteNode(root->right, key, success);
        else {
            success = true;
            if (!root->left || !root->right) {
                AVLNode<K, V>* temp = root->left ? root->left : root->right;
                if (!temp) { temp = root; root = nullptr; }
                else *root = *temp;
                delete temp;
            }
            else {
                AVLNode<K, V>* temp = minValueNode(root->right);
                root->key = temp->key;
                root->value = temp->value;
                root->right = deleteNode(root->right, temp->key, success);
            }
        }
        if (!root) return root;

        root->height = 1 + max(height(root->left), height(root->right));
        int balance = getBalance(root);

        if (balance > 1 && getBalance(root->left) >= 0) return rightRotate(root);
        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
        if (balance < -1 && getBalance(root->right) <= 0) return leftRotate(root);
        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }
        return root;
    }


    void preOrder(AVLNode<K, V>* root, stringstream& ss) {
        if (root) {
            ss << root->key << " ";
            preOrder(root->left, ss);
            preOrder(root->right, ss);
        }
    }

    void inOrder(AVLNode<K, V>* root, stringstream& ss) {
        if (root) {
            inOrder(root->left, ss);
            ss << root->key << " ";
            inOrder(root->right, ss);
        }
    }

    void postOrder(AVLNode<K, V>* root, stringstream& ss) {
        if (root) {
            postOrder(root->left, ss);
            postOrder(root->right, ss);
            ss << root->key << " ";
        }
    }

    void levelOrder(AVLNode<K, V>* root, stringstream& ss) {
        if (!root) return;
        queue<AVLNode<K, V>*> q;
        q.push(root);
        while (!q.empty()) {
            AVLNode<K, V>* node = q.front();
            q.pop();
            ss << node->key << " ";
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
    }

public:
    AVLTree() : root(nullptr) {}

    bool insert(K key, V value) {
        bool success = false;
        root = insert(root, key, value, success);
        return success;
    }

    bool remove(K key) {
        bool success = false;
        root = deleteNode(root, key, success);
        return success;
    }

    string traverse(int type) {
        stringstream ss;
        if (type == 1) preOrder(root, ss);
        else if (type == 2) inOrder(root, ss);
        else if (type == 3) postOrder(root, ss);
        else if (type == 4) levelOrder(root, ss);

        string result = ss.str();
        //trailing space 
        if (!result.empty() && result[result.length() - 1] == ' ') {
            result = result.substr(0, result.length() - 1);
        }
        return result;
    }
};

int main() {
    ifstream inputFile("input_avl.txt");
    ofstream outputFile("output.txt");

    if (!inputFile.is_open()) return 1;

    int N;
    inputFile >> N;
    AVLTree<int, int> tree;
    vector<string> results;

    for (int i = 0; i < N; ++i) {
        int e, x;
        inputFile >> e >> x;
        if (e == 1) {
            bool res = tree.insert(x, x);
            results.push_back(to_string(e) + " " + to_string(x) + " " + (res ? "1" : "0"));
        }
        else if (e == 0) {
            bool res = tree.remove(x);
            results.push_back(to_string(e) + " " + to_string(x) + " " + (res ? "1" : "0"));
        }
        else if (e == 2) {
            results.push_back(tree.traverse(x));
        }
    }
    outputFile << results.size() << endl;
    for (const string& s : results) {
        outputFile << s << endl;
    }

    return 0;
}