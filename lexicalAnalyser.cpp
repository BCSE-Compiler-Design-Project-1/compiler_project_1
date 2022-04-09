#include <iostream>
#include <regex>
#include <fstream>
#include <string>

using namespace std;
string keywords = "int|float|void|main|if|else|then|cin|cout";
string pattern_id = "(\\b(?!" + keywords + "\\b)[A-Za-z_][A-Za-z0-9_]*\\b)";

regex regex_for_identifier(pattern_id);
regex regex_for_negative_int("[-][0-9]+"); //^[-+]?\d*$
regex regex_for_positive_int("[0-9]+");

regex regex_for_positive_float("[0-9]+[.][0-9]+");
regex regex_for_negative_float("[-][0-9]+[.][0-9]+");

regex regex_for_type("int|float|void");

int main()
{
    string fileName;
    cout << "Enter the file name :- ";
    cin >> fileName;

    fstream file;
    file.open(fileName, ios::in);

    string tp;
    while (getline(file, tp))
    {
        tp = regex_replace(tp, regex_for_identifier, "id");

        tp = regex_replace(tp, regex_for_type, "TYPE");

        tp = regex_replace(tp, regex_for_negative_float, "float");
        tp = regex_replace(tp, regex_for_positive_float, "float");

        tp = regex_replace(tp, regex_for_negative_int, "int");
        tp = regex_replace(tp, regex_for_positive_int, "int");

        cout << tp << "\n";
    }

    return 0;
}