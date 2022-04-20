#include "LL1parser.hpp"
int main(int argc, char **argv)
{
    if (argc != 3)
        exit(1);
    LL1gr gr = LL1gr(argv[1], argv[2]);
    gr.readGrammar();
    gr.ll1ifyGrammar();
    gr.printGrammar();
    LL1pr pr = LL1pr(gr, argv[3], argv[2]);
    pr.firstUtil();
    pr.followUtil();
    pr.fillParsingTable();
    pr.parser();
}