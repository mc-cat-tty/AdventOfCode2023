#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <regex>

using namespace std;
struct ElfMapEntry {
  long d;  // destination start
  long s;  // source start
  long r;  // range
};
using ElfMap = vector<ElfMapEntry>;

auto accessMaps(long key, const vector<ElfMap> &maps) {
  long res = key;
  for (const auto &m : maps) {
    for (const auto &e : m) {
      long offset = res - e.s;
      if (offset >= 0 and offset < e.r) {
        res = e.d + offset;
        break;
      }
    }
  }
  return res;
}

long lowestLocation(const vector<long> &seeds, const vector<ElfMap> &maps) {
  // ranges::for_each(seeds, [](auto e) {cout << e << " " << endl;});
  // ranges::for_each(maps, [](auto l) {
  //   ranges::for_each(l, [](auto e){cout << e.d << " " << e.s << " " << e.r << " - ";});
  //   cout << endl;
  // });

  long sol = numeric_limits<long>::max();

  for (const auto &s : seeds) {
    sol = min(accessMaps(s, maps), sol);
  }

  return sol;
}

int main() {
  vector<long> seeds;
  vector<ElfMap> maps;

  ifstream in("input.txt");
  string buf;

  smatch m;
  getline(in, buf);
  regex_match(buf, m, regex("seeds: ([\\d+ ]+)"));
  stringstream ss(m[1]);
  while (ss >> buf) {
    seeds.emplace_back(stol(buf));
  }

  ElfMap tmp;
  regex re(R"(^[\d+ ]+$)");

  while (getline(in, buf)) {
    if (buf.empty()) continue;

    bool ok = regex_match(buf, re);

    if (not ok) {
      maps.emplace_back(tmp);
      tmp.clear();
    }
    else {
      stringstream ss(buf);
      long d, s, r;
      ss >> d >> s >> r;
      tmp.emplace_back(ElfMapEntry{d, s, r});
    }
  }

  maps.emplace_back(tmp);
  maps.erase(maps.begin());

  if (not in.flags()) {
    cerr << "Error while reading input" << endl;
    return 1;
  }

  in.close();

  cout << lowestLocation(seeds, maps) << endl;
}