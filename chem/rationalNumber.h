#pragma once

class RationalNumber
{
public:
    int numerator;
    int denominator;
    void set(RationalNumber *b);
    void inv();
    void add( RationalNumber *b);
    void multiply( RationalNumber *b);
    void strains();
    int isZero();
    int absInt(int x);

private:
    void simplify(RationalNumber *x);
};