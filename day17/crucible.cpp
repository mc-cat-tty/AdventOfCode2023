#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <regex>
#include <iterator>
#include <ranges>
#include <set>
#include <queue>
#include <map>

using namespace std;
constexpr long MAX_STREAK = 3;
pair<long, long> operator+(const pair<long, long> &a, const pair<long, long> &b) {
  return {a.first+b.first, a.second+b.second};
}

pair<long, long> operator-(const pair<long, long> &a, const pair<long, long> &b) {
  return {a.first-b.first, a.second-b.second};
}

pair<long, long> operator*(const pair<long, long> &p1, const long &k) {
  return {p1.first*k, p1.second*k};
}

struct Edges {
  struct ProxyStr : string {
    int operator[](long j) const {
      return static_cast<int>(this->at(j) - '0');
    }
  };

  vector<string> content;

  ProxyStr operator[](long i) const {
    return ProxyStr(content.at(i));
  }
};

struct DijkstraEntry {
  pair<long, long> coord;
  pair<long, long> dir;
  long dist;
};

inline bool inBoundaries(pair<long, long> p, long R, long C) {
  return p.first >= 0 and p.first < R and p.second >= 0 and p.second < C;
}

// steering logic
auto directions(const pair<long, long> &p, pair<long, long> &dir) {
  pair<long, long> dir1, dir2;
  if (dir.first == 0 and dir.second == 0) {  // starting point
    dir1 = {-1, 0};
    dir2 = {0, 1};
  }
  else if (dir.first != 0) {  // moved along Y (rows)
    dir1 = {0, 1};
    dir2 = {0, -1};
  }
  else {  // moved along X (cols)
    dir1 = {1, 0};
    dir2 = {-1, 0};
  }

  return set<pair<long, long>>{dir1, dir2};
}

long shortestPath(const Edges &e) {
  pair<long, long> source = {0, 0};
  pair<long, long> target = {e.content.size()-1, e.content.at(0).size()-1};
  const long R = target.first+1, C = target.second+1;
 
  const auto cmp = [](const auto &a, const auto &b){return a.dist > b.dist;};
  priority_queue<DijkstraEntry, vector<DijkstraEntry>, decltype(cmp)> q(cmp);
  q.push(DijkstraEntry{
    .coord = {0, 0},
    .dir = {0, 0},
    .dist = 0
  });

  set<pair<pair<long, long>, pair<long, long>>> visited;

  while (not q.empty()) {
    DijkstraEntry u = q.top();
    q.pop();
    if (visited.contains({u.coord, u.dir})) continue;
    visited.emplace(u.coord, u.dir);
    cout << visited.size() << endl;

    if (u.coord == target) return u.dist;

    for (const auto &d : directions(u.coord, u.dir)) {
      long alt = u.dist;

      for (long k=1; k<=MAX_STREAK; k++) {
        auto v = u.coord + d*k;
        if (not inBoundaries(v, R, C)) break;

        alt += e[v.first][v.second];

        q.push(DijkstraEntry{
          .coord = v,
          .dir = v - u.coord,
          .dist = alt
        });
      }
    }
  }

  throw runtime_error("Infreasable");
}

int main() {
  ifstream in("input.txt");
  Edges e;

  string buf;
  while (getline(in, buf)) {
    e.content.emplace_back(buf);
  }

  if (not in.flags()) {
    cerr << "Error while reading input" << endl;
    return 1;
  }

  in.close();

  cout << shortestPath(e) << endl;
}