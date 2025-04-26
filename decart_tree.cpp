#include <iostream>
#include <cstdlib>
#include <ctime>

template <typename T>
class Treap
{
private:
    struct Node
    {
        T value;
        int priority;
        int count;      // (узлы)
        int duplicates; // (количество повторений)
        Node *left;
        Node *right;

        Node(T val) : value(val), priority(rand()), count(1), duplicates(1),
                      left(nullptr), right(nullptr) {}
    };

    Node *root;

    // (Обновляем количество узлов в поддереве)
    void updateCount(Node *node)
    {
        if (node)
        {
            node->count = node->duplicates +
                          (node->left ? node->left->count : 0) +
                          (node->right ? node->right->count : 0);
        }
    }

    // (Разделение дерева по ключу)
    void split(Node *node, T key, Node *&left, Node *&right)
    {
        if (!node)
        {
            left = right = nullptr;
            return;
        }

        if (node->value <= key)
        {
            split(node->right, key, node->right, right);
            left = node;
        }
        else
        {
            split(node->left, key, left, node->left);
            right = node;
        }
        updateCount(node);
    }

    // (Слияние двух деревьев)
    Node *merge(Node *left, Node *right)
    {
        if (!left)
            return right;
        if (!right)
            return left;

        if (left->priority > right->priority)
        {
            left->right = merge(left->right, right);
            updateCount(left);
            return left;
        }
        else
        {
            right->left = merge(left, right->left);
            updateCount(right);
            return right;
        }
    }

    // (Рекурсивное удаление дерева)
    void clear(Node *node)
    {
        if (node)
        {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

public:
    Treap() : root(nullptr)
    {
        srand(time(0));
    }

    ~Treap()
    {
        clear(root);
    }

    // (Добавление элемента)
    void insert(T value)
    {
        Node *left, *right;
        split(root, value, left, right);

        // (Проверяем, есть ли уже такой элемент в левой части)
        Node *current = left;
        while (current && current->right)
        {
            current = current->right;
        }

        if (current && current->value == value)
        {
            // (Если элемент уже есть, увеличиваем счетчик дубликатов)
            current->duplicates++;
            updateCount(current);
            root = merge(left, right);
        }
        else
        {
            // (Иначе создаем новый узел)
            Node *newNode = new Node(value);
            root = merge(merge(left, newNode), right);
        }
    }

    // (Удаление одного вхождения элемента)
    void erase(T value)
    {
        Node *left, *mid, *right;
        split(root, value - 1, left, right);
        split(right, value, mid, right);

        if (mid)
        {
            if (mid->duplicates > 1)
            {
                mid->duplicates--;
                updateCount(mid);
                right = merge(mid, right);
            }
            else
            {
                // (Иначе удаляем узел полностью)
                delete mid;
                mid = nullptr;
            }
        }

        root = merge(left, right);
    }

    // (Получение общего количества элементов)
    int size() const
    {
        return root ? root->count : 0;
    }

    // (Проверка на пустоту)
    bool empty() const
    {
        return root == nullptr;
    }

    // Вывод элементов (для отладки)
    void print()
    {
        print(root);
        std::cout << std::endl;
    }

private:
    void print(Node *node)
    {
        if (node)
        {
            print(node->left);
            std::cout << node->value << node->duplicates;
            print(node->right);
        }
    }
};