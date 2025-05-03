#include <iostream>
#include <algorithm>
using namespace std;

struct Node
{
    int key;
    Node *left, *right;
    int height; // высота поддерева
    int size;   // количество узлов в поддереве

    Node(int k) : key(k), left(nullptr), right(nullptr), height(1), size(1) {}
};

// высота поддерева
int getHeight(Node *node)
{
    return node ? node->height : 0;
}

// размер поддерева
int getSize(Node *node)
{
    return node ? node->size : 0;
}

// Обновление параметров узла
void updateNode(Node *node)
{
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    node->size = 1 + getSize(node->left) + getSize(node->right);
}

// Правый поворот
Node *rightRotate(Node *y)
{
    Node *x = y->left;
    y->left = x->right;
    x->right = y;
    updateNode(y);
    updateNode(x);
    return x;
}

// Левый поворот
Node *leftRotate(Node *x)
{
    Node *y = x->right;
    x->right = y->left;
    y->left = x;
    updateNode(x);
    updateNode(y);
    return y;
}

// Вставка
Node *insert(Node *node, int key)
{
    if (!node)
        return new Node(key);

    if (key < node->key)
    {
        node->left = insert(node->left, key);
    }
    else if (key > node->key)
    {
        node->right = insert(node->right, key);
    }

    updateNode(node);

    // Балансировка
    int balance = getHeight(node->left) - getHeight(node->right);

    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    if (balance > 1 && key > node->left->key)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && key < node->right->key)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Поиск k-й порядковой статистики
Node *kthSmallest(Node *root, int k)
{
    if (!root || k <= 0 || k > root->size)
        return nullptr;

    int leftSize = getSize(root->left);

    if (k == leftSize + 1)
        return root;
    if (k <= leftSize)
        return kthSmallest(root->left, k);
    else
        return kthSmallest(root->right, k - leftSize - 1);
}

// Удаление дерева
void deleteTree(Node *root)
{
    if (!root)
        return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}
