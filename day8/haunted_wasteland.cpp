#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <regex>
#include <map>

using namespace std;
using Fork = pair<string, string>;

auto steps(const string &instr, const map<string, Fork> &m) {
  int res = 0;
  string next = "AAA";

  while (next != "ZZZ") {
    for (const auto &i : instr) {
      res++;

      switch (i) {
        case 'R': next = m.at(next).second; break;
        case 'L': next = m.at(next).first; break;
      }

      if (next.back() == 'Z') break;
    }
  }

  return res;
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
    regex_match(buf, sm, regex("([A-Z]+) = \\(([A-Z]+), ([A-Z]+)\\)"));
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

  cout << steps(instr, m) << endl;
}