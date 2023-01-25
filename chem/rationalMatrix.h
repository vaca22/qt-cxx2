#pragma once
#include "rationalNumber.h"
#include <cstdlib>

class RationalMatrix
{
public:
    int *coefficientNum;

    RationalMatrix(int **chem, int myRow, int myColumn);
    ~RationalMatrix();

private:
    int row = 0;
    int colume = 0;
    int absInt(int x);
    int lcm(int np, int dp);
    int gcd(int np, int dp);
    void swapRow(int i, int j);
    void reduceRowItself(int baseRow);
    void reduceRow(int baseRow, int secondRow);
    void rref();
    void coefficientArray();
    RationalNumber **rationalMatrix;
};
