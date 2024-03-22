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
//Testing Purposes
int main() {
    
    int passwordLength = 12;
    string generatedPassword = generatePassword(passwordLength);
    cout << "Generated password: " << generatedPassword << endl;

    
    cout << "Does the generated password contain at least one number and one special character? "
         << (containsNumberAndSpecialChar(generatedPassword) ? "Yes" : "No") << endl;

    
    string test = "test";
    string key = "Vigenere";
    string encryptedMessage = vigenereCipher(test, key, true);
    string decryptedMessage = vigenereCipher(encryptedMessage, key, false);

    cout << "Original message: " << test << endl;
    cout << "Encrypted message: " << test << endl;
    cout << "Decrypted message: " << test << endl;

    return 0;
}
