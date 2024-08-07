#include <iostream>
#include <queue>
#include <stack>
#include <cstdlib>

struct Node {
    int data;
    Node* left;
    Node* right;
    Node(const int& data = 0) : data(data), left(nullptr), right(nullptr) {}
};

class BinarySearchTree {
public:
    BinarySearchTree() : root(nullptr) {}

    BinarySearchTree(const int& data) : root(new Node(data)) {}

    BinarySearchTree(Node* node) : root(node) {}

    BinarySearchTree(const int& bottom, const int& top, const int& nodes_count) {
        if(nodes_count <= 0) {
            root = nullptr;
            return;
        }
        srand(time(NULL));
        
        root = new Node(bottom + rand() % top);
        for(int node = 1; node < nodes_count; ++node)
            this->insert(bottom + rand() % top);
    }

    BinarySearchTree(const BinarySearchTree& other) : root(nullptr) {
        copy(other.root, root);
    }

    ~BinarySearchTree() {
        destroy(root);
    }

    BinarySearchTree& operator=(const BinarySearchTree& other) {
        if (this != &other) {
            destroy(root);
            copy(other.root, root);
        }
        return *this;
    }
    
    Node* find(const int& key);
    Node* findMin();
    Node* findMax();    
    void insert(const int& data);
    void remove(const int& key);
    void levelOrder();
    void inOrder();

private:
    Node* root;

    void destroy(Node* node) {
        if (node == NULL) 
            return;

        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    void copy(Node* source, Node* &dest) {
        if (source == NULL) 
            return;

        dest = new Node(source->data);
        copy(source->left, dest->left);
        copy(source->right, dest->right);
    }
};

Node* BinarySearchTree::find(const int& key) {
    Node* curr = root;
    while (curr && curr->data != key)
    {
        if (key < curr->data)
            curr = curr->left;
        else
            curr = curr->right;
    }
    return curr;
}

Node* BinarySearchTree::findMin() {
    Node* min = root;
    while(min && min->left)
        min = min->left;
    return min;
}

Node* BinarySearchTree::findMax() {
    Node* max = root;
    while(max && max->right)
        max = max->right;
    return max;
}

void BinarySearchTree::insert(const int& data) {
    Node* newNode = new Node(data);
    Node* curr = root;
    Node* parent = nullptr;
    while (curr) 
    {
        parent = curr;
        if (data < curr->data)
            curr = curr->left; 
        else
            curr = curr->right;
    }
    if (parent == nullptr) 
        root = newNode;
    else if (data < parent->data)
        parent->left = newNode;
    else
        parent->right = newNode;
}

void BinarySearchTree::remove(const int& key) {
    Node* curr = root;
    Node* parent = nullptr;
    while (curr && curr->data != key) 
    {
        parent = curr;
        if (key < curr->data)
            curr = curr->left; 
        else
            curr = curr->right;
    }

    if (!curr)
        return;

    if (!curr->left && !curr->right) 
    {
        if (curr == root)
            root = nullptr;
        else if (curr == parent->left)
            parent->left = nullptr;
         
        else
            parent->right = nullptr;
        delete curr;
    } 
    else if (curr->left && !curr->right) 
    {
        if (curr == root)
            root = curr->left;
        else if (curr == parent->left) 
            parent->left = curr->left;
        else
            parent->right = curr->left;

        delete curr;
    }
    else if (!curr->left && curr->right) 
    {
        if (curr == root)
            root = curr->right;
        else if (curr == parent->left)
            parent->left = curr->right;
        else
            parent->right = curr->right;
        delete curr;
    } 
    else 
    {
        Node* successor = curr->right;
        parent = curr;
        while (successor->left) 
        {
            parent = successor;
            successor = successor->left;
        }
        curr->data = successor->data;
        if (successor == parent->left)
            parent->left = successor->right; 
        else
            parent->right = successor->right;
        delete successor;
    }
}

void BinarySearchTree::levelOrder() {
    using namespace std;

    if(root == nullptr)
        return;

    queue<Node*> nodesQueue;
    nodesQueue.push(root);
    Node *curr;
    while(!nodesQueue.empty())
    {
        curr = nodesQueue.front();
        cout << curr->data << ' ';
        if(curr->left)
            nodesQueue.push(curr->left);
        if(curr->right)
            nodesQueue.push(curr->right);
        nodesQueue.pop();
    }
}

void BinarySearchTree::inOrder() {
    using namespace std;

    if(root == nullptr)
        return;
    
    Node* curr = root;
    stack<Node*> nodesStack;
    nodesStack.push(curr);
    
    while(!nodesStack.empty())
    {
        if(!nodesStack.empty())
        {
            curr = nodesStack.top();
            cout << curr->data << ' ';
            nodesStack.pop();
            if(curr->right)
                curr = curr->right;
            else
                curr = nullptr;
        }
        while(curr)
        {
            nodesStack.push(curr);
            curr = curr->left;
        }
    }
}

BinarySearchTree random_tree(int nodes_count) {
    srand(time(NULL));
    if(nodes_count <= 0)
        return BinarySearchTree(0);

    BinarySearchTree tree;
    for(int node = 0; node < nodes_count; node++)
        tree.insert(-50 + rand() % 100);
    return tree;
}

int main(int argc, char const *argv[])
{
    using namespace std;      
    
    BinarySearchTree tree(0, 100, 14);
    tree.levelOrder();
    Node* min = tree.findMin();
    Node* max = tree.findMax();
    cout << endl << "min " << min->data << " max " << max->data << endl; 

    tree.inOrder();

    return 0;
}
