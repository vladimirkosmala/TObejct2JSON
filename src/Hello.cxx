#include <iostream>
#include "TRandom.h"
#include "TF1.h"

using namespace std;

void test()
{
  TRandom *rnd = new TRandom(time(0));
  Double_t x = rnd->Rndm();
  cout << "x = " << x << endl;
  TF1 *t = new TF1("dupa", "x+3", 0, 10);
  t->Draw();
}

# ifndef __CINT__
int main()
{
  test();
  return 0;
}
# endif
