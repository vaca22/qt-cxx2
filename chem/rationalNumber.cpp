#include "rationalNumber.h"

int RationalNumber::absInt(int x)
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




//--------------a=b--------------------------b->a;
void RationalNumber::set(  RationalNumber *b)
{
    numerator = b->numerator;
    denominator = b->denominator;
}


void RationalNumber::simplify( RationalNumber* x)
{
    if(x->numerator == 0)
    {
        x->denominator = 1;
        return;
    }

    int np = absInt(x->numerator);
    int dp = absInt(x->denominator);
    int remainder = dp % np;
    int temp = 0;

    while(remainder != 0)
    {
        temp = np % remainder;
        np = remainder;
        remainder = temp;
    }

    x->numerator /= np;
    x->denominator /= np;
}

//-------------------------a+=b;
void RationalNumber::add(RationalNumber *b)
{
   numerator = (numerator) * (b->denominator) + (b->numerator) * (denominator);
    (denominator) *= (b->denominator);
    simplify(this);
}




void RationalNumber::multiply(  RationalNumber *b)
{
    (numerator) *= (b->numerator);
    (denominator) *= (b->denominator);
    simplify(this);
}

void RationalNumber::inv()
{
    int temp = numerator;
    numerator = denominator;
    denominator = temp;
}


void RationalNumber::strains()
{
    numerator = -numerator;
}


int RationalNumber::isZero( )
{
    if(numerator == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


