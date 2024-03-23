#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cctype>
#include <algorithm> // Needed for std::transform


using namespace std;

// Encrypt or decrypt text using Vigenère cipher.

string vigenereCipher(const string& text, const string& key, bool encrypt) {
    string result;
    int keyIndex = 0, keyLength = key.length();
    for (char c : text) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a'; // Determine uppercase or lowercase
            int keyShift = toupper(key[keyIndex]) - 'A'; // Ensure key is treated case-insensitively
      
            int offset = encrypt ? keyShift : -keyShift; // Adjust direction based on encrypt or decrypt
            c = static_cast<char>(((c - base + offset + 26) % 26) + base);  // Apply Vigenère cipher
            keyIndex = (keyIndex + 1) % keyLength; // Move to next key character
        }
        result += c;
    }
    return result;
}

// Remove leading and trailing spaces from a string.
string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

// Check if string has at least one digit and one special character.
bool containsNumberAndSpecialChar(const string& password) {
    bool hasNumber = false, hasSpecial = false;
    for (char c : password) {
        if (isdigit(c)) hasNumber = true;
        if (ispunct(c)) hasSpecial = true;
    }
    return hasNumber && hasSpecial;
}

// Generate a random password of a given length, ensuring it contains a number and special character.
string generatePassword(int length) {
    string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()";
    string password;
    srand(static_cast<unsigned int>(time(nullptr))); // Seed random number generator
    do {
        password.clear();
        for (size_t i = 0; i < length; ++i) { // Change 'int' to 'size_t'
            password += chars[rand() % chars.size()]; // No need to cast size() to int
        }
    } while (!containsNumberAndSpecialChar(password)); // Ensure password meets criteria
    return password;
}

// Prompt user for a password that meets specific criteria.
void getPasswordFromUser(string& password) {
    do {
        cout << "Enter your password, the following is required: (8 or more charactter, must include 1 number and 1 special character): ";
        getline(cin, password);
        password = trim(password); // Remove extra spaces
    } while (password.length() < 8 || !containsNumberAndSpecialChar(password));
}

// Get a yes or no response from the user to a question.
bool getYesOrNoResponse(const string& question) {
    string response;
    do {
        cout << question << " (yes/no): ";
        getline(cin, response);
        transform(response.begin(), response.end(), response.begin(), ::tolower); // Use of transform
        // Convert to lowercase
    } while (response != "yes" && response != "no");
    return response == "yes";
}

// Store an encrypted password for a user and platform.
void storePassword(const string& username, const string& platform, const string& password, const string& key) {
    ofstream file("passwords.txt", ios::app); // Append mode
    if (file.is_open()) {
        file << username << " " << platform << " " << vigenereCipher(password, key, true) << "\n"; // Write encrypted password
    }
    else {
        cerr << "Unable to open file to store passwords." << endl;
    }
    file.close();
}

// Retrieve and decrypt a password for a user and platform

string retrievePassword(const string& username, const string& platform, const string& key) {
    ifstream file("passwords.txt");
    string storedUsername, storedplatform, storedPassword;
    while (file >> storedUsername >> storedplatform >> storedPassword) {
        if (storedUsername == username && storedplatform == platform) {
            return vigenereCipher(storedPassword, key, false); // Decrypt password
        }
    }
    return "Platform not found! Kindly Try Again!.";
}

// Check if a username is already used.
bool isUsernameTaken(const string& username) {
    ifstream usersFile("usernames&passwords.txt");
    string storedUsername, storedPassword;
    while (usersFile >> storedUsername >> storedPassword) {
        if (storedUsername == username) { // Username is taken
            return true;
        }
    }
    return false;
}

// Register a new user by storing username and encrypted password
void registerUser(const string& username, const string& password) {
    ofstream usersFile("usernames&passwords.txt", ios::app);
    if (usersFile.is_open()) {
        usersFile << username << " " << vigenereCipher(password, "PaS28KeYyOuS3F", true) << "\n";
    }
    else {
        cerr << "Error occured while processing your request!" << endl;
    }
    usersFile.close();
}
// Handles user login or sign-up process.
bool loginUser(string& username) {
    cout << "Input Username: ";
    getline(cin, username); // Get username from user

    username = trim(username); // Trim the username

    if (isUsernameTaken(username)) { // Checks if username exists
        ifstream usersFile("usernames&passwords.txt");
        string storedUsername, storedPasswordEncrypted;
        while (usersFile >> storedUsername >> storedPasswordEncrypted) {  // Read stored usernames and passwords
            if (storedUsername == username) {
                string password;
                cout << "Input Password: ";
                getline(cin, password);
                password = trim(password); // Trim the input password

                string decryptedStoredPassword = vigenereCipher(storedPasswordEncrypted, "PaS28KeYyOuS3F", false);
                decryptedStoredPassword = trim(decryptedStoredPassword); // Trim the decrypted password

                if (decryptedStoredPassword == password) { // Check if passwords match
                    usersFile.close(); // Close the file after reading
                    cout << "Welcome back, " << username << "!" << endl;
                    return true; // Correct password
                }
                else {
                    cout << "Incorrect Password or Username, Please Try Again!" << endl;
                }
            }
        }
        usersFile.close(); // Close the file after reading
    }
    else { // New user
        cout << "First Time Here? " << endl;
        if (getYesOrNoResponse("Would you like to sign up to AllSafe Password Manager?")) {
            string password;
            getPasswordFromUser(password);
            registerUser(username, password);
            cout << "Sign up successful! Welcome, " << username << "!" << endl;
            return true;
        }
        else {
            cout << "Sorry to hear you go, Goodbye!" << endl;
            return false;
        }
    }

    return false; // Default return for login failure
}




int main() {
    string username;

    if (!loginUser(username)) { // Start by logging in user
        cout << "Exiting program." << endl;
        return 1; // Exit if login/sign up is not successful
    }

    // User logged in successfully
    cout << "Welcome to AllSafe Password Management Application, " << username << "!" << endl;
    string platform, password;
    int choice;

    // Main menu for password management
    cout << "1: Store Password\n2: Retrieve Password\nEnter choice: ";
    cin >> choice;
    cin.ignore(); // Clear buffer after input

    if (choice == 1) {
        cout << "Platform name: ";
        getline(cin, platform); // Get platform name

        bool generatePass = getYesOrNoResponse("Would you like your password to be generated (recommended)?");
        if (generatePass) {
            int passLength = 0;
            do {
                cout << "Enter password length (minimum 8 characters): ";
                cin >> passLength;
                cin.ignore(); // Clear newline after entering length
                if (passLength < 8) {
                    cout << "Password length must be at least 8 characters." << endl;
                }
            } while (passLength < 8);
            password = generatePassword(passLength);
            cout << "Generated Password: " << password << endl;
        }
        else {
            getPasswordFromUser(password);
        }

        storePassword(username, platform, password, "PaS28KeYyOuS3F");
        cout << "Password stored successfully for " << platform << "." << endl;
    }
    else if (choice == 2) {
        cout << "Platform Name: ";
        getline(cin, platform);
        password = retrievePassword(username, platform, "PaS28KeYyOuS3F");
        if (password != "Platform Not Found! Make sure it's the right one!") {
            cout << "Password for " << platform << ": " << password << endl;
        }
        else {
            cout << "No password found for " << platform << "." << endl;
        }
    }
    else {
        cout << "Error Occured! Kindly pick from the choices available!" << endl;
    }

    return 0;
}
