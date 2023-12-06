#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <regex>
#include <cmath>

using namespace std;

inline auto targetFunCount(long totalTime, long recordDistance) {
  /*
  Formula explaination
  distance(charge) = time * vel = time * charge = time * (totalTime - charge) * charge = Tc - c^2
  g(charge) = distance(charge) + recordDistance = Tc - c^2 - D
  g(charge) > 0 -> Tc - c^2 - D > 0
  Intersection at c1 = (T - sqrt(T^2 - 4D) ) / 2, c2 = (T + sqrt(T^2 - 4D) ) / 2
  Interval size? approx. c2 - c1 + 1, but c1 and c2 live in real numbers, while the solution in natural numbers
  So... if c1 and c2 must be approximated to, respectively, the upper integer and lower integer
  But... if c1 and c2 were already integers, they must be discarded since the inequality states g(c) greater than zero (and not greater or equal)
  */
  double charge1 = (totalTime - sqrt(totalTime*totalTime - 4*recordDistance)) / 2;
  double charge2 = (totalTime + sqrt(totalTime*totalTime - 4*recordDistance)) / 2;
  long charge1Int = ceil(charge1);
  long charge2Int = floor(charge2);

  // Intersection point between y = recordDistance and y = Tc - c^2 are excluded
  if (charge1Int == charge1) charge1Int++;
  if (charge2Int == charge2) charge2Int--;

  return charge2Int - charge1Int + 1;
}

int main() {
  ifstream in("input.txt");

  string buf1;
  smatch sm1;
  getline(in, buf1);
  bool ok1 = regex_match(buf1, sm1, regex("[A-Za-z]+:([\\d+ ]+)"));
  stringstream ss1(sm1.str(1));
  
  string buf2;
  smatch sm2;
  getline(in, buf2);
  bool ok2 = regex_match(buf2, sm2, regex("[A-Za-z]+:([\\d+ ]+)"));
  stringstream ss2(sm2.str(1));

  if (not ok1 || not ok2) {
    return 1;
  }

  string t, d, _t, _d;
  while (ss1 >> _t and ss2 >> _d) {
    t += _t;
    d += _d;
  }

  if (not in.flags()) {
    cerr << "Error while reading input" << endl;
    return 1;
  }

  in.close();

  cout << targetFunCount(stol(t), stol(d)) << endl;
}