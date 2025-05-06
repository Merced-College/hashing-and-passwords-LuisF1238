// Jose Flores
// CPSC 25

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

// Improved hash function to reduce collisions
int ImprovedHashString(const string& value) {
    const int prime = 26;  // Prime number for better distribution
    unsigned long hashCode = 0;

    for (char character : value) {
        hashCode = (hashCode * prime + character) & 0x7fffffff; // Keeps hash positive
    }

    return static_cast<int>(hashCode);
}

int main() {
    unordered_map<int, vector<string> > passwordTable; 
    ifstream passwordFile("10-million-password-list-top-10000.txt"); 

    if (!passwordFile) {
        cerr << "Error: Password file not found!" << endl;
        return 1;
    }

    string password;
    int totalPasswords = 0;
    int totalCollisions = 0;

    cout << "Reading passwords and hashing...\n";
    while (getline(passwordFile, password)) {
        int hashValue = ImprovedHashString(password);

        // Detect collisions
        if (passwordTable.find(hashValue) != passwordTable.end()) {
            totalCollisions++;
        }

        passwordTable[hashValue].push_back(password);
        totalPasswords++;
    }
    passwordFile.close();

    // Print Total Collisions
    cout << "Hashing complete.\nTotal passwords: " << totalPasswords << endl;
    cout << "Total collisions: " << totalCollisions << endl;

    // Print first 10 passwords and their hashes
    cout << "\nFirst 10 password hashes for verification:\n";
    int count = 0;
    for (const auto& entry : passwordTable) {
        for (const string& pass : entry.second) {
            cout << pass << " -> " << entry.first << endl;
            count++;
            if (count == 10) break;
        }
        if (count == 10) break;
    }

    // Error handling for invalid input
    int userHash;
    while (true) {
        cout << "\nEnter hashed value (or -1 to exit): ";
        cin >> userHash;

        if (userHash == -1) break;

        if (passwordTable.find(userHash) != passwordTable.end()) {
            cout << "Possible original passwords:\n";
            for (const string& pass : passwordTable[userHash]) {
                cout << " - " << pass << endl;
            }
        } else {
            cout << "Password not found in database." << endl;
        }
    }

    cout << "Done" << endl;
    return 0;
}
