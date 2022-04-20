#include "LL1parser.hpp"
int main()
{
    string grammar, sentence;
    cin >> grammar >> sentence;
    LL1gr gr = LL1gr(grammar);
    gr.readGrammar();
    gr.ll1ifyGrammar();
    gr.printGrammar();
    LL1pr pr = LL1pr(gr, sentence);
    pr.firstUtil();
    pr.followUtil();
    pr.fillParsingTable();
    pr.parser();
}