#include <iostream>
#include <fstream>
#include <string>

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
