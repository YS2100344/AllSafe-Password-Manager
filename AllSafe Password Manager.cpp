#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <algorithm>

using namespace std;

string vigenereCipher(const string& text, const string& key, bool encrypt) {
    string result;
    int keyIndex = 0, keyLength = key.length();
    for (char c : text) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            int offset = (encrypt ? 1 : -1) * (tolower(key[keyIndex % keyLength]) - 'a');
            c = static_cast<char>(((c - base + offset + 26) % 26) + base);
            keyIndex++;
        }
        result += c;
    }
    return result;
}
string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}
bool containsNumberAndSpecialChar(const string& str) {
    bool hasNumber = false, hasSpecial = false;
    for (char c : str) {
        if (c >= '0' && c <= '9') hasNumber = true;
        else if (!(c >= 'a' && c <= 'z') && !(c >= 'A' && c <= 'Z') && !(c == ' ')) hasSpecial = true;
    }
    return hasNumber && hasSpecial;
}

string generatePassword(int length) {
    string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()";
    string password;
    srand(static_cast<unsigned int>(time(nullptr)));
    do {
        password.clear();
        for (size_t i = 0; i < length; ++i) { 
            password += chars[rand() % chars.size()]; 
    } while (!containsNumberAndSpecialChar(password));
    return password;
}
void getPasswordFromUser(string& password) {
    do {
        cout << "Enter your password, it must include at least 1 number and 1 special character: ";
        getline(cin, password);

        
        bool hasNumber = false, hasSpecial = false;
        for (char c : password) {
            if (c >= '0' && c <= '9') hasNumber = true;
            if (!isalnum(c)) hasSpecial = true; 
        }

        if (!hasNumber || !hasSpecial || password.length() < 8) {
            cout << "Password does not meet requirements.\n";
        }
    } while (password.length() < 8 || !containsNumberAndSpecialChar(password));
}
bool isUsernameTaken(const string& username) {
    ifstream file("usernames&passwords.txt");
    string storedUsername;
    while (file >> storedUsername) {
        if (storedUsername == username) {
            return true; 
        }
        file.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return false;
}

void registerUser(const string& username, const string& password) {
    if (!isUsernameTaken(username)) {
        ofstream usersFile("usernames&passwords.txt", ios::app);
        if (usersFile.is_open()) {
            usersFile << username << " " << vigenereCipher(password, "V1gn3r3C1ph3r", true) << "\n";
            cout << "Registration successful." << endl;
        } else {
            cerr << "Error occurred while processing your request!" << endl;
        }
        usersFile.close();
    } else {
        cout << "Username is already taken." << endl;
    }
}

string retrievePassword(const string& username, const string& platform, const string& key) {
    ifstream file("passwords.txt");
    string storedUsername, storedPlatform, storedPassword;
    while (file >> storedUsername >> storedPlatform >> storedPassword) {
        if (storedUsername == username && storedPlatform == platform) {
            return vigenereCipher(storedPassword, key, false); 
        }
    
        file.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return "";
}

bool getYesOrNoResponse(const string& question) {
    string response;
    do {
        cout << question << " (yes/no): ";
        getline(cin, response);
        transform(response.begin(), response.end(), response.begin(), ::tolower); 
    } while (response != "yes" && response != "no");
    return response == "yes";
}
void storePassword(const string& username, const string& platform, const string& password, const string& key) {
    ofstream file("passwords.txt", ios::app);
    if (file.is_open()) {
        file << username << " " << platform << " " << vigenereCipher(password, key, true) << "\n";
    }
    else {
        cerr << "Unable to open file to store passwords." << endl;
    }
    file.close();
}

bool loginUser(string& username) {
    cout << "Input Username: ";
    getline(cin, username);
    username = trim(username); 

    if (isUsernameTaken(username)) {
        ifstream usersFile("usernames&passwords.txt");
        string storedUsername, storedPasswordEncrypted;
        while (usersFile >> storedUsername >> storedPasswordEncrypted) {
            if (storedUsername == username) {
                string password;
                cout << "Input Password: ";
                getline(cin, password);
                password = trim(password); 

                string decryptedStoredPassword = vigenereCipher(storedPasswordEncrypted, "PaS28KeYyOuS3F", false);
                decryptedStoredPassword = trim(decryptedStoredPassword); 

                if (decryptedStoredPassword == password) {
                    usersFile.close(); 
                    cout << "Welcome back, " << username << "!" << endl;
                    return true; 
                else {
                    cout << "Invalid password." << endl;
                }
            }
        }
        usersFile.close(); 
    }
    else { 
        cout << "First Time Here? " << endl;
        if (getYesOrNoResponse("Would you like to sign up to AllSafe Password Manager?")) {
            string password;
            getPasswordFromUser(password);
            registerUser(username, password);
            cout << "Sign up successful! Welcome, " << username << "!" << endl;
            return true;
        }
        else {
            cout << "Error orccured while processing your request!" << endl;
            return false;
        }
    }

    return false; 
}

int main() {
    string username;

    if (!loginUser(username)) {
        cout << "Exiting program." << endl;
        return 1;
    }

    cout << "Welcome to AllSafe Password Management Application, " << username << "!" << endl;
    string platform, password;
    int choice;

    cout << "1: Store Password\n2: Retrieve Password\nEnter choice: ";
    cin >> choice;

    if (choice == 1) {
        cout << "Platform name: ";
        getline(cin, platform); 

        bool generatePass = getYesOrNoResponse("Would you like your password to be generated (recommended)?");
        if (generatePass) {
            int passLength = 0;
            do {
                cout << "Enter password length (minimum 8 characters): ";
                cin >> passLength;
                
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

        storePassword(username, platform, password, "V1gn3r3C1ph3r");
        cout << "Password stored successfully for " << platform << "." << endl;
    }
    else if (choice == 2) {
        cout << "Platform Name: ";
        getline(cin, platform); 
        password = retrievePassword(username, platform, "V1gn3r3C1ph3r");
        if (password != "Platform not found! Kindly Try Again!.") {
            cout << "Password for " << platform << ": " << password << endl;
        }
        else {
            cout << "No password found for " << platform << "." << endl;
        }
    }
    else {
        cout << "Error Occurred! Kindly pick from the choices available!" << endl;
    }

    return 0;
}
