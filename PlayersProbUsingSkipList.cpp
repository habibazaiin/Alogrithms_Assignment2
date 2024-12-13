#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
using namespace std;

struct Node
{
    int playerId;
    int score;
    vector<Node *> forward; // Forward pointers at different levels

    Node(int id, int sc, int level) : playerId(id), score(sc), forward(level + 1, nullptr) {}
};

// Skip List class
class SkipList
{
    int maxLevel;
    float probability;
    Node *header;
    int level;

    unordered_map<int, Node *> playerMap; // Map to track players by ID

public:
    SkipList(int maxLvl, float prob) : maxLevel(maxLvl), probability(prob), level(0)
    {
        header = new Node(-1, -1, maxLevel); // create header node with no ID or score
    }

    ~SkipList()
    {
        Node *current = header;
        while (current)
        {
            Node *temp = current;
            current = current->forward[0];
            delete temp;
        }
    }

    int randomLevel()
    {
        int lvl = 0;
        while ((rand() / double(RAND_MAX)) < probability && lvl < maxLevel)
        {
            lvl++;
        }
        return lvl;
    }

    void addPlayer(int playerId, int initialScore)
    {
        if (playerMap.find(playerId) != playerMap.end())
        {
            cout << "Player with ID " << playerId << " already exists!" << endl;
            return;
        }

        vector<Node *> update(maxLevel + 1, nullptr);
        Node *current = header;

        // Find the correct position to insert the new player by descending order
        for (int i = level; i >= 0; i--)
        {
            while (current->forward[i] && current->forward[i]->score > initialScore)
            {
                current = current->forward[i];
            }
            update[i] = current;
        }

        int randomLvl = randomLevel();
        if (randomLvl > level)
        {
            for (int i = level + 1; i <= randomLvl; i++)
            {
                update[i] = header; // Point the header node to new level
            }
            level = randomLvl; // Increase the list's level
        }

        Node *newNode = new Node(playerId, initialScore, randomLvl);
        for (int i = 0; i <= randomLvl; i++)
        {
            newNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = newNode;
        }

        playerMap[playerId] = newNode; // Add player to map
    }

    void removePlayer(int playerId)
    {
        if (playerMap.find(playerId) == playerMap.end())
        {
            cout << "Player with ID " << playerId << " does not exist!" << endl;
            return;
        }

        vector<Node *> update(maxLevel + 1, nullptr);
        Node *current = header;

        // Traverse the list to find the node to remove
        for (int i = level; i >= 0; i--)
        {
            while (current->forward[i] && current->forward[i]->playerId != playerId)
            {
                current = current->forward[i];
            }
            update[i] = current;
        }

        Node *targetNode = playerMap[playerId];
        for (int i = 0; i <= level; i++)
        {
            if (update[i]->forward[i] == targetNode)
            {
                update[i]->forward[i] = targetNode->forward[i]; // Remove node from forward pointers
            }
        }

        delete targetNode;
        playerMap.erase(playerId);

        // Reduce the level of the skip list if necessary
        while (level > 0 && header->forward[level] == nullptr)
        {
            level--;
        }
    }

    void updateScore(int playerId, int newScore)
    {
        if (playerMap.find(playerId) == playerMap.end())
        {
            cout << "Player with ID " << playerId << " does not exist!" << endl;
            return;
        }

        int oldScore = playerMap[playerId]->score;
        removePlayer(playerId);
        addPlayer(playerId, newScore);

        cout << "Updated player " << playerId << "'s score from " << oldScore << " to " << newScore << endl;
    }

    void displayLeaderboard(int topN)
    {
        cout << "Top " << topN << " players:" << endl;

        Node *current = header->forward[0]; // Start from the head of the list

        int count = 0;
        while (current && count < topN)
        {
            cout << "ID: " << current->playerId << " (Score: " << current->score << ")" << endl;
            current = current->forward[0];
            count++;
        }
    }

    void displayPlayerScore(int playerId)
    {
        if (playerMap.find(playerId) != playerMap.end())
        {
            cout << "Player ID " << playerId << "'s score: " << playerMap[playerId]->score << endl;
        }
        else
        {
            cout << "Player with ID " << playerId << " does not exist!" << endl;
        }
    }

    void printPlayersScores()
    {
        for (int i = level; i >= 0; i--)
        {
            Node *current = header->forward[i];
            cout << "Level " << i << ": " << endl;
            while (current)
            {
                cout << current->playerId << ": " << current->score << " ";
                current = current->forward[i];
            }
            cout << "\n";
        }
    }
};

int main()
{
    srand(time(0));

    SkipList leaderboard(5, 0.5);

    leaderboard.addPlayer(1, 50);
    leaderboard.addPlayer(2, 70);
    leaderboard.addPlayer(3, 30);

    // o retrieve the top N
    leaderboard.displayLeaderboard(2);
    // updateScore
    leaderboard.updateScore(1, 10);
    // updateScore at any time
    leaderboard.updateScore(2, 10);
    // view their score at any time
    leaderboard.displayPlayerScore(2);
    // player exits at any time
    leaderboard.removePlayer(3);
    // player can join at any time
    leaderboard.addPlayer(4, 190);
    // o retrieve the top N
    leaderboard.displayLeaderboard(3);
    leaderboard.printPlayersScores();

    return 0;
}
