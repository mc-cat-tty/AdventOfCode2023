#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <regex>

using namespace std;
using Race = pair<int, int>;

inline auto distance(int buttonCharge, int time) {
  return buttonCharge*time;
}

int winRace(vector<Race> races) {
  int res = 1;

  for (const auto &[totalTime, recordDistance] : races) {
    int count = 0;
    for (int chargeTime=0; chargeTime<totalTime; chargeTime++) {
      count += distance(chargeTime, totalTime-chargeTime) > recordDistance ? 1 : 0;
    }
    res *= count;
  }

  return res;
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

  vector<Race> v;
  int t, d;
  while (ss1 >> t and ss2 >> d) {
    v.emplace_back(make_pair(t, d));
  }

  if (not in.flags()) {
    cerr << "Error while reading input" << endl;
    return 1;
  }

  in.close();

  cout << winRace(v) << endl;
}