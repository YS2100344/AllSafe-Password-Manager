#include <iostream>
#include <fstream>
#include <string>
#include <ctime>


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

    for (int i = 0; i < length; ++i) {
        password += chars[rand() % chars.size()];
    }
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

      
        for (auto& c : response) c = tolower(c);

    } while (response != "yes" && response != "no");

    return response == "yes";
}

void storePassword(const string& username, const string& platform, const string& password, const string& key) {
    ofstream file("passwords.txt", ios::app);
    if (!file.is_open()) {
        cerr << "Error" << endl;
        return;
    }
    file << username << " " << platform << " " << vigenereCipher(password, key, true) << "\n";
    file.close();
}
bool loginUser() {
    string username, password, fileUsername, filePasswordEncrypted, key = "V1gn3r3C1ph3r";
    cout << "Input Username: ";
    getline(cin, username);

    ifstream usersFile("usernames&passwords.txt");
    if (!usersFile.is_open()) {
        cerr << "Unable to open users file." << endl;
        return false;
    }

    while (usersFile >> fileUsername >> filePasswordEncrypted) {
        if (fileUsername == username) {
            cout << "Input Password: ";
            getline(cin, password);
            if (vigenereCipher(filePasswordEncrypted, key, false) == password) {
                cout << "Login successful." << endl;
                return true;
            } else {
                cout << "Invalid password." << endl;
                return false;
            }
        }
    }
    cout << "Username not found." << endl;
    return false;
}
//Testing Purposes
int main() {
    
    int passwordLength = 12;
    string generatedPassword = generatePassword(passwordLength);
    cout << "Generated password: " << generatedPassword << endl;

    
    cout << "Does the generated password contain at least one number and one special character? "
         << (containsNumberAndSpecialChar(generatedPassword) ? "Yes" : "No") << endl;

    
    string test = "test";
    string key = "testkey";
    string encryptedMessage = vigenereCipher(test, key, true);
    string decryptedMessage = vigenereCipher(encryptedMessage, key, false);

    cout << "Original message: " << test << endl;
    cout << "Encrypted message: " << test << endl;
    cout << "Decrypted message: " << test << endl;

    return 0;
}
