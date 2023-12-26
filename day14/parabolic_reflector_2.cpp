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
constexpr long CYCLES = 1e9;
using Platform = vector<string>;

auto totalWeight(Platform p) {
  int res = 0;
  for (int c=0; c<p.at(0).size(); c++) {
    for (int r=0; r<p.size(); r++) {
      if (p[r][c] == 'O') res += p.size() - r;  
    }
  }
  return res;
}

auto rot90(const Platform &p) {
  Platform res;
  for (int i=0; i<p.at(0).size(); i++) {
    string tmp;
    for (auto it = p.rbegin(); it != p.rend(); it++)
      tmp += (*it).at(i);
    res.emplace_back(tmp);
  }
  return res;
}

void moveRocks(string &s) {
  long count = 0;
  auto dim = s.size();

  int f = 0, l = 0;  // first
  for (; l<dim; l++) {  // last
    if (s[l] == 'O') count++;
    if (s[l] == '#') {
      s.replace(f, l-f, string(l-f, '.'));
      s.replace(l-count, count, string(count, 'O'));
      f = l+1;
      count = 0;
    }
  }
  s.replace(f, l-f, string(l-f, '.'));
  s.replace(l-count, count, string(count, 'O'));
}

auto cycle(Platform p) {
  for (const auto &_ : views::iota(0, 4)) {
    p = rot90(p);
    for (auto &l : p) moveRocks(l);
  }
  return p;
}

auto cycles(const Platform &p) {
  vector<Platform> visitedStates;
  Platform curr = p;
  for (int i=0; i<CYCLES and ranges::find(visitedStates, curr) == visitedStates.end(); i++) {
    visitedStates.emplace_back(curr);
    curr = cycle(curr);
  }

  auto loopStart = ranges::find(visitedStates, curr);
  auto loopEnd = visitedStates.end();
  auto loopLen = distance(loopStart, loopEnd);
  auto size = visitedStates.size() - 1;  // -1 cause first doesn't count
  auto remaining = CYCLES - size;
  return *(loopStart + remaining%loopLen - 1);
}

int main() {
  ifstream in("input.txt");
  Platform platform;
  string buf;

  while (getline(in, buf)) {
    platform.push_back(buf);
  }

  if (not in.flags()) {
    cerr << "Error while reading input" << endl;
    return 1;
  }

  in.close();

  cout << totalWeight(cycles(platform)) << endl;
}