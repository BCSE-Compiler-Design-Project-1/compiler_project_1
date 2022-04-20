#include <iostream>
#include <regex>
#include <fstream>
#include <string>

using namespace std;
string keywords = "void|int|float|main|if|else|then|cin|cout";
string pattern_id = "(\\b(?!" + keywords + "\\b)[A-Za-z_][A-Za-z0-9_]*\\b)";

regex regex_for_identifier(pattern_id);
regex regex_for_negative_int("[-][0-9]+"); //^[-+]?\d*$
regex regex_for_positive_int("[0-9]+");

regex regex_for_positive_float("[0-9]+[.][0-9]+");
regex regex_for_negative_float("[-][0-9]+[.][0-9]+");

int main()
{
    string fileName;
    cout << "Enter the file name :- ";
    cin >> fileName;

    fstream file;
    ofstream file1;

    file.open(fileName, ios::in);
    file1.open("lex_ana_output.txt");

    string tp;
    while (getline(file, tp))
    {
        tp = regex_replace(tp, regex_for_identifier, "id");

        tp = regex_replace(tp, regex_for_negative_float, "real");
        tp = regex_replace(tp, regex_for_positive_float, "real");

        tp = regex_replace(tp, regex_for_negative_int, "number");
        tp = regex_replace(tp, regex_for_positive_int, "number");

        file1 << tp << "\n";
    }

    return 0;
}