#include <iostream>
#include <queue>
#include <limits>
using namespace std;

enum NodeColor { RED, BLACK };

struct Node {
    int key;
    NodeColor color;
    Node* left, * right, * parent;

    Node(int key) {
        this->key = key;
        left = right = parent = nullptr;
        this->color = RED;
    }
};

class RBTree {
private:
    Node* root;


    Node* insertToBST(Node* root, Node* newNode) {
        if (root == nullptr)
            return newNode;

        if (newNode->key < root->key) {
            root->left = insertToBST(root->left, newNode);
            if (root->left != nullptr) root->left->parent = root;
        } else if (newNode->key > root->key) {
            root->right = insertToBST(root->right, newNode);
            if (root->right != nullptr) root->right->parent = root;
        }

        return root;
    }


    void rotateLeft(Node*& root, Node*& node) {
        Node* rightChild = node->right;

        node->right = rightChild->left;
        if (node->right != nullptr)
            node->right->parent = node;

        rightChild->parent = node->parent;
        if (node->parent == nullptr)
            root = rightChild;
        else if (node == node->parent->left)
            node->parent->left = rightChild;
        else
            node->parent->right = rightChild;

        rightChild->left = node;
        node->parent = rightChild;
    }

    void rotateRight(Node*& root, Node*& node) {
        Node* leftChild = node->left;

        node->left = leftChild->right;
        if (node->left != nullptr)
            node->left->parent = node;

        leftChild->parent = node->parent;
        if (node->parent == nullptr)
            root = leftChild;
        else if (node == node->parent->left)
            node->parent->left = leftChild;
        else
            node->parent->right = leftChild;

        leftChild->right = node;
        node->parent = leftChild;
    }


    void resolveInsert(Node*& root, Node*& node) {
        Node* parent = nullptr;
        Node* grandParent = nullptr;

        while ((node != root) && (node->color == RED) && (node->parent->color == RED)) {
            parent = node->parent;
            grandParent = node->parent->parent;

            if (parent == grandParent->left) {
                Node* uncle = grandParent->right;

                if (uncle != nullptr && uncle->color == RED) {
                    grandParent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = grandParent;
                } else {
                    if (node == parent->right) {
                        rotateLeft(root, parent);
                        node = parent;
                        parent = node->parent;
                    }

                    rotateRight(root, grandParent);
                    swap(parent->color, grandParent->color);
                    node = parent;
                }
            } else {
                Node* uncle = grandParent->left;

                if (uncle != nullptr && uncle->color == RED) {
                    grandParent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = grandParent;
                } else {
                    if (node == parent->left) {
                        rotateRight(root, parent);
                        node = parent;
                        parent = node->parent;
                    }

                    rotateLeft(root, grandParent);
                    swap(parent->color, grandParent->color);
                    node = parent;
                }
            }
        }

        root->color = BLACK;
    }

    // Fix Red-Black Tree violations after deletion
    void resolveDelete(Node*& root, Node* node, Node* parentNode) {
        while (node != root && (node == nullptr || node->color == BLACK)) {
            if (node == parentNode->left) {
                Node* sibling = parentNode->right;

                // Case 1: Sibling is red
                if (sibling->color == RED) {
                    sibling->color = BLACK;
                    parentNode->color = RED;
                    rotateLeft(root, parentNode);
                    sibling = parentNode->right;
                }

                // Case 2: Sibling's children are black
                if ((sibling->left == nullptr || sibling->left->color == BLACK) &&
                    (sibling->right == nullptr || sibling->right->color == BLACK)) {
                    sibling->color = RED;
                    node = parentNode;
                    parentNode = node->parent;
                } else {
                    // Case 3: Sibling's right child is black
                    if (sibling->right == nullptr || sibling->right->color == BLACK) {
                        if (sibling->left != nullptr)
                            sibling->left->color = BLACK;
                        sibling->color = RED;
                        rotateRight(root, sibling);
                        sibling = parentNode->right;
                    }

                    // Case 4: Sibling's right child is red
                    sibling->color = parentNode->color;
                    parentNode->color = BLACK;
                    if (sibling->right != nullptr)
                        sibling->right->color = BLACK;
                    rotateLeft(root, parentNode);
                    node = root;
                    break;
                }
            } else {
                // Mirror cases for right child
                Node* sibling = parentNode->left;

                // Case 1: Sibling is red
                if (sibling->color == RED) {
                    sibling->color = BLACK;
                    parentNode->color = RED;
                    rotateRight(root, parentNode);
                    sibling = parentNode->left;
                }

                // Case 2: Sibling's children are black
                if ((sibling->right == nullptr || sibling->right->color == BLACK) &&
                    (sibling->left == nullptr || sibling->left->color == BLACK)) {
                    sibling->color = RED;
                    node = parentNode;
                    parentNode = node->parent;
                } else {
                    // Case 3: Sibling's left child is black
                    if (sibling->left == nullptr || sibling->left->color == BLACK) {
                        if (sibling->right != nullptr)
                            sibling->right->color = BLACK;
                        sibling->color = RED;
                        rotateLeft(root, sibling);
                        sibling = parentNode->left;
                    }

                    // Case 4: Sibling's left child is red
                    sibling->color = parentNode->color;
                    parentNode->color = BLACK;
                    if (sibling->left != nullptr)
                        sibling->left->color = BLACK;
                    rotateRight(root, parentNode);
                    node = root;
                    break;
                }
            }
        }

        if (node != nullptr)
            node->color = BLACK;
    }


    Node* findMinNode(Node* node) {
        Node* current = node;
        while (current && current->left != nullptr)
            current = current->left;
        return current;
    }


    void inorderPrint(Node* root) {
        if (root == nullptr)
            return;

        inorderPrint(root->left);
        cout << "Node: " << root->key << ", Color: " << (root->color == RED ? "RED" : "BLACK") << endl;
        inorderPrint(root->right);
    }


    Node* deleteNode(Node* root, int key) {
        if (root == nullptr) return root;

        if (key < root->key)
            root->left = deleteNode(root->left, key);
        else if (key > root->key)
            root->right = deleteNode(root->right, key);
        else {
            Node* replacement = nullptr;
            NodeColor originalColor;

            if (root->left == nullptr || root->right == nullptr) {
                Node* child = (root->left != nullptr) ? root->left : root->right;
                originalColor = root->color;

                if (child == nullptr) {
                    replacement = root;
                    root = nullptr;
                } else {
                    *root = *child;
                    replacement = root;
                    delete child;
                }

                if (originalColor == BLACK) {
                    resolveDelete(this->root, replacement, replacement->parent);
                }
            } else {
                Node* successor = findMinNode(root->right);
                root->key = successor->key;
                originalColor = successor->color;

                root->right = deleteNode(root->right, successor->key);

                if (originalColor == BLACK) {
                    Node* replacementChild = (root->right != nullptr) ? root->right : nullptr;
                    resolveDelete(this->root, replacementChild, root);
                }
            }
        }

        return root;
    }


    void displayTree(Node* root, string indent = "", bool last = true) {
        if (root != nullptr) {
            cout << indent;
            if (last) {
                cout << "R----";
                indent += "   ";
            } else {
                cout << "L----";
                indent += "|  ";
            }

            cout << root->key << "(" << (root->color == RED ? "RED" : "BLACK") << ")" << endl;

            displayTree(root->left, indent, false);
            displayTree(root->right, indent, true);
        }
    }

public:
    RBTree() { root = nullptr; }

    bool find(Node* root, int key) {
        if (root == nullptr)
            return false;

        if (key == root->key)
            return true;

        if (key < root->key)
            return find(root->left, key);
        else
            return find(root->right, key);
    }

    void add(int key) {
        cout << "Adding node with key: " << key << endl;

        if (find(root, key)) {
            cout << "Duplicate key detected! Node with key " << key << " already exists in the tree." << endl;
            return;
        }

        Node* newNode = new Node(key);
        root = insertToBST(root, newNode);
        resolveInsert(root, newNode);
        cout << "After balancing:\n";
        displayTree(root);
    }

    void Delete(int key) {
        if (root == nullptr) {
            cout << "Tree is empty. Nothing to delete." << endl;
            return;
        }
        root = deleteNode(root, key);
        cout << "After deleting " << key << ":\n";
        displayTree(root);
    }

    void inorder() {
        if (root == nullptr) {
            cout << "Tree is empty. Nothing to traverse." << endl;
            return;
        }
        inorderPrint(root);
    }
};


int getValidInput() {
    int key;
    while (true) {
        if (cin >> key)
            return key;
        else {
            cout << "Invalid input! Please enter a valid integer: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

int main() {
    RBTree tree;
    int choice, key;

    do {
        cout << "\nRB Tree Operations Menu\n";
        cout << "1. Add Node\n";
        cout << "2. Delete Node\n";
        cout << "3. Display Inorder Traversal\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        choice = getValidInput();

        switch (choice) {
            case 1:
                cout << "Enter the key to add: ";
                key = getValidInput();
                tree.add(key);
                break;
            case 2:
                cout << "Enter the key to delete: ";
                key = getValidInput();
                tree.Delete(key);
                break;
            case 3:
                cout << "Displaying Inorder Traversal:\n";
                tree.inorder();
                break;
            case 4:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 4);

    return 0;
}
