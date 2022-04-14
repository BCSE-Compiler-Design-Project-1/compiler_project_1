#include <iostream>
#include <math.h>
#include <vector>
#include <list>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#define EPSILON "\u03B5"
using namespace std;
class LL1gr
{
private:
    unordered_map<string, int> non_terminals;
    unordered_map<string, int> terminals;
    unordered_map<string, vector<vector<string>>> parsed_prod;
    string startSym;
    int nTcount = 0;
    int Tcount = 0;
    void insertTl(string);
    void inserNtl(string);
    void printProd(const vector<string> &);
    vector<vector<string>> parse(const string &);
    vector<string> parse2(const string &);
    void left_factor(const string &);
    void remLR_dir(const string &);
    vector<vector<string>> concat_prod(const string &, const vector<vector<string>>::iterator &);
    void remLR_indir();

public:
    void readGrammar();
    void printGrammar();
    void ll1ifyGrammar();
    unordered_map<string, int> &getNt()
    {
        return non_terminals;
    }
    unordered_map<string, int> &getT()
    {
        return terminals;
    }
    unordered_map<string, vector<vector<string>>> &getParsedProd()
    {
        return parsed_prod;
    }
    string getStartSym()
    {
        return startSym;
    }
};

void LL1gr::printGrammar()
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

void LL1gr::printProd(const vector<string> &pr)
{
    for (auto it : pr)
        cout << it << " ";
}

void LL1gr::insertTl(string tl)
{
    if (non_terminals.find(tl) == non_terminals.end())
        terminals[tl] = this->Tcount++;
}

void LL1gr::inserNtl(string nTl)
{
    if (!this->nTcount)
        this->startSym = nTl;
    if (non_terminals.find(nTl) == non_terminals.end())
    {
        non_terminals[nTl] = this->nTcount++;
        parsed_prod[nTl] = vector<vector<string>>();
    }
    if (terminals.find(nTl) != terminals.end())
    {
        terminals.erase(nTl);
        this->Tcount--;
    }
}

vector<vector<string>> LL1gr::parse(const string &prod)
{
    vector<vector<string>> productions;
    string s = prod.substr(prod.find(":-") + 3);
    vector<string> temp;
    string p = "";
    for (char ch : s)
    {
        if (ch == '|')
        {
            productions.push_back(temp);
            temp.clear();
            p = "";
        }
        else if (ch == ' ')
        {
            if (!p.empty())
            {
                if (p == "#")
                    p = EPSILON;
                temp.push_back(p);
                this->insertTl(p);
                p = "";
            }
        }
        else
            p += ch;
    }
    if (p == "#")
        p = EPSILON;
    temp.push_back(p);
    this->insertTl(p);
    productions.push_back(temp);
    return productions;
}

vector<string> LL1gr::parse2(const string &prod)
{
    vector<string> productions;
    string s = prod.substr(prod.find(":-") + 3);
    string p = "";
    for (char ch : s)
    {
        if (ch == ' ')
        {
            if (!p.empty())
            {
                if (p == "#")
                    p = EPSILON;
                productions.push_back(p);
                this->insertTl(p);
                p = "";
            }
        }
        else
            p += ch;
    }
    if (p == "#")
        p = EPSILON;
    productions.push_back(p);
    this->insertTl(p);
    return productions;
}

void LL1gr::readGrammar()
{
    int n;
    cin >> n;
    cin.ignore();
    string s;
    while (n--)
    {
        getline(cin, s);
        string nTl = s.substr(0, s.find(" "));
        this->inserNtl(nTl);
        parsed_prod[nTl].push_back(parse2(s));
    }
    if (terminals.find(EPSILON) == terminals.end())
        insertTl(EPSILON);
}

void LL1gr::left_factor(const string &prod)
{
    int i, j;
    int e = 1, p = this->parsed_prod[prod].size();
    vector<pair<string, int>> prodleft(p, {prod, 0});
    vector<vector<string>> prodright(this->parsed_prod[prod]);
    for (i = 0; i < p; ++i)
    {
        for (j = i + 1; j < p; ++j)
        {
            if (prodleft[j].second == prodleft[i].second)
            {
                int k = 0;
                vector<string> com;
                while (k < prodright[i].size() && k < prodright[j].size() && prodright[i][k] == prodright[j][k])
                {
                    com.push_back(prodright[i][k]);
                    ++k;
                }
                if (k == 0)
                    continue;
                pair<string, int> comleft = prodleft[i];
                if (k == prodright[i].size())
                {
                    prodleft[i].second = e;
                    prodleft[j].second = e;
                    prodright[i].clear();
                    prodright[i].push_back(EPSILON);
                    vector<string> temp(prodright[j]);
                    prodright[j].clear();
                    prodright[j].insert(prodright[j].end(), temp.begin() + k, temp.end());
                }
                else if (k == prodright[j].size())
                {
                    prodleft[i].second = e;
                    prodleft[j].second = e;
                    prodright[j].clear();
                    prodright[j].push_back(EPSILON);
                    vector<string> temp(prodright[i]);
                    prodright[i].clear();
                    prodright[i].insert(prodright[i].end(), temp.begin() + k, temp.end());
                }
                else
                {
                    prodleft[i].second = e;
                    prodleft[j].second = e;
                    vector<string> temp1(prodright[i]);
                    prodright[i].clear();
                    prodright[i].insert(prodright[i].end(), temp1.begin() + k, temp1.end());
                    vector<string> temp2(prodright[j]);
                    prodright[j].clear();
                    prodright[j].insert(prodright[j].end(), temp2.begin() + k, temp2.end());
                }
                int l;
                for (l = j + 1; l < p; ++l)
                {
                    if (comleft.first == prodleft[l].first && comleft.second == prodleft[l].second && equal(prodright[l].begin(), prodright[l].begin() + min(k, int(prodright[l].size())), com.begin()))
                    {
                        prodleft[l].second = e;
                        vector<string> temp(prodright[l]);
                        prodright[l].clear();
                        prodright[l].insert(prodright[l].end(), temp.begin() + k, temp.end());
                    }
                }
                prodleft.push_back(comleft);
                vector<string> temp(com);
                temp.push_back(prodleft[i].first + (prodleft[i].second ? to_string(prodleft[i].second) : ""));
                prodright.push_back(temp);
                ++p;
                ++e;
            }
        }
    }
    parsed_prod.erase(prod);
    for (int i = 0; i < p; ++i)
    {
        string ntl = prodleft[i].first + (prodleft[i].second ? to_string(prodleft[i].second) : "");
        if (non_terminals.find(ntl) == non_terminals.end())
            this->inserNtl(ntl);
        parsed_prod[prodleft[i].first + (prodleft[i].second ? to_string(prodleft[i].second) : "")].push_back(prodright[i]);
    }
}

vector<vector<string>> LL1gr::concat_prod(const string &prod, const vector<vector<string>>::iterator &it)
{
    auto it1 = it->begin();
    it1++;
    auto it2 = it->end();
    vector<vector<string>> productions;
    vector<string> new_prod;
    for (auto pr : this->parsed_prod[prod])
    {
        new_prod.reserve(pr.size() + it2 - it1 + 1);
        new_prod.insert(new_prod.end(), pr.begin(), pr.end());
        new_prod.insert(new_prod.end(), it1, it2);
        productions.push_back(new_prod);
        new_prod.clear();
    }
    return productions;
}

void LL1gr::remLR_indir()
{
    vector<string> prods(non_terminals.size());
    for (auto it : non_terminals)
        prods[it.second] = it.first;
    for (int i = 0; i < prods.size(); ++i)
    {
        string ci = prods[i];
        for (int j = 0; j < i; ++j)
        {
            string cj = prods[j];
            for (auto it = parsed_prod[ci].begin(); it != parsed_prod[ci].end();)
            {
                if ((*it)[0] == cj)
                {

                    vector<vector<string>> new_prod = concat_prod(cj, it);
                    it = parsed_prod[ci].erase(it);
                    it = parsed_prod[ci].insert(it, new_prod.begin(), new_prod.end());
                    it += new_prod.size();
                }
                else
                    it++;
            }
        }
        remLR_dir(ci);
    }
}
void LL1gr::remLR_dir(const string &prod)
{
    vector<vector<string>> recur, non_recur;
    for (auto it : this->parsed_prod[prod])
    {
        if (prod != it[0])
            non_recur.push_back(it);
        else
            recur.push_back(vector<string>(it.begin() + 1, it.end()));
    }
    if (recur.empty())
        return;
    this->parsed_prod[prod].clear();
    this->insertTl(EPSILON);
    for (auto x : non_recur)
    {
        if (x[0] == EPSILON)
            x.pop_back();
        x.push_back(prod + "\'");
        this->parsed_prod[prod].push_back(x);
    }
    vector<vector<string>> new_prod;
    for (auto x : recur)
    {
        x.push_back(prod + "\'");
        new_prod.push_back(x);
    }
    new_prod.push_back(vector<string>(1, EPSILON));
    this->inserNtl(prod + "\'");
    this->parsed_prod[prod + "\'"] = new_prod;
}

void LL1gr::ll1ifyGrammar()
{
    for (auto it : non_terminals)
        this->left_factor(it.first);
    this->remLR_indir();
}