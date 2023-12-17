#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <regex>
#include <iterator>
#include <ranges>
#include <map>

using namespace std;

using namespace std;
using Cache = map<string, long>;
struct Line {
  string row;
  vector<int> dmg;  // damaged vector

  friend ostream& operator<<(ostream &o, const Line &l) {
    o << l.row << " - ";
    ranges::for_each(l.dmg, [&o](auto b){o << b << ", ";});
    return o;
  }
};

bool check(const Line &l) {
  auto lastBlock = ranges::find(l.row, '?');

  auto s = l.row
    | views::take(lastBlock - l.row.begin())
    | views::split('.')
    | views::transform([&](auto e) mutable { return e.size(); })
    | views::filter([](auto c){ return c != 0; })
    | views::common;

  if (lastBlock != l.row.end()) {
    auto it = s.begin();
    auto dist = distance(s.begin(), s.end());
    if (dist > l.dmg.size()) return false;

    for (int i=0; i < dist - 1; i++, it++) {
      if (l.dmg[i] != *it) return false;
    }
    return true;
  }
  
  auto it = l.dmg.begin();
  for (const auto &c : s) {
    if (c != *it) return false;
    it++;
  }

  if (it != l.dmg.end()) return false;
  return true;
}

string minimize(const string &s) {
  string res;
  char prev = 'x';
  for (const auto &c : s) {
    if (prev != c or c != '.') res += c;

    prev = c;
  }
  return res;
}

long arrangements(Line l, Cache &c) {
  // memoization
  string min = minimize(l.row);
  if (c.count(min)) return c.at(min);

  auto it = ranges::find(min, '?');
  if (it == min.end()) return check(l);
  if (not check(l)) return 0;  // infeasable

  *it = '.';
  l.row = min;
  long opIdxCount = arrangements(l, c);

  *it = '#';
  l.row = min;
  long dmgIdxCount = arrangements(l, c);

  *it = '?';
  c[min] = opIdxCount + dmgIdxCount;
  return opIdxCount + dmgIdxCount;
}

long totalArrangements(vector<Line> lv) {
  for (auto &l : lv) {
    string tmpR = l.row;
    vector tmpV = l.dmg;
    for (const auto &_ : views::iota(0, 2)) {  // exp increasing length (2**2)
      tmpR += '?' + tmpR;
      tmpV.insert(tmpV.end(), tmpV.begin(), tmpV.end());
    }
    l.row += '?' + tmpR;
    l.dmg.insert(l.dmg.end(), tmpV.begin(), tmpV.end());
  }

  // ranges::for_each(lv, [](const auto &l){cout << l << endl;});

  return accumulate(
    lv.begin(), lv.end(), 0L,
    [](const auto &acc, const auto &e){auto c = Cache{}; return acc + arrangements(e, c);}
  );
  return 0;
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