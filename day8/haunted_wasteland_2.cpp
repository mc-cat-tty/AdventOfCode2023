#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <regex>
#include <map>
#include <ranges>
#include <numeric>

using namespace std;
using Fork = pair<string, string>;

auto steps(const string &start, const string &instr, const map<string, Fork> &m) {
  long res = 0;
  string next = start;

  while (*(next.end()-1) != 'Z') {
    for (const auto &i : instr) {
      res++;

      switch (i) {
        case 'R': next = m.at(next).second; break;
        case 'L': next = m.at(next).first; break;
      }

      if (*(next.end()-1) == 'Z') break;
    }
  }

  return res;
}

auto totalSteps(const string &instr, const map<string, Fork> &m) {
  auto startingPoints = m
    | views::keys
    | views::filter([](auto e){ return *(e.end()-1) == 'A'; });

  vector<long> res;
  for (const auto &s : startingPoints) {
    res.emplace_back(steps(s, instr, m));
  }

  return accumulate(res.begin(), res.end(), 1L, [](long acc, long v){return acc*v/gcd(acc, v);});
}

int main() {
  ifstream in("input.txt");
  map<string, Fork> m;
 
  string instr;
  getline(in, instr);

  string buf;
  while (getline(in, buf)) {
    if (buf.empty()) continue;

    smatch sm;
    regex_match(buf, sm, regex("([A-Z\\d]+) = \\(([A-Z\\d]+), ([A-Z\\d]+)\\)"));
    string curr = sm.str(1);
    string l = sm.str(2);
    string r = sm.str(3);

    m[curr] = make_pair(l, r);
  }

  if (not in.flags()) {
    cerr << "Error while reading input" << endl;
    return 1;
  }

  in.close();

  cout << totalSteps(instr, m) << endl;
}