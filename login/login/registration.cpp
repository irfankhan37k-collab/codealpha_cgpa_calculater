#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>
#include <iomanip>

using namespace std;

// --- Global Constants ---
// Use a simple text file to simulate a database for storing user credentials
const string DB_FILENAME = "user_credentials.txt";
const string DELIMITER = ":";

/**
 * Loads all existing credentials from the file into a map for fast lookup.
 * @param usersMap Reference to a map where keys are usernames and values are passwords.
 * @return true if loading was successful, false otherwise.
 */
bool loadCredentials(map<string, string>& usersMap) {
    usersMap.clear();
    ifstream file(DB_FILENAME);
    if (!file.is_open()) {
        // If the file doesn't exist yet, it's not an error for the first run
        return true; 
    }

    string line;
    while (getline(file, line)) {
        // Find the delimiter (username:password)
        size_t pos = line.find(DELIMITER);
        if (pos != string::npos) {
            string username = line.substr(0, pos);
            string password = line.substr(pos + 1);
            usersMap[username] = password;
        }
    }
    file.close();
    return true;
}

/**
 * Saves a single new user credential to the file.
 * NOTE: For production, passwords should be hashed (e.g., using bcrypt), not stored in plaintext.
 * @param username The new username.
 * @param password The new password.
 * @return true if saving was successful.
 */
bool saveCredential(const string& username, const string& password) {
    // Open file in append mode
    ofstream file(DB_FILENAME, ios::app);
    if (!file.is_open()) {
        cerr << "Error: Could not open database file for writing.\n";
        return false;
    }

    // Write the new credential in "username:password" format
    file << username << DELIMITER << password << "\n";
    file.close();
    return true;
}

/**
 * Validates the username and password formats.
 * Checks: 
 * - Not empty
 * - No delimiter characters in the username or password (to prevent file format corruption)
 * @param input The string to validate (username or password).
 * @return true if valid, false otherwise.
 */
bool validateInput(const string& input) {
    if (input.empty()) {
        return false;
    }
    // Ensure the input doesn't contain the delimiter character
    if (input.find(DELIMITER) != string::npos) {
        return false;
    }
    return true;
}

// --- Registration Function ---
void registerUser() {
    string username, password;
    map<string, string> usersMap;
    loadCredentials(usersMap); // Load existing users

    cout << "\n--- New User Registration ---\n";

    // 1. Take username input
    cout << "Enter new username: ";
    cin >> username;

    // Validate username and check for duplicates
    if (!validateInput(username)) {
        cout << "Error: Username cannot be empty or contain the '" << DELIMITER << "' character.\n";
        return;
    }
    if (usersMap.count(username)) {
        cout << "Error: Username '" << username << "' is already taken. Please try another one.\n";
        return;
    }

    // 1. Take password input
    cout << "Enter new password (min 6 chars): ";
    cin >> password;

    // Validate password
    if (!validateInput(password) || password.length() < 6) {
        cout << "Error: Password must be at least 6 characters long and cannot contain the '" << DELIMITER << "' character.\n";
        return;
    }

    // 2. Store user credentials securely in a file (Step 3)
    if (saveCredential(username, password)) {
        cout << "\nRegistration successful for user: " << username << endl;
    } else {
        cout << "\nRegistration failed due to a file writing error.\n";
    }
}

// --- Login Function ---
void loginUser() {
    string username, password;
    map<string, string> usersMap;
    loadCredentials(usersMap); // Load existing users

    cout << "\n--- User Login ---\n";
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    // 3. Implement a login function that reads credentials and verifies user identity (Step 4)
    auto it = usersMap.find(username);

    if (it != usersMap.end()) {
        // User found, now check password
        if (it->second == password) {
            // Success message (Step 5)
            cout << "\nSUCCESS! Welcome back, " << username << ".\n";
            // In a real application, you would grant access/start session here.
        } else {
            // Error message (Step 5)
            cout << "\nError: Incorrect password.\n";
        }
    } else {
        // Error message (Step 5)
        cout << "\nError: Username '" << username << "' not found.\n";
    }
}

// --- Main Menu ---
int main() {
    int choice;

    cout << "------------------------------------------\n";
    cout << "  C++ Console Login & Registration System\n";
    cout << "------------------------------------------\n";
    
    // Ensure the database file can be created/read
    map<string, string> initialCheck;
    if (!loadCredentials(initialCheck)) {
        return 1; // Exit if file operation fails critically
    }
    
    do {
        cout << "\nSelect an option:\n";
        cout << "1. Register New User\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Enter choice (1-3): ";
        
        if (!(cin >> choice)) {
            cerr << "Invalid input. Please enter a number.\n";
            cin.clear(); // Clear error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard bad input
            choice = 0; // Set choice to an invalid value to repeat loop
            continue;
        }

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                loginUser();
                break;
            case 3:
                cout << "\nSystem exiting. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please enter 1, 2, or 3.\n";
                break;
        }
    } while (choice != 3);

    return 0;