#include "chem.h"

using namespace std;

bool ChemItem::operator==(const ChemItem &b)
{
    if (type == b.type && num == b.num)
    {
        return true;
    }
    else
    {
        return false;
    }
}

ChemItem::ChemItem(int myType, int myNum)
{
    type = myType;
    num = myNum;
}

ChemEquation::ChemEquation(const char ga[])
{

    setChem(ga);
    toMatrixBaby();
    matrix = new RationalMatrix(baby, myRow, myColume);
}

string ChemEquation::myResult()
{
    string chesult = "";
    for (int k = 0; k < myColume; k++)
    {
        chesult += to_string(matrix->coefficientNum[k]);
        chesult += "  ";
    }
    return chesult;
}

ChemEquation::~ChemEquation()
{
    delete matrix;
    free(chemMine);

    for (int i = 0; i < myRow; i++)
    {
        free(baby[i]);
    }

    free(baby);
    cutString.clear();
    elementList.clear();
    complexResult.clear();
}

int ChemEquation::findEle(string x)
{
    int result = -1;
    int k = 0;

    for (auto val : elementList)
    {
        if (val == x)
        {
            result = k;
            break;
        }

        k++;
    }

    return result;
}

//----------------------------------------parse char array to string list;
void ChemEquation::setChem(const char *chem)
{
    int chemLen = strlen(chem);

    chemMine = (char *)malloc(sizeof(char) * chemLen);

    int chemMineIndex = 0;

    for (int k = 0; k < chemLen; k++)
    {
        if (chem[k] != 32)
        {
            chemMine[chemMineIndex] = chem[k];
            chemMineIndex++;
        }
    }

    chemMine[chemMineIndex] = '\0';
    chemMineLen = strlen(chemMine);
    chemString = string(chemMine);
    elementList.clear();
}

int ChemEquation::convertNumber(int k)
{
    int size = chemMineLen;
    char *num = (char *)malloc(size + 1);
    int numIndex = 0;
    int j = k;
    int ord = k;

    while (j < size)
    {
        int a = chemMine[j];

        if (a >= 48 && a <= 57)
        {
            num[numIndex++] = a;
            j++;
        }
        else
        {
            break;
        }
    }

    if (j == ord)
    {
        free(num);
        return 1;
    }
    else
    {
        num[numIndex] = 0;
        int re = atoi(num);
        free(num);
        return re;
    }
}

void ChemEquation::toList()
{

    int bracketFlag = 0;
    vector<int> bracketItem;

    int chemIntSize = chemMineLen;

    int elementType = 1;

    int lastCutIndex = 0;

    while (chemMine[lastCutIndex] >= 48 && chemMine[lastCutIndex] <= 57)
    {
        lastCutIndex++;
    }

    for (int k = 0; k < chemMineLen; k++)
    {
        char chemMineTemp = chemMine[k];

        if (chemMineTemp == 43)
        {
            cutString.push_back(chemString.substr(lastCutIndex, k + 1 - lastCutIndex));
            lastCutIndex = k + 1;

            while (chemMine[lastCutIndex] >= 48 && chemMine[lastCutIndex] <= 57)
            {
                lastCutIndex++;
            }

            complexResult.push_back(N1);
        }
        else if (chemMineTemp == 61)
        {
            cutString.push_back(chemString.substr(lastCutIndex, k + 1 - lastCutIndex));
            lastCutIndex = k + 1;

            while (chemMine[lastCutIndex] >= 48 && chemMine[lastCutIndex] <= 57)
            {
                lastCutIndex++;
            }

            complexResult.push_back(N2);
        }
        else if (chemMineTemp == 40)
        {
            bracketFlag = 1;
            bracketItem.clear();
        }
        else if (chemMineTemp == 41)
        {
            bracketFlag = false;
            int hol = convertNumber(k + 1);

            for (auto val : bracketItem)
            {
                complexResult[val] = ChemItem(complexResult[val].type, hol * complexResult[val].num);
            }
        }
        else if (chemMineTemp >= 65 && chemMineTemp <= 90)
        {
            string ele;
            int num = 0;

            if (k < chemIntSize - 1)
            {
                if (chemMine[k + 1] >= 97 && chemMine[k + 1] <= 122)
                {
                    num = convertNumber(k + 2);
                    ele = chemString.substr(k, 2);
                }
                else
                {
                    num = convertNumber(k + 1);
                    ele = chemString.substr(k, 1);
                }
            }
            else
            {
                num = convertNumber(k + 1);
                ele = chemString.substr(k, 1);
            }

            int inEle = findEle(ele);

            if (inEle >= 0)
            {
                complexResult.push_back(ChemItem(inEle + 1, num));
            }
            else
            {
                complexResult.push_back(ChemItem(elementType, num));
                elementType++;
                elementList.push_back(ele);
            }

            if (bracketFlag)
            {
                bracketItem.push_back(complexResult.size() - 1);
            }
        }
    }

    cutString.push_back(chemString.substr(lastCutIndex, chemIntSize));
    bracketItem.clear();
}

int ChemEquation::signChem(int x)
{
    if (x > 0)
    {
        return 1;
    }
    else if (x < 0)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

void ChemEquation::toMatrixBaby()
{
    toList();
    vector<int> chemListIndex;

    int m = 0;
    int e = 0;

    chemListIndex.push_back(-1);

    for (int index = 0; index < complexResult.size(); ++index)
    {
        auto val = complexResult[index];

        if (val.type > m)
        {
            m = val.type;
        }

        if (val == N1 || val == N2)
        {
            chemListIndex.push_back(index);
        }

        if (val == N2)
        {
            e = index;
        }
    }

    int n = chemListIndex.size();

    baby = (int **)malloc(sizeof(int *) * m);

    for (int i = 0; i < m; i++)
    {
        baby[i] = (int *)malloc(sizeof(int) * n);

        for (int j = 0; j < n; j++)
        {
            baby[i][j] = 0;
        }
    }

    myRow = m;
    myColume = n;

    chemListIndex.push_back(complexResult.size());

    for (int i = 0; i < n; i++)
    {
        for (int j = chemListIndex[i] + 1; j < chemListIndex[i + 1]; j++)
        {
            baby[complexResult[j].type - 1][i] += signChem(e - j) * complexResult[j].num;
        }
    }

    chemListIndex.clear();
}
