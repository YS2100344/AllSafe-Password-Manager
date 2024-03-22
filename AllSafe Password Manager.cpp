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
