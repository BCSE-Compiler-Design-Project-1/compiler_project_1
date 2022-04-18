#include <bits/stdc++.h>

using namespace std;

class Attribute {
    int lineno;
    int level;
    string data_type;
    public:

    Attribute() {
    }

    Attribute(string dt) {
        data_type = dt;
    }

    friend class SymTab;
};

class SymTab {
    vector<unordered_map<string,Attribute>> hashmaps; 
    int currLevel;
    string outfile;
    
    bool checkAtLevel(string idname, int level) {
        if (level <= currLevel) {
            return (hashmaps[level].find(idname) != hashmaps[level].end());
        }
        return false;
    }

    public:
    SymTab(string filename = "symtab.txt") {
        currLevel = -1;
        outfile = filename;
        ofstream fout(outfile, ios::out);
        fout.close();
    }
    void entry() {
        ++currLevel;
        hashmaps.push_back(unordered_map<string, Attribute>());
    }
    void exit() {
        if (currLevel >= 0) {
            ofstream fout(outfile, ios::app);
            --currLevel;
            if(hashmaps[currLevel + 1].size()!=0){
                fout << "Symbol Table for the exiting block: \n";
                for (auto itr : hashmaps[currLevel + 1]) {
                    fout << "Identifier : " << itr.first << " ---> Data Type : " << itr.second.data_type << " , Scope Number : " << currLevel + 1 << "\n";
                }
                fout<<"\n";
            }           
            fout.close();
            hashmaps.pop_back();
        }
    }
    pair<bool, Attribute> lookup(string idname) {
        for (int level = currLevel; level >= 0; --level) {
            if (checkAtLevel(idname, level))
                return {true, hashmaps[level].find(idname)->second};
        }
        return {false, Attribute()};
    }
    bool insert(string idname, string type) {
        if (checkAtLevel(idname, currLevel)) {
            cerr << "Error!!!! Multiple Declarations. (" << idname << " is already declared)\n";
            return false;
        } else {
            hashmaps[currLevel][idname] = Attribute(type);
            return true;
        }
    }
};
