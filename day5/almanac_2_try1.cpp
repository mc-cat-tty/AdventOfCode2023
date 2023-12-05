#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <regex>
#include <map>
#include <execution>
#include <ranges>

using namespace std;
struct ElfMapEntry {
  long d;  // destination start
  long s;  // source start
  long r;  // range
};
using ElfMap = vector<ElfMapEntry>;

map<long, long> cache;

inline auto accessMaps(long key, const vector<ElfMap> &maps) {
  try {
    return cache.at(key);
  }
  catch (exception e) {
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
    cache[key] = res;
    return res;
  }
}

long lowestLocation(const vector<pair<long, long>> &seeds, const vector<ElfMap> &maps) {
  // ranges::for_each(seeds, [](auto e) {cout << e << " " << endl;});
  // ranges::for_each(maps, [](auto l) {
  //   ranges::for_each(l, [](auto e){cout << e.d << " " << e.s << " " << e.r << " - ";});
  //   cout << endl;
  // });

  long sol = numeric_limits<long>::max();

  for (const auto &[s, len] : seeds) {
    auto v = views::iota(s, s+len);
    for_each(execution::par, v.begin(), v.end(), [&](const auto &i){sol = min(accessMaps(i, maps), sol);});
    cout << "Done " << s << " with len " << len << endl;
  }

  return sol;
}

int main() {
  vector<pair<long, long>> seeds;
  vector<ElfMap> maps;

  ifstream in("input.txt");
  string buf;

  smatch m;
  getline(in, buf);
  regex_match(buf, m, regex("seeds: ([\\d+ ]+)"));
  stringstream ss(m[1]);
  string buf2;
  while (ss >> buf) {
    ss >> buf2;
    seeds.emplace_back(make_pair(stol(buf), stol(buf2)));
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