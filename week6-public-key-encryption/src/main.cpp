#include <stdio.h>
#include <string>
#include <iostream>
#include <cmath>
#include <gmpxx.h>
#include "utils/log/log.hpp"


class ModulusFactorBase
{
  protected:
  explicit ModulusFactorBase(const std::string aMpzStr):
    N{aMpzStr}
  {}

  virtual mpz_class factorN() = 0;

  mpz_class getSqrtN()
  {
    mpz_t a;
    mpz_init(a);

    // a=ceil(sqrt(N))
    mpz_sqrt(a, N.get_mpz_t());
    mpz_add_ui(a, a, 1);
    BOOST_LOG_TRIVIAL(debug) << "a=ceil(sqrt(N)) " << a;

    auto retValue = mpz_class(a);
    mpz_clear(a);
    return retValue;
  }

  bool testN(mpz_class _p, mpz_class _q)
  {
    mpz_t n;
    mpz_init(n);
    mpz_mul(n, _p.get_mpz_t(), _q.get_mpz_t());
    BOOST_LOG_TRIVIAL(debug) << "N=pq " << n;
    auto retVal = testN(mpz_class(n));
    mpz_clear(n);
    return retVal;
  }

  bool testN(mpz_class _value)
  {
    return mpz_cmp(N.get_mpz_t(), _value.get_mpz_t()) == 0;
  }

  mpz_class getX(mpz_class _a)
  {
    mpz_t x_sq,x;
    mpz_init(x);
    mpz_init(x_sq);

    // x=sqrt(A^2−N)
    mpz_mul(x_sq, _a.get_mpz_t(), _a.get_mpz_t());
    mpz_sub(x_sq, x_sq, N.get_mpz_t());
    mpz_sqrt(x, x_sq);
    BOOST_LOG_TRIVIAL(debug) << "x=sqrt(A^2−N) " << x;

    auto retValue = mpz_class(x);

    mpz_clear(x);
    mpz_clear(x_sq);

    return retValue;
  }

  std::pair<mpz_class, mpz_class> getPrimes(mpz_class _a, mpz_class _x)
  {
    mpz_t p,q;
    mpz_init(p);
    mpz_init(q);

    // p=a−x
    mpz_sub(p, _a.get_mpz_t(), _x.get_mpz_t());
    BOOST_LOG_TRIVIAL(debug) << "p=a−x " << p;

    // q=a+x
    mpz_add(q, _a.get_mpz_t(), _x.get_mpz_t());
    BOOST_LOG_TRIVIAL(debug) << "q=a+x " << q;

    auto retValue = std::make_pair(mpz_class(p), mpz_class(q));

    mpz_clear(p);
    mpz_clear(q);

    return retValue;
  }

  mpz_class min(mpz_class _l, mpz_class _r)
  {
    return (mpz_cmp(_l.get_mpz_t(), _r.get_mpz_t()) < 0) ? _l : _r; 
  }
  
  const mpz_class N;
};

class ModulusFactor_1 : public ModulusFactorBase
{
  public:
  ModulusFactor_1():
    ModulusFactorBase{"17976931348623159077293051907890247336179769789423065727343008115"
                      "77326758055056206869853794492129829595855013875371640157101398586"
                      "47833778606925583497541085196591615128057575940752635007475935288"
                      "71082364994994077189561705436114947486504671101510156394068052754"
                      "0071584560878577663743040086340742855278549092581"}
  {}



  mpz_class factorN() override final
  {
    auto a = getSqrtN();
    auto x = getX(a);

    auto [p, q] = getPrimes(a, x);

    if (!testN(p, q)) {
      throw std::exception();
    }
    return min(p, q);
  }
};


class ModulusFactor_2 : public ModulusFactorBase
{
  public:
  ModulusFactor_2():
    ModulusFactorBase{"6484558428080716696628242653467722787263437207069762630604390703787"
                      "9730861808111646271401527606141756919558732184025452065542490671989"
                      "2428844841839353281972988531310511738648965962582821502504990264452"
                      "1008852816733037111422964210278402893076574586452336833570778346897"
                      "15838646088239640236866252211790085787877"}
  {}

  mpz_class factorN() override final
  {
    auto sqrtn = getSqrtN();
    mpz_t a, end;

    mpz_init_set(a, sqrtn.get_mpz_t());
    mpz_init(end);
    // A−sqrt(N)<2^20
    mpz_add_ui(end, sqrtn.get_mpz_t(), pow(2, 20));

    do {
      mpz_add_ui(a, a, 1);
      auto x = getX(mpz_class(a));
      auto [p, q] = getPrimes(mpz_class(a), x);
      if (testN(p,q)) {
        BOOST_LOG_TRIVIAL(debug) << "FOUND PRIMES";
        mpz_clear(end);
        mpz_clear(a);
        return min(p, q);
      }
    } while (
      (mpz_cmp(a, end) != 0)
    );

    mpz_clear(end);
    mpz_clear(a);
    throw std::exception();
  }
};


class ModulusFactor_3 : public ModulusFactorBase
{
  public:
  ModulusFactor_3():
    ModulusFactorBase{"72006226374735042527956443552558373833808445147399984182665305798191"
                      "63556901883377904234086641876639384851752649940178970835240791356868"
                      "77441155132015188279331812309091996246361896836573643119174094961348"
                      "52463970788523879939683923036467667022162701835329944324119217381272"
                      "9276147530748597302192751375739387929"}
  {}

  mpz_class factorN() override final
  {
    /*
     *                         sqrt(N)
     * -|----------------------|-|---------------------------|-
     *  p                        A=(p+q)/2                   q
     * p and q are odd, therefore A is integer
     *                         sqrt(6N)
     * -|--|-------------------|-|-----------------------------
     *  p  q                     A=(3p+2q)/2
     * =>
     *                         sqrt(6N)
     * -|----------------------|-|----------------------------|-
     *  3p                       A=(3p+2q)/2                 2q
     * (3p+2q) might be odd, therefore multiply by two
     *                         2sqrt(6N)=sqrt(24N)
     * -|----------------------|-|----------------------------|-
     *  6p                       A=(6p+4q)/2                 4q
     * (6p+4q) is even
     */
    auto a = get2Sqrt6N();
    auto x = getSqrtApow2Sub24N(a);

    auto [p6, q4] = getPrimes(a, x);

    mpz_t p,q;
    mpz_init(p);
    mpz_init(q);

    mpz_div_ui(p, p6.get_mpz_t(), 6);
    mpz_div_ui(q, q4.get_mpz_t(), 4);

    BOOST_LOG_TRIVIAL(debug) << "p " << p;
    BOOST_LOG_TRIVIAL(debug) << "q " << q;

    if (!testN(mpz_class(p), mpz_class(q))) {
      throw std::exception();
    }

    auto retValue = min(mpz_class(p), mpz_class(q));
    mpz_clear(p);
    mpz_clear(q);

    return retValue;
  }

  private:
    mpz_class get2Sqrt6N()
  {
    mpz_t a;
    mpz_init(a);

    // a=ceil(2sqrt(6N)) => a=ceil(sqrt(24N))
    mpz_mul_ui(a, N.get_mpz_t(), 24);
    mpz_sqrt(a, a);
    mpz_add_ui(a, a, 1);
    BOOST_LOG_TRIVIAL(debug) << "a=ceil(2sqrt(6N)) " << a;

    auto retValue = mpz_class(a);
    mpz_clear(a);
    return retValue;
  }

  mpz_class getSqrtApow2Sub24N(mpz_class _a)
  {
    mpz_t x_sq,x,n;
    mpz_init(x);
    mpz_init(n);
    mpz_init(x_sq);

    // x=sqrt(A^2−24N)
    mpz_mul(x_sq, _a.get_mpz_t(), _a.get_mpz_t());
    mpz_mul_ui(n, N.get_mpz_t(), 24);
    mpz_sub(x_sq, x_sq, n);
    mpz_sqrt(x, x_sq);
    BOOST_LOG_TRIVIAL(debug) << "x=sqrt(A^2−24N) " << x;

    auto retValue = mpz_class(x);

    mpz_clear(x);
    mpz_clear(x_sq);
    mpz_clear(n);

    return retValue;
  }
};

int main(void)
{
  Logger::getInstance().init();

  ModulusFactor_1 mf1;
  auto r1 = mf1.factorN();

  ModulusFactor_2 mf2;
  auto r2 = mf2.factorN();
  
  ModulusFactor_3 mf3;
  auto r3 = mf3.factorN();

  BOOST_LOG_TRIVIAL(debug)
  << "\nResults:\n"
  << "\t(1): " << r1 << "\n"
  << "\t(2): " << r2 << "\n"
  << "\t(3): " << r3 << "\n"
  << "\n";


  return 0;
}
