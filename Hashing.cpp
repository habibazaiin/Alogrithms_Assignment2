#include <bits/stdc++.h>

using namespace std;

int TABLE_SIZE;


int getTableSize() {
    cout << "Enter table size: ";
    cin >> TABLE_SIZE;
}


int divisionHash(int key) {
    return key % TABLE_SIZE;
}


int multiplicationHash(int key) {
    const double A = 0.618033;
    double temp = key * A;
    double fractionalPart = temp - floor(temp);
    return (int)(TABLE_SIZE * fractionalPart);
}


int midSquareHash(int key) {
    long long squared = (long long)key * key;
    string squaredStr = to_string(squared);
    int r = to_string(TABLE_SIZE).length();
    int mid = (squaredStr.length() - r) / 2;
    string midDigits = squaredStr.substr(mid, r);
    int hashValue = stoi(midDigits) % TABLE_SIZE;
    return hashValue;
}


int foldingHash(int key, bool useFoldBoundary) {
    string keyStr = to_string(key);
    long long sum = 0;
    int partSize = to_string(TABLE_SIZE - 1).length();
    vector<string> parts;
    for (size_t i = 0; i < keyStr.length(); i += partSize) {
        int s = keyStr.length() - i;
        parts.push_back(keyStr.substr(i,  min(partSize, s)));
    }

    if (useFoldBoundary) {
        if (!parts.empty()) {
            reverse(parts.front().begin(), parts.front().end());
            reverse(parts.back().begin(), parts.back().end());
        }
    }

    for (const string &part : parts) {
        sum += stoi(part);
    }

    return sum % TABLE_SIZE;
}


vector<vector<int>> hashTable;

void insertChainingByDivisionMethod(int key) {
    int index = divisionHash(key);
    hashTable[index].push_back(key);
}

void insertChainingBymultiplicationMethod(int key) {
    int index = multiplicationHash(key);
    hashTable[index].push_back(key);
}

void insertChainingBymidSquareMethod(int key) {
    int index = midSquareHash(key);
    hashTable[index].push_back(key);
}

void insertChainingByfoldingMethod(int key) {
    int index = foldingHash(key, false);
    hashTable[index].push_back(key);
}

void insertChainingByfoldingUseFoldBoundaryMethod(int key) {
    int index = foldingHash(key, true);
    hashTable[index].push_back(key);
}


void displayChaining() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        cout << i << ": ";
        if (hashTable[i].empty()) {
            cout << "NULL";
        } else {
            for (int key: hashTable[i]) {
                cout << key << " -> ";
            }
            cout << "NULL";
        }
        cout << endl;

    }
}


vector<int> openAddressingHashTable;
vector<bool> isOccupied;

void LinearProbing(int key, int value) {
    int index = key;
    int originalIndex = index;
    while (isOccupied[index]) {
        index = (index + 1) % TABLE_SIZE;
        if (index == originalIndex) {
            cout << "Table is full!" << endl;
            return;
        }
    }
    openAddressingHashTable[index] = value;
    isOccupied[index] = true;
}

void QuadraticProbing(int key, int value) {
    int index = key;
    int originalIndex = index;
    int i = 1;
    while (isOccupied[index]) {
        index = (originalIndex + i * i) % TABLE_SIZE;
        if (index == originalIndex) {
            cout << "Table is full!" << endl;
            return;
        }
        i++;
    }

    openAddressingHashTable[index] = value;
    isOccupied[index] = true;
}

void displayOpenAddressing() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        if (isOccupied[i]) {
            cout << i << ": " << openAddressingHashTable[i] << endl;
        } else {
            cout << i << ": " << "NULL" << endl;
        }
    }
}


bool isPrime(int num) {
    if (num <= 1) return false;
    for (int i = 2; i <= sqrt(num); ++i) {
        if (num % i == 0) return false;
    }
    return true;
}


int largestPrimeLessThan(int n) {
    for (int i = n - 1; i >= 2; --i) {
        if (isPrime(i)) {
            return i;
        }
    }
    return -1;
}


int doubleHash(int key, int i) {
    int h1 = key;
    int n = largestPrimeLessThan(TABLE_SIZE);
    int h2 = n - (key % n);
    return (h1 + i * h2) % TABLE_SIZE;
}

void insertDoubleHashing(int key, int value) {
    int i = 0;
    int index;
    while (i < TABLE_SIZE) {
        index = doubleHash(key, i);
        if (!isOccupied[index]) {
            openAddressingHashTable[index] = value;
            isOccupied[index] = true;
            return;
        }
        ++i;
    }
    cout << "Table is full!" << endl;
}

void displayDoubleHashing() {
    displayOpenAddressing();
}


int selectHashingMethod() {
    int choice;
    cout << "\n--- Select Hashing Technique ---\n";
    cout << "1. Division Method Hashing" << endl;
    cout << "2. Multiplication Method Hashing" << endl;
    cout << "3. Mid Square Method Hashing" << endl;
    cout << "4. Fold-Shifting Method Hashing" << endl;
    cout << "5. Fold-Boundary Method Hashing" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    return choice;
}


int selectCollisionResolution() {
    int choice;
    cout << "\n--- Select Collision Resolution Method ---\n";
    cout << "1. Chaining Method" << endl;
    cout << "2. Open Addressing Method (Linear Probing)" << endl;
    cout << "3. Open Addressing Method (Quadratic Probing)" << endl;
    cout << "4. Double Hashing Method" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    return choice;
}

int main() {
    int hashChoice, collisionChoice, key;
    string flag;

    getTableSize();


    hashTable.clear();
    hashTable.resize(TABLE_SIZE);
    openAddressingHashTable.clear();
    openAddressingHashTable.resize(TABLE_SIZE, -1);
    isOccupied.clear();
    isOccupied.resize(TABLE_SIZE, false);

    do {

        hashChoice = selectHashingMethod();


        collisionChoice = selectCollisionResolution();
        int hashidx;

        switch (hashChoice) {
            case 1:
                cout << "Enter key to insert: ";
                cin >> key;
                hashidx = divisionHash(key);
                insertChainingByDivisionMethod(key);
                break;
            case 2:
                cout << "Enter key to insert: ";
                cin >> key;
                hashidx = multiplicationHash(key);
                insertChainingBymultiplicationMethod(key);
                break;
            case 3:
                cout << "Enter key to insert: ";
                cin >> key;
                hashidx = midSquareHash(key);
                insertChainingBymidSquareMethod(key);
                break;
            case 4:
                cout << "Enter key to insert: ";
                cin >> key;
                hashidx = foldingHash(key, false);
                insertChainingByfoldingMethod(key);
                break;
            case 5:
                cout << "Enter key to insert: ";
                cin >> key;
                hashidx = foldingHash(key, true);
                insertChainingByfoldingUseFoldBoundaryMethod(key);
                break;
            default:
                cout << "Invalid choice for hashing method.\n";
                continue;
        }
        switch (collisionChoice) {
            case 1:
                displayChaining();
                break;
            case 2:
                LinearProbing(hashidx, key);
                displayOpenAddressing();
                break;
            case 3:
                QuadraticProbing(hashidx, key);
                displayOpenAddressing();
                break;
            case 4:
                insertDoubleHashing(hashidx, key);
                displayDoubleHashing();
                break;
            default:
                cout << "Invalid collision resolution method.\n";
                continue;
        }

        cout << "Enter Yes if you want to continue inserting: ";
        cin >> flag;
    } while (flag == "Yes");

    return 0;
}
