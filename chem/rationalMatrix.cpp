#include "rationalMatrix.h"




int RationalMatrix::absInt(int x)
{
    if(x < 0)
    {
        return -x;
    }
    else
    {
        return x;
    }
}



RationalMatrix::RationalMatrix(int ** chem, int myRow, int myColumn)
{
    row = myRow;
    colume = myColumn;
    rationalMatrix = (RationalNumber **)malloc(sizeof(RationalNumber*)*row);

    for (int k = 0; k < row; k++)
    {
        rationalMatrix[k] = (RationalNumber * ) malloc(sizeof(RationalNumber) * colume);

        for (int j = 0; j < colume; j++)
        {
            rationalMatrix[k][j].numerator = chem[k][j];
            rationalMatrix[k][j].denominator = 1;
        }
    }
    coefficientArray();
}



RationalMatrix::~RationalMatrix()
{
    for (int k = 0; k < row; k++)
    {
        free(rationalMatrix[k]);
    }

    free(rationalMatrix);
    free(coefficientNum);

}


void RationalMatrix::swapRow(int i, int j)
{
    if (i == j)
    {
        return;
    }

    RationalNumber temp;

    for (int k = 0; k < colume; k++)
    {
        temp.set(& rationalMatrix[i][k]);
        rationalMatrix[i][k].set(& rationalMatrix[j][k]);
        rationalMatrix[j][k].set(& temp);
    }
}


void RationalMatrix::reduceRowItself(int baseRow)
{
    RationalNumber temp;
    temp.set(  & (rationalMatrix[baseRow][baseRow]));
    temp.inv();

    for (int k = 0; k < colume; k++)
    {
        (rationalMatrix[baseRow][k]).multiply(  & temp);
    }
}


void RationalMatrix::reduceRow(int baseRow, int secondRow)
{
    RationalNumber temp;
    temp.set(& (rationalMatrix[secondRow][baseRow]));

    for (int k = 0; k < colume; k++)
    {
        RationalNumber baseRowTemp;
        baseRowTemp.set( & (rationalMatrix[baseRow][k]));
        baseRowTemp.multiply( & temp);
        baseRowTemp.strains();
        rationalMatrix[secondRow][k].add( &baseRowTemp);
    }
}


void RationalMatrix::rref()
{
    for (int i = 0; i < row; i++)
    {
        for (int j = i; j < row; j++)
        {
            if (!rationalMatrix[j][i].isZero())
            {
                swapRow(j, i);
                reduceRowItself(i);
                break;
            }
        }

        for (int j = i + 1; j < row; j++)
        {
            reduceRow(i, j);
        }
    }

    for (int i = (row - 1); i >= 1; i--)
    {
        for (int j = (i - 1); j >= 0; j--)
        {
            reduceRow(i, j);
        }
    }
}



int RationalMatrix::gcd(int np, int dp)
{
    int npt = absInt(np);
    int dpt = absInt(dp);
    int remainder = dpt % npt;
    int temp = 0;

    while (remainder != 0)
    {
        temp = npt % remainder;
        npt = remainder;
        remainder = temp;
    }

    return npt;
}

int RationalMatrix::lcm(int np, int dp)
{
    return np * dp / gcd(np, dp);
}


void RationalMatrix::coefficientArray()
{
    rref();
    int * deu;
    coefficientNum = (int * ) malloc(sizeof(int) * (row + 1));
    deu = (int * ) malloc(sizeof(int) * (row + 1));
    int dlcm = 1;

    for (int k = 0; k < row; k++)
    {
        dlcm = lcm(absInt(rationalMatrix[k][colume - 1].denominator), dlcm);
        coefficientNum[k] = absInt(rationalMatrix[k][colume - 1].numerator);
        deu[k] = absInt(rationalMatrix[k][colume - 1].denominator);
    }

    coefficientNum[row] = 1;
    deu[row] = 1;

    for (int k = 0; k < colume; k++)
    {
        coefficientNum[k] *= (dlcm / deu[k]);
    }

    int ngcd = 1;

    for (int k = 0; k < colume; k++)
    {
        ngcd = gcd(ngcd, coefficientNum[k]);
    }

    for (int k = 0; k < colume; k++)
    {
        coefficientNum[k] /= ngcd;
    }

    free(deu);
}