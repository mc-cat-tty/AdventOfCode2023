#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <regex>

using namespace std;
using Race = pair<long, long>;

inline auto distance(long buttonCharge, long time) {
  return buttonCharge*time;
}

long winRace(Race race) {
  long totalTime = race.first;
  long recordDistance = race.second;
  long count = 0;
  
  for (long chargeTime=0; chargeTime<totalTime; chargeTime++) {
    count += distance(chargeTime, totalTime-chargeTime) > recordDistance ? 1 : 0;
  }

  return count;
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

  cout << winRace(make_pair(stol(t), stol(d))) << endl;
}