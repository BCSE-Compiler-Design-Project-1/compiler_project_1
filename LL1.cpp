#include "LL1parser.hpp"
int main()
{
    LL1gr gr = LL1gr();
    gr.readGrammar();
    gr.ll1ifyGrammar();
    gr.printGrammar();
    cout << "\n\n";
    LL1pr pr = LL1pr(gr);
    pr.firstUtil();
    cout << "\n\n";
    pr.followUtil();
    cout << "\n\n";
    pr.fillParsingTable();
    cout << "\n\n";
    pr.parser();
    cout << "\n\n";
}