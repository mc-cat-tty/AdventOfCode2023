#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <set>

constexpr long EXPANSION_FACTOR = 1e6;

using namespace std;

struct Coord {
  long r;
  long c;

  bool operator== (const Coord &o) const {
    return r == o.r and c == o.c;
  }
};


auto expand(const vector<string> &uni, set<long> &expRows, set<long> &expCols) {
  for (long r=0; r<uni.size(); r++) {
    if (ranges::find(uni[r], '#') != uni[r].end()) continue;

    expRows.insert(r);
  }

  for (long c = 0; c<uni[0].size(); c++) {
    bool noGal = true;
    for (long r = 0; r<uni.size(); r++) {
      if (uni[r][c] == '#') {
        noGal = false;
        break;
      }
    }
    if (noGal) expCols.insert(c);
  }
}

auto galaxies(const vector<string> &univ) {
  vector<Coord> res;
  for (long i = 0; const auto &row : univ) {
    for (long j = 0; const auto &ch : row) {
      if (ch == '#') res.emplace_back(Coord{i, j});
      j++;
    }
    i++;
  }
  return res;
}

auto crossed(const set<long> &eR, const set<long> &eC, Coord src, Coord dst) {
  long rows = 0;
  for (const auto &er : eR) {
    if (er > min(src.r, dst.r) and er < max(dst.r, dst.r)) rows++;
  }

  long cols = 0;
  for (const auto &ec : eC) {
    if (ec > min(src.c, dst.c) and ec < max(src.c, dst.c)) cols++;
  }

  return make_pair(rows, cols);
}

auto sumShortestPaths(vector<string> univ) {
  set<long> eR, eC;  // expandedRows, expandedColumns
  expand(univ, eR, eC);
  // ranges::for_each(eR, [](const auto &e){cout << e << " " << endl;}); cout << endl;
  // ranges::for_each(eC, [](const auto &e){cout << e << " " << endl;}); cout << endl;

  auto gv = galaxies(univ);
  // ranges::for_each(gv, [](const auto &e){cout << e.r << " " << e.c << endl;});

  long res = 0;
  for (long i = 0; i<gv.size(); i++) {
    Coord gs = gv[i];

    for (long j=i+1; j<gv.size(); j++) {
      Coord gd = gv[j];
      // cout << i << " " << j << endl;

      auto [crossedR, crossedC] = crossed(eR, eC, gs, gd); 
      // cout << crossedR << " x " << crossedC << endl;
      
      long dist =
        abs(gd.r - gs.r) + crossedR * (EXPANSION_FACTOR-1) +
        abs(gd.c - gs.c) + crossedC * (EXPANSION_FACTOR-1);
      
      res += dist;
      // cout << i << " " << j << " = " << dist << endl;
    }
  }

  return res;
}

int main() {
  ifstream in("input.txt");
  vector<string> m;
  string buf;

  while (getline(in, buf)) {
    m.emplace_back(buf);
  }

  if (not in.flags()) {
    cerr << "Error while reading input" << endl;
    return 1;
  }

  in.close();

  cout << sumShortestPaths(m) << endl;
}