#pragma once

#include <cstdlib>
#include <string>
#include <cstring>
#include <vector>
#include "rationalMatrix.h"
using namespace std;
class ChemItem
{
public:
    int type;
    int num;
    ChemItem(int myType, int myNum);
    bool operator==(const ChemItem &b);
};

class ChemEquation
{
public:
       ChemEquation(const char ga[]);
        ~ChemEquation();
        string myResult();

private:
    int myRow;
    int myColume;
    RationalMatrix *matrix;
    void setChem(const char *chem);
    void toMatrixBaby();

    const ChemItem N1 = ChemItem(0, 0);
    const ChemItem N2 = ChemItem(-1, 0);
    char *chemMine;
    int chemMineLen;
    string chemString;
    vector<string> cutString;
    vector<string> elementList;
    vector<ChemItem> complexResult;
    int **baby;

    int convertNumber(int k);
    void toList();
    int findEle(string x);
    int signChem(int x);
};
