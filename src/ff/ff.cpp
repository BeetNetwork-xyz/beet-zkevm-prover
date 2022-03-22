#include "ff.hpp"

uint64_t FiniteField::add (uint64_t a, uint64_t b)
{
    mpz_class result = ( mpz_class(a) + mpz_class(b) ) % mpz_class(p);
    return result.get_ui();
}

uint64_t FiniteField::sub (uint64_t a, uint64_t b)
{
    mpz_class result = ( mpz_class(p) + mpz_class(a) - mpz_class(b) ) % mpz_class(p);
    return result.get_ui();
}   

uint64_t FiniteField::neg (uint64_t a)
{
    mpz_class result = ( mpz_class(p) - mpz_class(a) ) % mpz_class(p);
    return result.get_ui();
}

uint64_t FiniteField::mul (uint64_t a, uint64_t b)
{
    mpz_class result = ( mpz_class(a) * mpz_class(b) ) % mpz_class(p);
    return result.get_ui();
}

/* Extended Euclidean algorithm */
uint64_t FiniteField::inv (uint64_t a)
{
    if (a==0)
    {
        cerr << "FiniteField::inv() called with zero" << endl;
        exit(-1);
    }
    mpz_class t = 0;
    mpz_class r = p;
    mpz_class newt = 1;
    mpz_class newr = a % p;
    mpz_class q;
    mpz_class aux1;
    mpz_class aux2;
    while (newr != 0)
    {
        q = r/newr;
        aux1 = t;
        aux2 = newt;
        t = aux2;
        newt = aux1 - q*aux2;
        aux1 = r;
        aux2 = newr;
        r = aux2;
        newr = aux1 - q*aux2;
    }
    if (t<0)
    {
        t += p;
    }
    return t.get_ui();
}

uint64_t FiniteField::div (uint64_t a, uint64_t b)
{
    return mul(a, inv(b));
}

void FiniteField::check (bool condition)
{
    if (!condition)
    {
        cerr << "Error: FiniteFields::check() failed" << endl;
        exit(-1);
    }
}

void FiniteField::test (void)
{
    uint64_t a, b, r;

    a = 5;
    b = 10;
    r = add(a, b);
    check(r==15);

    a = 10;
    b = p - 5;
    r = add(a, b);
    check(r==5);

    a = 10;
    b = 5;
    r = sub(a, b);
    check(r==5);

    a = 10;
    b = p - 5;
    r = sub(a, b);
    check(r==15);

    a = 5;
    r = neg(a);
    check(r==(p-5));

    a = p - 5;
    r = neg(a);
    check(r==5);

    a = 5;
    b = 10;
    r = mul(a, b);
    check(r==50);

    a = p - 5;
    b = p - 10;
    r = mul(a, b);
    check(r==50);

    a = 5;
    b = inv(a);
    r = mul(a, b);
    check(r==1);

    a = 50;
    b = 5;
    r = div(a, b);
    check(r==10);
}