#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "SymTab.cpp"

using namespace std;

SymTab S;
vector<string> tokens;

void tokenize(string str)
{
    string token;
    istringstream ss(str);
    while (ss >> token)
    {
        tokens.push_back(token);
    }
}

int main()
{

    string str;
    while (getline(cin, str))
        tokenize(str);
    int sz = tokens.size(), i = 0;
    S.entry();
    while (i < sz)
    {
        if (tokens[i] == "int" || tokens[i] == "float" || tokens[i] == "char")
        {
            S.insert(tokens[i + 1], tokens[i]);
            i += 3;
        }
        else if (tokens[i] == "{")
        {
            S.entry();
            ++i;
        }
        else if (tokens[i] == "}")
        {
            S.exit();
            ++i;
        }
        else
        {
            ++i;
        }
    }
    S.exit();
    return 0;
}