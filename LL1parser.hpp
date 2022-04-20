#include <stack>
#include <fstream>
#include "LL1grammer.hpp"
#include "n-aryTree.hpp"

class LL1pr
{
private:
    unordered_map<string, int> non_terminals;
    unordered_map<string, int> terminals;
    unordered_map<string, vector<vector<string> > > parsed_prod;
    vector<string> prods;
    unordered_map<string, unordered_set<string> > FIRST;
    unordered_map<string, unordered_set<string> > FOLLOW;
    unordered_map<int, pair<int, int> > productions;
    vector<list<int> > follow_dep;
    vector<vector<int> > parsing_table;
    string startSym;
    int nTcount = 0;
    int Tcount = 0;
    fstream inpFile;
    // int node_count = 1;
    // tnode root;
    // vector<bool> flag;
    void printProd(const vector<string> &);
    void firstNTL(const string &);
    bool firstRHS(const string &, int, unordered_set<string> &);
    void follow(const string &);
    void topologicalSort();
    void topologicalSortUtil(int, vector<bool> &, stack<int> &);
    void printParsing(const string &, const vector<string> &, const string &, const string &);

public:
    LL1pr(LL1gr &gr, string file1)
    {
        inpFile.open(file1, ios::in);
        this->non_terminals = gr.getNt();
        this->terminals = gr.getT();
        this->parsed_prod = gr.getParsedProd();
        this->startSym = gr.getStartSym();
        // this->root = tnode(this->startSym);
        this->nTcount = this->non_terminals.size();
        this->Tcount = this->terminals.size();
        this->follow_dep.resize(this->nTcount);
        this->prods.resize(non_terminals.size());
        for (auto it : non_terminals)
            prods[it.second] = it.first;
        int count = 0;
        for (auto it : this->prods)
        {
            // cout << it << " " << non_terminals[it] << "  ";
            for (int i = 0; i < this->parsed_prod[it].size(); i++)
            {
                // cout << count << " " << it << " " << i << "   ";
                productions[count++] = {non_terminals[it], i};
            }
            // cout << "\n\n";
        }
        parsing_table.resize(non_terminals.size());
        for (auto &it : parsing_table)
            it = vector<int>(this->Tcount, -1);
        int i = 0;
        for (auto &it : terminals)
        {
            it.second = i++;
            // cout << it.first << " " << it.second << "\n";
        }
    }
    void firstUtil();
    void followUtil();
    void printGrammar();
    void fillParsingTable();
    void parser();
};

void LL1pr::printGrammar()
{
    for (auto it : parsed_prod)
    {
        cout << it.first << " -> ";
        printProd(it.second[0]);
        for (int i = 1; i < it.second.size(); ++i)
        {
            cout << "| ";
            printProd(it.second[i]);
        }
        cout << "\n";
    }
}

void LL1pr::printProd(const vector<string> &pr)
{
    for (auto it : pr)
        cout << it << " ";
}

void LL1pr::firstUtil()
{
    for (auto it : non_terminals)
    {
        if (FIRST.find(it.first) == FIRST.end())
            firstNTL(it.first);
    }
    for (auto it : FIRST)
    {
        cout << it.first << " - {";
        for (auto it1 : it.second)
            cout << " '" << it1 << "' ,";
        cout << " }\n";
    }
    cout << "\n\n";
}
bool LL1pr::firstRHS(const string &s, int i, unordered_set<string> &first)
{
    auto &it = parsed_prod[s][i];
    if (it[0] == EPSILON)
    {
        first.insert(EPSILON);
        return true;
    }
    bool flag = true;
    for (auto it1 : it)
    {
        if (terminals.find(it1) != terminals.end())
        {
            // cout << it1 << "\n";
            first.insert(it1);
            flag = false;
            break;
        }
        else
        {
            // cout << it1 << "\n";
            if (FIRST.find(it1) == FIRST.end())
                firstNTL(it1);
            unordered_set<string> temp(FIRST[it1].begin(), FIRST[it1].end());
            if (temp.find(EPSILON) != temp.end())
            {
                temp.erase(EPSILON);
                first.insert(temp.begin(), temp.end());
            }
            else
            {
                flag = false;
                first.insert(temp.begin(), temp.end());
                break;
            }
        }
    }
    return flag;
}

void LL1pr::firstNTL(const string &s)
{
    bool flag;
    for (auto i = 0; i != parsed_prod[s].size(); i++)
    {
        flag = firstRHS(s, i, FIRST[s]);
        if (flag)
            FIRST[s].insert(EPSILON);
    }
}

void LL1pr::followUtil()
{
    FOLLOW[startSym].insert("$");
    for (auto it : prods)
        follow(it);
    topologicalSort();
    for (auto it : FOLLOW)
    {
        cout << it.first << " - {";
        for (auto it1 : it.second)
            cout << " '" << it1 << "' ,";
        cout << " }\n";
    }
    cout << "\n\n";
}
void LL1pr::follow(const string &s)
{
    for (auto it : parsed_prod[s])
    {
        unordered_set<string> currFIRST;
        currFIRST.insert(EPSILON);
        for (auto it1 = it.rbegin(); it1 != it.rend(); it1++)
        {
            if (terminals.find(*it1) != terminals.end())
            {
                currFIRST.clear();
                currFIRST.insert(*it1);
                continue;
            }
            else
            {
                FOLLOW[*it1].insert(currFIRST.begin(), currFIRST.end());
                if (currFIRST.count(EPSILON) != 0)
                {
                    FOLLOW[*it1].erase(EPSILON);
                    if (*it1 != s)
                        follow_dep[non_terminals[s]].push_back(non_terminals[*it1]);
                }
                if (FIRST[*it1].find(EPSILON) == FIRST[*it1].end())
                {
                    currFIRST = FIRST[*it1];
                }
                else
                {
                    currFIRST.insert(FIRST[*it1].begin(), FIRST[*it1].end());
                }
            }
        }
    }
}

void LL1pr::topologicalSort()
{
    stack<int> Stack;
    vector<bool> visited(non_terminals.size(), false);

    for (int i = 0; i < non_terminals.size(); i++)
        if (!visited[i])
            topologicalSortUtil(i, visited, Stack);

    while (!Stack.empty())
    {
        int v = Stack.top();
        for (auto i = follow_dep[v].begin(); i != follow_dep[v].end(); ++i)
        {
            FOLLOW[prods[*i]].insert(FOLLOW[prods[v]].begin(), FOLLOW[prods[v]].end());
        }
        Stack.pop();
    }
    cout << "\n";
}

void LL1pr::topologicalSortUtil(int v, vector<bool> &visited, stack<int> &Stack)
{
    visited[v] = true;

    for (auto i = follow_dep[v].begin(); i != follow_dep[v].end(); ++i)
        if (!visited[*i])
            topologicalSortUtil(*i, visited, Stack);

    Stack.push(v);
}

void LL1pr::fillParsingTable()
{
    unordered_set<string> temp;
    int count = 0;
    for (int i = 0; i < this->nTcount; i++)
    {
        for (int j = 0; j < this->parsed_prod[this->prods[i]].size(); j++)
        {
            firstRHS(this->prods[i], j, temp);
            for (auto it : temp)
            {
                if (it != EPSILON)
                {
                    this->parsing_table[i][this->terminals[it]] = count;
                }
                else
                {
                    for (auto it1 : FOLLOW[prods[i]])
                        if (it1 != "$")
                            this->parsing_table[i][this->terminals[it1]] = count;
                        else
                            this->parsing_table[i][this->terminals[EPSILON]] = count;
                }
            }
            ++count;
            temp.clear();
        }
        for (auto it : terminals)
            if (it.first != EPSILON && this->parsing_table[i][it.second] == -1 && FIRST[prods[i]].find(it.first) == FIRST[prods[i]].end() && FOLLOW[prods[i]].find(it.first) == FOLLOW[prods[i]].end())
                this->parsing_table[i][it.second] = -2;
    }
    cout << "\n";
    for (auto i = 0; i < nTcount; ++i)
    {
        cout.width(20);
        cout << left << prods[i];
        for (auto j = 0; j < Tcount; ++j)
        {
            cout.width(7);
            cout << right << parsing_table[i][j];
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void LL1pr::parser()
{
    stack<string> parsing_st;
    vector<string> curr_st;
    string matched = "", action = "push " + startSym, st = "";
    printParsing("MATCHED", vector<string>(1, "STACK"), "INPUT", "ACTION");
    parsing_st.push("$");
    parsing_st.push(startSym);
    curr_st.push_back("$");
    curr_st.push_back(startSym);
    // tnode *prev_node = &root, *curr_node = nullptr;
    string curr_token;
    bool flag = true;
    inpFile >> curr_token;
    printParsing(matched, curr_st, curr_token, "push " + startSym);
    do
    {
        if (parsing_st.top() == curr_token)
        {
            if (curr_token == "$")
            {
                // cout << "accepted.\n";
                action = "accept";
                flag = false;
            }
            else
            {
                matched += parsing_st.top() + " ";
                action = "match " + parsing_st.top();
                parsing_st.pop();
                curr_st.pop_back();
                inpFile >> curr_token;
            }
            printParsing(matched, curr_st, curr_token, action);
        }
        else
        {
            int prod_num = parsing_table[non_terminals[parsing_st.top()]][curr_token != "$" ? terminals[curr_token] : terminals[EPSILON]];
            if (prod_num >= 0)
            {
                pair<int, int> temp = productions[prod_num];
                vector<string> prod = parsed_prod[prods[temp.first]][temp.second];
                parsing_st.pop();
                curr_st.pop_back();
                if (prod[0] != EPSILON)

                    for (auto it = prod.rbegin(); it != prod.rend(); it++)
                    {
                        parsing_st.push(*it);
                        curr_st.push_back(*it);
                    }
                action = "output " + prods[temp.first] + " -> ";
                for (auto it = prod.begin(); it != prod.end(); it++)
                {
                    action += *it + " ";
                    // curr_node->root.push_back(new tnode(*it));
                }
                printParsing(matched, curr_st, curr_token, action);
            }
            else if (prod_num == -1)
            {
                if (parsing_st.top() == startSym)
                {
                    action = "error skip " + curr_token;
                    printParsing(matched, curr_st, curr_token, action);
                    inpFile >> curr_token;
                }
                else
                {
                    curr_st.pop_back();
                    parsing_st.pop();
                    action = "error M[" + parsing_st.top() + ", '" + curr_token + "' ] = pop";
                    printParsing(matched, curr_st, curr_token, action);
                }
            }
            else
            {
                action = "error M[" + parsing_st.top() + ", '" + curr_token + "' ] = scan";
                printParsing(matched, curr_st, curr_token, action);
                inpFile >> curr_token;
            }
        }
    } while (flag);
    inpFile.close();
}

void LL1pr::printParsing(const string &matched, const vector<string> &curr_st, const string &input_token, const string &action)
{
    cout.width(50);
    cout << left << matched << " | ";
    string st = "";
    for (auto it = curr_st.rbegin(); it != curr_st.rend(); it++)
        st += *it + " ";
    cout.width(40);
    cout << left << st << " | ";
    cout.width(10);
    cout << left << input_token << " | ";
    cout.width(30);
    cout << left << action;
    cout << endl;
}