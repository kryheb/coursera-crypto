#include <stdio.h>
#include <string>
#include <iostream>
#include <cmath>
#include <gmpxx.h>
#include "utils/log/log.hpp"

/*
 Your goal this week is to write a program to compute discrete log modulo a prime `p`.
 Let ggg be some element in Zp∗ and suppose you are given `h` in Zp∗ such that h=g^x where 1≤x≤2^40.
 Your goal is to find `x`. More precisely, the input to your program is p,g,h and the output is x.

The trivial algorithm for this problem is to try all 2^40 possible values of x until the correct one is found,
that is until we find an x satisfying h=g^x in Zp. This requires 2^40 multiplications.
In this project you will implement an algorithm that runs in time roughly 2^20 using a meet in the middle attack.

Let B=2^20. Since x is less than B^2 we can write the unknown x base B as x=x0*B+x1 where x0,x1 are in the range [0,B−1].
Then

h = g^x = g^(x0B+x1) = (g^B)^x0⋅g^x1 in Zp.

By moving the term g^x1 the other side we obtain

  h/(g^x1) = (g^B)^x0 in Zp.

The variables in this equation are x0,x1 everything else is known: you are given g,h and B=2^20.
Since the variables x0 and x1 are now on different sides of the equation we can find a solution using meet in the middle (Lecture 3.3 at 14:25):

  * First build a hash table of all possible values of the left hand side h/g^x1 for x1 = 0,1,…,2^20.
  * Then for each value x0 = 0,1,2,…,2^20 check if the right hand side (g^B)^x0 in this hash table.
    If so, then you have found a solution (x0,x1) from which you can compute the required x as x=x0B+x1. 

The overall work is about 2^20 multiplications to build the table and another 2^20 lookups in this table.

Now that we have an algorithm, here is the problem to solve:

p=134078079299425970995740249982058461274793658205923933 \
  77723561443721764030073546976801874298166903427690031 \
  858186486050853753882811946569946433649006084171
g=11717829880366207009516117596335367088558084999998952205 \
  59997945906392949973658374667057217647146031292859482967 \
  5428279466566527115212748467589894601965568
h=323947510405045044356526437872806578864909752095244 \
  952783479245297198197614329255807385693795855318053 \
  2878928001494706097394108577585732452307673444020333

Each of these three numbers is about 153 digits. Find x such that h=g^x in Zp.
 */

class Solver
{
  mpz_class p { "134078079299425970995740249982058461274793658205923933"
                "77723561443721764030073546976801874298166903427690031"
                "858186486050853753882811946569946433649006084171" };
  mpz_class g { "11717829880366207009516117596335367088558084999998952205"
                "59997945906392949973658374667057217647146031292859482967"
                "5428279466566527115212748467589894601965568" };
  mpz_class h { "323947510405045044356526437872806578864909752095244"
                "952783479245297198197614329255807385693795855318053"
                "2878928001494706097394108577585732452307673444020333" };

  static constexpr double b{pow(2,20)};
  std::map<mpz_class, double> hashTable;

  public:
  auto getB() { return b; }

  void buildHashTable()
  {
    BOOST_LOG_TRIVIAL(debug) << "Build has table";
    for (auto i=0; i<=b; ++i) {
      mpz_t exp, divisor, result;
      mpz_init(divisor);
      mpz_init(result);
      mpz_init_set_ui(exp, i);
      // g^x mod p
      mpz_powm(divisor, g.get_mpz_t(), exp, p.get_mpz_t());
      // (h/(g^x)) mod p
      hashTable.insert({moduloDivide(h, mpz_class(divisor), p), i});
      
      mpz_clear(exp);
      mpz_clear(divisor);
      mpz_clear(result);
    }
    BOOST_LOG_TRIVIAL(debug) << "Done";
  }

  std::pair<double, double> findSolution()
  {
    BOOST_LOG_TRIVIAL(debug) << "Begin lookup";
    for (auto i=0; i<=b; ++i) {
      mpz_t exp, result;
      mpz_init(result);
      mpz_init_set_ui(exp, i);
      // exponent x*b
      mpz_mul_ui(exp, exp, b);
      // g^exp mod p
      mpz_powm(result, g.get_mpz_t(), exp, p.get_mpz_t());

      auto it = hashTable.find(mpz_class(result));
      // found
      if (it != hashTable.end()) {
        mpz_clear(exp);
        mpz_clear(result);
        BOOST_LOG_TRIVIAL(debug) << "Result found";
        // return a, b
        return {i, it->second};
      }

      mpz_clear(exp);
      mpz_clear(result);
    }
    // Solution not found -> throw
    throw std::exception();
  }


  static mpz_class moduloDivide(mpz_class _divident, mpz_class _divisor, mpz_class _modulo)
  {
    mpz_t result, inv;
    mpz_init(result);
    mpz_init(inv);
    mpz_mod(result, _divident.get_mpz_t(), _modulo.get_mpz_t());

    int res = mpz_invert(inv, _divisor.get_mpz_t(), _modulo.get_mpz_t());
    if (!res) {
      // no invert
      throw std::exception();
    }

    mpz_mul(result, result, inv);
    mpz_mod(result, result, _modulo.get_mpz_t());

    auto returnValue = mpz_class(result);

    mpz_clear(result);
    mpz_clear(inv);
    return returnValue;
  }
};

int main(void)
{
  Logger::getInstance().init();
  Solver s;
  s.buildHashTable();
  auto res = s.findSolution();
  BOOST_LOG_TRIVIAL(debug) << (uint64_t)res.first * (uint64_t)s.getB() + (uint64_t)res.second;

  return 0;
}
