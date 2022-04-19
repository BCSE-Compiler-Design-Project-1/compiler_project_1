// C++ implementation to print N-ary Tree graphically

#include <iostream>
#include <list>
#include <vector>

using namespace std;

// Structure of the node
struct tnode
{
    string n;
    list<tnode *> root;
    tnode()
    {
    }
    tnode(string data) : n(data)
    {
    }
};

// Function to print the N-ary tree graphically
void printNTree(tnode *x, vector<bool> flag, int depth = 0, bool isLast = false)
{
    // Condition when node is None
    if (x == NULL)
        return;

    // Loop to print the depths of the current node
    for (int i = 1; i < depth; ++i)
    {
        // Condition when the depth is exploring
        if (flag[i] == true)
            cout << "| "
                 << " "
                 << " "
                 << " ";
        // Otherwise print the blank spaces
        else
            cout << " "
                 << " "
                 << " "
                 << " ";
    }

    // Condition when the current node is the root node
    if (depth == 0)
        cout << x->n << '\n';

    // Condition when the node is the last node of the exploring depth
    else if (isLast)
    {
        cout << "+--- " << x->n << '\n';
        // No more childrens turn it to the non-exploring depth
        flag[depth] = false;
    }
    else
        cout << "+--- " << x->n << '\n';

    int it = 0;
    for (auto i = x->root.begin(); i != x->root.end(); ++i, ++it)

        // Recursive call for the children nodes
        printNTree(*i, flag, depth + 1, it == (x->root.size()) - 1);
    flag[depth] = true;
}

// Function to form the Tree and print it graphically
/*void formAndPrintTree()
{
    int nv = 10;
    tnode r(0), n1(1), n2(2), n3(3), n4(4), n5(5), n6(6), n7(7), n8(8), n9(9);

    // Array to keep track of exploring depths
    vector<bool> flag(nv, true);

    // Tree Formation
    r.root.push_back(&n1);
    n1.root.push_back(&n4);
    n1.root.push_back(&n5);
    r.root.push_back(&n2);
    r.root.push_back(&n3);
    n3.root.push_back(&n6);
    n3.root.push_back(&n7);
    n7.root.push_back(&n9);
    n3.root.push_back(&n8);

    printNTree(&r, flag);
}
*/