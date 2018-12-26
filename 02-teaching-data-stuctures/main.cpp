#include <iostream>

using namespace std;

int intmax(int a, int b) {
    return (a > b) ? a : b;
}

enum RotationType {

    LEFT, RIGHT

};

struct Node {

    Node(int value): value(value), parent(nullptr), right(nullptr), left(nullptr) {}

    int value;
    Node* parent;
    Node* right;
    Node* left;

};

struct AVLNode {

    AVLNode(int value, Node* link): value(value), parent(nullptr), right(nullptr), left(nullptr), link(link), size(1) {}

    int value;
    AVLNode* parent;
    AVLNode* right;
    AVLNode* left;
    Node* link;
    int size;

};

class Tree {

public:

    Tree(): root(nullptr), avlRoot(nullptr) {}

    /**
     * Inserts element into the tree
     *
     * @param x element to be inserted
     */
    void insert(int x) {
        Node* newNode = new Node(x);
        AVLNode* avlPlace = findAVLInsertPlace(avlRoot, x);

        if(avlPlace == nullptr && avlRoot == nullptr) {
            avlRoot = insert(avlRoot, x, newNode);
            root = insertIterative(root, x, newNode);
        }
        else if(avlPlace == nullptr && avlRoot != nullptr) {
            return;
        }
        else {
            Node* place = avlPlace->link;
            if(x > place->value) {
                if(place->right == nullptr) {
                    insertIterative(place, x, newNode);
                }
                else {
                    insertIterative(successorNode(avlPlace->value)->link, x, newNode);
                }
            }
            else {
                if(place->left == nullptr) {
                    insertIterative(place, x, newNode);
                }
                else {
                    insertIterative(predecessorNode(avlPlace->value)->link, x, newNode);
                }
            }
            avlRoot = insert(avlRoot, x, newNode);
        }
    }

    /**
     * Removes element from the tree
     *
     * @param x element to be removed
     */
    void remove(int x) {
        AVLNode* toRemoveAvl = find(avlRoot, x);
        Node* toRemove = nullptr;
        if(toRemoveAvl == nullptr) {
            return;
        }
        toRemove = toRemoveAvl->link;

        if(toRemove->parent == nullptr) {
            root = remove(toRemove, x);
        }
        else {
            if(toRemove->parent->left == toRemove) {
                Node* toRemoveParent = toRemove->parent;
                toRemoveParent->left = remove(toRemove, x);
                if(toRemoveParent->left != nullptr) {
                    toRemoveParent->left->parent = toRemoveParent;
                }
            }
            else {
                Node* toRemoveParent = toRemove->parent;
                toRemoveParent->right = remove(toRemove, x);
                if(toRemoveParent->right != nullptr) {
                    toRemoveParent->right->parent = toRemoveParent;
                }
            }

        }

        avlRoot = remove(avlRoot, x);
    }

    /**
     * Returns parent for the given element
     *
     * @param x element to find it's parent
     * @return parent's value
     */
    int parent(int x) {
        AVLNode* avlV = find(avlRoot, x);
        if(avlV == nullptr) {
            return -1;
        }
        Node* v = avlV->link;

        if(v == nullptr) {
            return -1;
        }
        if(v->parent == nullptr) {
            return 0;
        }
        return v->parent->value;
    }

    /**
     * Returns successor for the given element
     *
     * @param x element to find it's successor
     * @return successor's value
     */
    int successor(int x) {
        AVLNode* v = find(avlRoot, x);

        if(v == nullptr) {
            return -1;
        }
        if(v == max(avlRoot)) {
            return 0;
        }

        if(v->right != nullptr) {
            return min(v->right)->value;
        }
        AVLNode* parent = v->parent;
        while(parent != nullptr && v == parent->right) {
            v = parent;
            parent = parent->parent;
        }

        return parent->value;
    }

    /**
     * Predecessor AVL node
     */
    int predecessor(int x) {
        AVLNode* v = find(avlRoot, x);

        if(v == nullptr) {
            return -1;
        }
        if(v == min(avlRoot)) {
            return 0;
        }

        if(v->left != nullptr) {
            return max(v->left)->value;
        }
        AVLNode* parent = v->parent;
        while(parent != nullptr && v == parent->left) {
            v = parent;
            parent = parent->parent;
        }

        return parent->value;
    }

    /**
     * Successor AVL node
     */
    AVLNode* successorNode(int x) {
        AVLNode* v = find(avlRoot, x);

        if(v == nullptr) {
            return nullptr;
        }
        if(v == max(avlRoot)) {
            return nullptr;
        }

        if(v->right != nullptr) {
            return min(v->right);
        }
        AVLNode* parent = v->parent;
        while(parent != nullptr && v == parent->right) {
            v = parent;
            parent = parent->parent;
        }

        return parent;
    }

    /**
     * Predecessor AVL node
     */
    AVLNode* predecessorNode(int x) {
        AVLNode* v = find(avlRoot, x);

        if(v == nullptr) {
            return nullptr;
        }
        if(v == min(avlRoot)) {
            return nullptr;
        }

        if(v->left != nullptr) {
            return max(v->left);
        }
        AVLNode* parent = v->parent;
        while(parent != nullptr && v == parent->left) {
            v = parent;
            parent = parent->parent;
        }

        return parent;
    }

    /**
     * Performs a rotation at the given element
     *
     * @param x element at which the rotation is performed
     * @param type rotation type (LEFT/RIGHT)
     * @return status (1/0/-1)
     */
    int rotate(int x, RotationType type) {
        AVLNode* avlV = find(avlRoot, x);
        if(avlV == nullptr) {
            return -1;
        }
        Node* v = avlV->link;

        if(v == nullptr) {
            return -1;
        }

        if(type == RotationType::LEFT) {
            if(v->right == nullptr) {
                return 0;
            }
            rotateLeft(v);
        }

        if(type == RotationType::RIGHT) {
            if(v->left == nullptr) {
                return 0;
            }
            rotateRight(v);
        }

        return 1;
    }

private:

    AVLNode* findAVLInsertPlace(AVLNode* v, int x) {
        if (v == nullptr) {
            return nullptr;
        }

        while(true) {
            if (x > v->value) {
                if (v->right == nullptr) {
                    return v;
                }
                v = v->right;
            }
            else if (x < v->value) {
                if (v->left == nullptr) {
                    return v;
                }
                v = v->left;
            }
            else {
                return nullptr;
            }
        }
    }

    /**
     * AVL find
     */
    AVLNode* find(AVLNode* v, int x) {
        if(v == nullptr) {
            return nullptr;
        }
        else if(x == v->value) {
            return v;
        }
        else if(x < v->value) {
            return find(v->left, x);
        }
        else {
            return find(v->right, x);
        }
    }

    /**
     * Finds a node in the tree
     */
    Node* find(Node* v, int x) {
        if(v == nullptr) {
            return nullptr;
        }
        else if(x == v->value) {
            return v;
        }
        else if(x < v->value) {
            return find(v->left, x);
        }
        else {
            return find(v->right, x);
        }
    }

    /**
     * Returns node with the lowest value
     */
    Node* min(Node* v) {
        if(v == nullptr) {
            return nullptr;
        }
        if(v->left == nullptr) {
            return v;
        }
        return min(v->left);
    }

    /**
     * Returns node with the highest value
     */
    Node* max(Node* v) {
        if(v == nullptr) {
            return nullptr;
        }
        if(v->right == nullptr) {
            return v;
        }
        return max(v->right);
    }

    /**
     * Iterative insert
     */
    Node* insertIterative(Node* v, int x, Node* newNode) {
        avlRoot = insert(avlRoot, x, newNode);
        Node* start = v;
        Node* trail = nullptr;

        while(start != nullptr) {
            trail = start;
            if(x < start->value) {
                start = start->left;
            }
            else {
                start = start->right;
            }
        }

        if(trail == nullptr) {
            return newNode;
        }
        else if(x < trail->value) {
            trail->left = newNode;
            newNode->parent = trail;
        }
        else {
            trail->right = newNode;
            newNode->parent = trail;
        }

        return v;
    }

    /**
     * Recursive variation of BST removal
     */
    Node* remove(Node* v, int x) {
        if(v == nullptr) {
            return nullptr;
        }
        if(x < v->value) {
            v->left = remove(v->left, x);
        }
        else if(x > v->value) {
            v->right = remove(v->right, x);
        }
        else if (x == v->value) {
            if(v->left == nullptr && v->right == nullptr) {
                return nullptr;
            }
            if(v->left == nullptr) {
                v->right->parent = v->parent;
                return v->right;
            }
            if(v->right == nullptr) {
                v->left->parent = v->parent;
                return v->left;
            }
            AVLNode* avlSuccessor = successorNode(v->value);
            Node* w = avlSuccessor->link;
            v->value = w->value;
            avlSuccessor->link = v;
            v->right = remove(v->right, w->value);
        }
        return v;
    }

    /**
     *AVL insert
     */
    AVLNode* insert(AVLNode* v, int x, Node* link) {
        if(v == nullptr) {
            return new AVLNode(x, link);
        }

        if(x < v->value) {
            AVLNode* leftChild = insert(v->left, x, link);
            v->left = leftChild;
            leftChild->parent = v;
        }
        else if(x > v->value) {
            AVLNode* rightChild = insert(v->right, x, link);
            v->right = rightChild;
            rightChild->parent = v;
        }
        else {
            return v;
        }

        v->size = 1 + intmax(size(v->right), size(v->left));

        int b = balance(v);

        // right
        if(b > 1 && x < v->left->value) {
            return rotateRight(v);
        }
        // left
        if(b < -1 && x > v->right->value) {
            return rotateLeft(v);
        }
        // left right
        if(b > 1 && x > v->left->value) {
            v->left =  rotateLeft(v->left);
            return rotateRight(v);
        }
        // right left
        if(b < -1 && x < v->right->value) {
            v->right = rotateRight(v->right);
            return rotateLeft(v);
        }

        return v;
    }

    /**
     * AVL remove
     */
    AVLNode* remove(AVLNode* v, int x) {
        if(v == nullptr) {
            return nullptr;
        }
        if(x < v->value) {
            v->left = remove(v->left, x);
        }
        else if(x > v->value) {
            v->right = remove(v->right, x);
        }
        else if (x == v->value) {
            if(v->left == nullptr && v->right == nullptr) {
                return nullptr;
            }
            if(v->left == nullptr) {
                v->right->parent = v->parent;
                return v->right;
            }
            if(v->right == nullptr) {
                v->left->parent = v->parent;
                return v->left;
            }
            AVLNode* w = min(v->right);
            v->value = w->value;
            v->link = w->link;
            v->right = remove(v->right, w->value);
        }

        v->size = 1 + intmax(size(v->right), size(v->left));

        int b = balance(v);

        // right
        if(b > 1 && balance(v->left) >= 0) {
            return rotateRight(v);
        }
        // left right
        if(b > 1 && balance(v->left) < 0) {
            v->left = rotateLeft(v->left);
            return rotateRight(v);
        }
        // left
        if(b < -1 && balance(v->right) <= 0) {
            return rotateLeft(v);
        }
        // right left
        if(b < -1 && balance(v->right) > 0) {
            v->right = rotateRight(v->right);
            return rotateLeft(v);
        }

        return v;
    }

    AVLNode* min(AVLNode* v) {
        if(v == nullptr) {
            return nullptr;
        }
        if(v->left == nullptr) {
            return v;
        }
        return min(v->left);
    }

    AVLNode* max(AVLNode* v) {
        if(v == nullptr) {
            return nullptr;
        }
        if(v->right == nullptr) {
            return v;
        }
        return max(v->right);
    }

    /**
     * Rotates left at vertex v
     */
    Node* rotateLeft(Node* v) {
        Node* pivot = v->right;

        if(v == root) {
            root = pivot;
        }

        if(v->parent != nullptr) {
            if(v == v->parent->left) {
                v->parent->left = pivot;
            }
            if(v == v->parent->right) {
                v->parent->right = pivot;
            }
        }

        pivot->parent = v->parent;
        v->parent = pivot;

        v->right = pivot->left;
        if(v->right != nullptr) {
            v->right->parent = v;
        }
        pivot->left = v;
        return pivot;
    }

    /**
     * Rotates right at vertex v
     */
    Node* rotateRight(Node* v) {
        Node* pivot = v->left;

        if(v == root) {
            root = pivot;
        }

        if(v->parent != nullptr) {
            if(v == v->parent->left) {
                v->parent->left = pivot;
            }
            if(v == v->parent->right) {
                v->parent->right = pivot;
            }
        }

        pivot->parent = v->parent;
        v->parent = pivot;

        v->left = pivot->right;
        if(v->left != nullptr) {
            v->left->parent = v;
        }
        pivot->right = v;
        return pivot;
    }

    int balance(AVLNode* v) {
        if(v == nullptr || (v->left == nullptr && v->right == nullptr)) {
            return 0;
        }
        if(v->left == nullptr) {
            return -v->right->size;
        }
        if(v->right == nullptr) {
            return v->left->size;
        }
        return v->left->size - v->right->size;
    }

    int size(AVLNode* v) {
        if(v == nullptr) {
            return 0;
        }
        return v->size;
    }

    AVLNode* rotateLeft(AVLNode* v) {
        AVLNode* pivot = v->right;

        if(v == avlRoot) {
            avlRoot = pivot;
        }

        if(v->parent != nullptr) {
            if(v == v->parent->left) {
                v->parent->left = pivot;
            }
            if(v == v->parent->right) {
                v->parent->right = pivot;
            }
        }

        pivot->parent = v->parent;
        v->parent = pivot;

        v->right = pivot->left;
        if(v->right != nullptr) {
            v->right->parent = v;
        }
        pivot->left = v;

        v->size = 1 + intmax(size(v->left), size(v->right));
        pivot->size = 1 + intmax(size(pivot->left), size(pivot->right));

        return pivot;
    }

    AVLNode* rotateRight(AVLNode* v) {
        AVLNode* pivot = v->left;

        if(v == avlRoot) {
            avlRoot = pivot;
        }

        if(v->parent != nullptr) {
            if(v == v->parent->left) {
                v->parent->left = pivot;
            }
            if(v == v->parent->right) {
                v->parent->right = pivot;
            }
        }

        pivot->parent = v->parent;
        v->parent = pivot;

        v->left = pivot->right;
        if(v->left != nullptr) {
            v->left->parent = v;
        }
        pivot->right = v;

        v->size = 1 + intmax(size(v->left), size(v->right));
        pivot->size = 1 + intmax(size(pivot->left), size(pivot->right));

        return pivot;
    }

    Node* root;
    AVLNode* avlRoot;

};

int main() {

    Tree tree;

    while(true) {
        int operation, x, y;
        cin >> operation;

        if(operation == 6) {
            break;
        }

        cin >> x;

        if(operation == 1) {
            tree.insert(x);
        }

        if(operation == 2) {
            tree.remove(x);
        }

        if(operation == 3) {
            int parent = tree.parent(x);

            if(parent == -1) {
                cout << "notfound" << endl;
            } else if(parent == 0) {
                cout << "noparent" << endl;
            } else {
                cout << parent << endl;
            }
        }

        if(operation == 4) {
            int successor = tree.successor(x);

            if(successor == -1) {
                cout << "notfound" << endl;
            } else if(successor == 0) {
                cout << "nosuccessor" << endl;
            } else {
                cout << successor << endl;
            }
        }

        if(operation == 5) {
            cin >> y;
            int result = tree.rotate(x, y == 1 ? RotationType::LEFT : RotationType::RIGHT);

            if (result == -1) {
                cout << "notfound" << endl;
            } else if (result == 0) {
                cout << "norotate" << endl;
            }
        }
    }

    return 0;

}