#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

struct Coord {
  int r;
  int c;

  bool operator== (const Coord &o) const {
    return r == o.r and c == o.c;
  }
};

auto expand(vector<string> &uni) {
  for (int r=0; r<uni.size(); r++) {
    if (ranges::find(uni[r], '#') != uni[r].end()) continue;

    uni.insert(uni.begin() + r, uni[r]);
    r++;
  }

  for (int c = 0; c<uni[0].size(); c++) {
    bool noGal = true;
    for (int r = 0; r<uni.size(); r++) {
      if (uni[r][c] == '#') {
        noGal = false;
        break;
      }
    }
    if (noGal) {
      for (auto &r : uni) {
        r.insert(r.begin() + c, '.');
      }
      c++;
    }
  }
}

auto galaxies(const vector<string> &univ) {
  vector<Coord> res;
  for (int i = 0; const auto &row : univ) {
    for (int j = 0; const auto &ch : row) {
      if (ch == '#') res.emplace_back(Coord{i, j});
      j++;
    }
    i++;
  }
  return res;
}

auto sumShortestPaths(vector<string> univ) {
  expand(univ);
  // ranges::for_each(univ, [](const auto &r){
  //   ranges::for_each(r, [](const auto &c){ cout << c; });
  //   cout << endl;
  // });

  auto gv = galaxies(univ);
  // ranges::for_each(gv, [](const auto &e){cout << e.r << " " << e.c << endl;});

  int res = 0;
  for (int i = 0; i<gv.size(); i++) {
    Coord gs = gv[i];

    for (int j=i+1; j<gv.size(); j++) {
      Coord gd = gv[j];
      res += abs(gs.r - gd.r) + abs(gs.c - gd.c);  // Manhattan distance
      // cout << i << " " << j << " = " << abs(gs.r - gd.r) + abs(gs.c - gd.c) << endl;
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