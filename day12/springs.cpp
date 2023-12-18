#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <regex>
#include <iterator>
#include <ranges>

using namespace std;

struct Line {
  string row;
  vector<int> dmg;  // damaged vector

  friend ostream& operator<<(ostream &o, const Line &l) {
    o << l.row << " - ";
    ranges::for_each(l.dmg, [&o](auto b){o << b << ", ";});
    return o;
  }
};

bool check(Line l) {
  auto s = l.row
    | views::split('.')
    | views::transform([](auto e){ return e.size(); })
    | views::filter([](auto c){ return c != 0; });
  
  auto it = l.dmg.begin();
  for (const auto &c : s) {
    if (c != *it) return false;
    it++;
  }

  if (it != l.dmg.end()) return false;
  return true;
}

// crappy complete search
int arrangements(Line l, int idx) {
  if (idx > l.row.size()) return check(l);
  if (l.row[idx] != '?') return arrangements(l, idx+1);

  l.row[idx] = '.';
  int opIdxCount = arrangements(l, idx+1);

  l.row[idx] = '#';
  int dmgIdxCount = arrangements(l, idx+1);

  return opIdxCount + dmgIdxCount;
}

int totalArrangements(const vector<Line> &lv) {
  return accumulate(
    lv.begin(), lv.end(), 0,
    [](const auto &acc, const auto &e){return acc + arrangements(e, 0);}
  );
}

int main() {
  ifstream in("input.txt");
  vector<Line> lines;
  string buf;

  while (getline(in, buf)) {
    smatch sm;
    regex_match(buf, sm, regex("^([\\.#\\?]+) ([\\d,]+)$"));
    
    auto sv = sm.str(2) | views::transform([](auto c){return c == ',' ? ' ' : c;});
    string s;
    ranges::copy(sv, back_inserter(s));
    stringstream ss(s);
    lines.emplace_back(sm.str(1), vector(istream_iterator<int>(ss), istream_iterator<int>()));
  }

  if (not in.flags()) {
    cerr << "Error while reading input" << endl;
    return 1;
  }

  in.close();

  cout << totalArrangements(lines) << endl;
}