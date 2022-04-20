#include "LL1parser.hpp"
int main()
{
    string grammar, sentence, filename;
    int cnt = 0;

    while (getline(cin, filename))
    {
        if (cnt == 0)
            grammar = filename;
        else if (cnt == 1)
            sentence = filename;
        cnt++;
    }
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