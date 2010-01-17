#include <string>
#include <vector>

#include "parsing.h"

using namespace std;

string trim(string s, char c) {
    int start = 0;
    int end = s.length() - 1;

    while ((start < (int) s.length()) && (s[start] == c)) ++start;

    if (start >= (int) s.length()) return "";

    while ((end > 0) && (s[end] == c)) --end;

    return s.substr(start, end - start + 1);
}

vector<string> split(string s, char c) {

    vector<string> result;

    unsigned int i = 0;

    while (i < s.length()) {

        while ((s[i] == c) && (i < s.length()))	++i;

        string nextString = "";

        while ((s[i] != c) && (i < s.length()))
            nextString += s[i++];

        if (nextString.length() > 0)
            result.push_back(nextString);
    }

    return result;

}

string getValue(string s, char c) {

    int i = s.find(c);

    if (i != (int) string::npos)
        return s.substr( i + 1 );
    else
        return "";

}

string getKey(string s, char c) {

    int i = s.find(c);

    if (i != (int) string::npos)
        return s.substr( 0, i );
    else
        return "";

}

