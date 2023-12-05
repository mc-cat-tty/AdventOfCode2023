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

inline auto reverseAccessMaps(long rkey, vector<ElfMap> maps) {
  long res = rkey;
  ranges::reverse(maps);
  for (const auto &m : maps) {
    for (const auto &[d, s, r] : m) {
      long offset = res - d;
      if (offset >= 0 and offset < r) {
        res = s + offset;
        break;
      }
    }
  }
  return res;
}

long lowestLocation(const vector<pair<long, long>> &seeds, const vector<ElfMap> &maps) {
  // ranges::for_each(seeds, [](auto e) {cout << e << " " << endl;});
  // ranges::for_each(maps, [](auto l) {
  //   ranges::for_each(l, [](auto e){cout << e.d << " " << e.s << " " << e.r << " - ";});
  //   cout << endl;
  // });

  long i=0;
  while (true) {
    long rev = reverseAccessMaps(i, maps);
    // cout << i << "->" << rev << endl;
    for (const auto &[s, r] : seeds){
      if (rev >= s and rev < s+r) {
        return i;
      }
    }
    i++;
  }
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