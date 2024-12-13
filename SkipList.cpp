#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;

// Node structure for the skip list
struct Node {
    int value;
    vector<Node*> forward; // Forward pointers at different levels

    Node(int val, int level) : value(val), forward(level + 1, nullptr) {}
};

// Skip List class
class SkipList {
    int maxLevel;     // Maximum level of the skip list
    float probability; // Probability for increasing levels
    Node* header;     // Header node
    int level;        // Current highest level in the list

public:
    SkipList(int maxLvl, float prob) : maxLevel(maxLvl), probability(prob), level(0) {
        header = new Node(-1, maxLevel); // Initialize header node
    }

    ~SkipList() {
        delete header;
    }

    // Generate a random level for a new node
    int randomLevel() {
        int lvl = 0;
        while ((rand() / double(RAND_MAX)) < probability && lvl < maxLevel) {
            lvl++;
        }
        return lvl;
    }

    // Insert a value into the skip list
    void insert(int value) {
        vector<Node*> update(maxLevel + 1, nullptr);
        Node* current = header;

        // Find the position to insert
        for (int i = level; i >= 0; i--) {
            while (current->forward[i] && current->forward[i]->value < value) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        current = current->forward[0];

        // If the value doesn't already exist, insert it
        if (!current || current->value != value) {
            int randomLvl = randomLevel();
            if (randomLvl > level) {
                for (int i = level + 1; i <= randomLvl; i++) {
                    update[i] = header;
                }
                level = randomLvl;
            }

            Node* newNode = new Node(value, randomLvl);
            for (int i = 0; i <= randomLvl; i++) {
                newNode->forward[i] = update[i]->forward[i];
                update[i]->forward[i] = newNode;
            }
        }
    }

    // Search for a value in the skip list
    bool search(int value) {
        Node* current = header;

        // Traverse the levels to find the value
        for (int i = level; i >= 0; i--) {
            while (current->forward[i] && current->forward[i]->value < value) {
                current = current->forward[i];
            }
        }
        current = current->forward[0];
        return current && current->value == value;
    }

    // Delete a value from the skip list
    void remove(int value) {
        vector<Node*> update(maxLevel + 1, nullptr);
        Node* current = header;

        // Find the position to remove
        for (int i = level; i >= 0; i--) {
            while (current->forward[i] && current->forward[i]->value < value) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        current = current->forward[0];

        // If the value exists, remove it
        if (current && current->value == value) {
            for (int i = 0; i <= level; i++) {
                if (update[i]->forward[i] != current) break;
                update[i]->forward[i] = current->forward[i];
            }

            delete current;

            // Update the level of the skip list if the level is empty !header->forward[level]
            while (level > 0 && !header->forward[level]) {
                level--;
            }
        }
    }

    // Display the skip list
    void display() {
        for (int i = 0; i <= level; i++) {
            Node* current = header->forward[i];
            cout << "Level " << i << ": ";
            while (current) {
                cout << current->value << " ";
                current = current->forward[i];
            }
            cout << endl;
        }
    }
};

// Example usage
int main() {
    srand(time(0)); // Seed for randomness

    SkipList skipList(5, 0.5); // Max level 5, probability 0.5

    skipList.insert(3);
    skipList.insert(6);
    skipList.insert(7);
    skipList.insert(9);
    skipList.insert(12);
    skipList.insert(50);

    cout << "Skip List after insertion:" << endl;
    skipList.display();

    cout << "Search 7: " << (skipList.search(7) ? "Found" : "Not Found") << endl;
    cout << "Search 15: " << (skipList.search(15) ? "Found" : "Not Found") << endl;

    skipList.remove(7);
    cout << "Skip List after deletion of 7:" << endl;
    skipList.display();

    return 0;
}
