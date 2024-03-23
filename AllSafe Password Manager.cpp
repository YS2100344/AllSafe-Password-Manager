#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <algorithm>
#include <cctype>



using namespace std;


string vigenereCipher(const string& text, const string& key, bool encrypt) {
    string result;
    int keyIndex = 0, keyLength = key.length();

    for (char c : text) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            int offset = (encrypt ? 1 : -1) * (key[keyIndex] - base);
            c = static_cast<char>(((c - base + offset + 26) % 26) + base);
            keyIndex = (keyIndex + 1) % keyLength;
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
bool containsNumberAndSpecialChar(const string& password) {
    bool hasNumber = false, hasSpecial = false;
    for (char c : password) {
        if (isdigit(c)) hasNumber = true;
        if (ispunct(c)) hasSpecial = true;
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
        }
    } while (!containsNumberAndSpecialChar(password));
    return password;
}

void getPasswordFromUser(string& password) {
    do {
        cout << "Enter your password, the following is required: (8 or more characters, must include 1 number and 1 special character): ";
        getline(cin, password);
        password = trim(password); 
    } while (password.length() < 8 || !containsNumberAndSpecialChar(password));
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

string retrievePassword(const string& username, const string& platform, const string& key) {
    ifstream file("passwords.txt");
    string storedUsername, storedplatform, storedPassword;
    while (file >> storedUsername >> storedplatform >> storedPassword) {
        if (storedUsername == username && storedplatform == platform) {
            return vigenereCipher(storedPassword, key, false);
        }
    }
    return "Platform not found! Kindly Try Again!.";
}

bool isUsernameTaken(const string& username) {
    ifstream usersFile("usernames&passwords.txt");
    string storedUsername, storedPassword;
    while (usersFile >> storedUsername >> storedPassword) {
        if (storedUsername == username) {
            return true;
        }
    }
    return false;
}

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
                }
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
    cin.ignore(); 

    if (choice == 1) {
        cout << "Platform Name: ";
        getline(cin, platform);

        bool generatePass = getYesOrNoResponse("Would you like your password to be generated (recommended)?");
        if (generatePass) {
            int passLength = 0;
            do {
                cout << "Enter password length (minimum 8 characters): ";
                cin >> passLength;
                cin.ignore(); 
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
        cout << "Error Occurred! Kindly pick from the choices available!" << endl;
    }

    return 0;
}
