#include <iostream>

using namespace std;

class Node 
{
    public:
    int value;
    Node* left;
    Node* right;
    
    Node(int val) : value(val), left(NULL), right(NULL) {}
};

class BST 
{
    public:
    Node* root;

    BST() : root(nullptr) {}
    
    void insert(int val) 
    {
        root = insertHelper(root, val);
    }
    Node* insertHelper(Node* node, int val) 
    {
        if (node == nullptr) 
        {
            return new Node(val);
        }
        if (val < node->value) 
        {
            node->left = insertHelper(node->left, val);
        } else if (val > node->value) 
        {
            node->right = insertHelper(node->right, val);
        }
        return node;
    }

    bool search(int val) 
    {
        return searchHelper(root, val);
    }
    bool searchHelper(Node* node, int val) 
    {
        if (node == nullptr) 
        {
            return false;
        }
        if (node->value == val) 
        {
            return true;
        }
        if (val < node->value) 
        {
            return searchHelper(node->left, val);
        }
        return searchHelper(node->right, val);
    }
    void remove(int val) 
    {
        root = removeHelper(root, val);
    }

    Node* removeHelper(Node* node, int val) 
    {
        if (node == nullptr) 
        {
            return node;
        }
        if (val < node->value) 
        {
            node->left = removeHelper(node->left, val);
        } else if (val > node->value) 
        {
            node->right = removeHelper(node->right, val);
        } else {
            // Node found, remove it
            if (node->left == nullptr) 
            {
                Node* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) 
            {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            Node* temp = findMin(node->right);
            node->value = temp->value;
            node->right = removeHelper(node->right, temp->value);
        }
        return node;
    }

    Node* findMin(Node* node) 
    {
        while (node->left != nullptr) 
        {
            node = node->left;
        }
        return node;
    }
};

