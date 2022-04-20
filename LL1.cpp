#include "LL1parser.hpp"
int main()
{
    string proj_gr, sentence;
    cout << "Enter the project grammar file name :- ";
    cin >> proj_gr;
    LL1gr gr = LL1gr(proj_gr);
    cout << "Enter the sentence :- ";
    cin >> sentence;

    gr.readGrammar();
    gr.ll1ifyGrammar();
    gr.printGrammar();
    LL1pr pr = LL1pr(gr, sentence);
    pr.firstUtil();
    pr.followUtil();
    pr.fillParsingTable();
    pr.parser();
}