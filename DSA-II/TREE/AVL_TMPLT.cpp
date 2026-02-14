#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <random>
#include <set>
#include <ctime>

using namespace std;


template<typename T>
class AVLNode {
public:
    T key;
    AVLNode<T>* left;
    AVLNode<T>*  right;
    int height;

    AVLNode(T k) {
        key = k;
        left = right = nullptr;
        height = 1;
    }
};

template<typename T>
class AVLTree {
private:
    AVLNode<T>* root;
    int height(AVLNode<T>* node) {
        if (node == nullptr) return 0;
        return node->height;
    }

    int balancefactor(AVLNode<T>* node) {
        if (node == nullptr) return 0;
        return height(node->left) - height(node->right);
    }

    AVLNode<T>* rightRotate(AVLNode<T>* y) {
        AVLNode<T>* x = y->left;
        AVLNode<T>* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height
            = max(height(y->left), height(y->right)) + 1;
        x->height
            = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    AVLNode<T>* leftRotate(AVLNode<T>* y) {
        AVLNode<T>* x = y->right;
        AVLNode<T>* T2 = x->left;

        x->left = y;
        y->right = T2;

        y->height
            = max(height(y->left), height(y->right)) + 1;
        x->height
            = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    AVLNode<T>* insert(AVLNode<T>* node, T key) {
        //normal bst insert

        if (node == nullptr)
            return new AVLNode<T>(key);

        if (key < node->key)
            node->left = insert(node->left, key);
        else if (key > node->key)
            node->right = insert(node->right, key);
        else
            return node;

        node->height = 1
            + max(height(node->left),
                height(node->right));

        int bf = balancefactor(node);
        if (bf > 1) {
            if (balancefactor(node->left) >= 0) {
                return rightRotate(node);          // LL
            }
            else {
                node->left = leftRotate(node->left); // LR
                return rightRotate(node);
            }
        }
        if (bf < -1) {
            if (balancefactor(node->right) <= 0) {
                return leftRotate(node);           // RR
            }
            else {
                node->right = rightRotate(node->right); // RL
                return leftRotate(node);
            }
        }
        return node;
    }
    AVLNode<T>* minValue(AVLNode<T>* node) {
        AVLNode<T>* current = node;
        while (current->left != nullptr)
            current = current->left;
        return current;
    }
    AVLNode<T>* deleteNode(AVLNode<T>* root, T key)
    {
        // Perform standard BST delete
        if (root == nullptr)
            return root;

        if (key < root->key)
            root->left = deleteNode(root->left, key);
        else if (key > root->key)
            root->right = deleteNode(root->right, key);
        else {
            // Node with only one child or no child
            if ((root->left == nullptr) || (root->right == nullptr)) {
                AVLNode<T>* temp = root->left ? root->left : root->right;

                if (temp == nullptr) {
                    delete root;
                    return nullptr;
                }
                else {
                    AVLNode<T>* old = root;
                    root = temp;
                    delete old;
                }
            }

            else {

                AVLNode<T>* temp
                    = minValue(root->right);
                root->key = temp->key;
                root->right
                    = deleteNode(root->right, temp->key);
            }
        }

        if (root == nullptr)
            return root;

        // Update height of the current node
        root->height = 1
            + max(height(root->left),
                height(root->right));

        // Get the balance factor of this node
        int balance = balancefactor(root);

        // If this node becomes unbalanced, then there are 4
        // cases

        // Left Left Case
        if (balance > 1 && balancefactor(root->left) >= 0)
            return rightRotate(root);

        // Left Right Case
        if (balance > 1 && balancefactor(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }

        // Right Right Case
        if (balance < -1 && balancefactor(root->right) <= 0)
            return leftRotate(root);

        // Right Left Case
        if (balance < -1
            && balancefactor(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }
    void inorder(AVLNode<T>* root)
    {
        if (root != nullptr) {
            inorder(root->left);
            cout << root->key << " ";
            inorder(root->right);
        }
    }

    void preorder(AVLNode<T>* root)
    {
        if (root != nullptr) {
            cout << root->key << " ";
            preorder(root->left);

            preorder(root->right);
        }
    }
    void postorder(AVLNode<T>* root)
    {
        if (root != nullptr) {
            postorder(root->left);
            postorder(root->right);
            cout << root->key << " ";
        }
    }

    // function to search for a key in the subtree rooted
    // with root
    bool search(AVLNode<T>* root, T key)
    {
        if (root == nullptr)
            return false;
        if (root->key == key)
            return true;
        if (key < root->key)
            return search(root->left, key);
        return search(root->right, key);
    }
    public:
    // Constructor to initialize the AVL tree
    AVLTree()
        : root(nullptr)
    {
    }

    // Function to insert a key into the AVL tree
    void insert(T key) { root = insert(root, key); }

    // Function to remove a key from the AVL tree
    void remove(T key) { root = deleteNode(root, key); }

    // Function to search for a key in the AVL tree
    bool search(T key) { return search(root, key); }

    // Function to print the inorder traversal of the AVL
    // tree
    void printInorder()
    {
        preorder(root);
        cout << endl;
    }
};
int main()
{
    AVLTree<int> avl;

    // Insert nodes into the AVL tree
    int N;cin>>N;
    while(N--){
        int e,k;
        cin>>e>>k;
        if(e==1){
            avl.insert(k);
        }
        if(e==2){
            if(k==1){
                avl.printInorder();
            }
        }
        if(e==0){
            avl.remove(k);
        }
    }

    return 0;
}
