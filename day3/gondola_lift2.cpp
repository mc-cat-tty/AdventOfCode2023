#include <iostream>
#include <sstream>
#include<vector>
#include <iterator>
#include <algorithm>
#include <fstream>

using namespace std;
using Coord = pair<int, int>;

auto extractParts(const vector<string> &m) {
  vector<Coord> res;
  for (int i=0; const auto &s : m) {
    for (int j=0; const auto &c : s) {
      if (isdigit(c)) {
        res.emplace_back(make_pair(i, j));
      }
      j++;
    }
    i++;
  }
  return res;
}

auto extractAsterisks(const vector<string> &m) {
  vector<Coord> res;
  for (int i=0; const auto &s : m) {
    for (int j=0; const auto &c : s) {
      if (c == '*') {
        res.emplace_back(make_pair(i, j));
      }
      j++;
    }
    i++;
  }
  return res;
}

inline bool areNear(Coord p, Coord s) {
  return (p.first >= s.first-1 and p.first <= s.first+1) and (p.second <= s.second+1 and p.second >= s.second-1);
}

inline bool isGear(Coord s, Coord p1, Coord p2) {
  return areNear(p1, s) and areNear(p2, s);
}

int extract(const vector<string> &m, const Coord &start, int &endCol) {
  string buf{m[start.first][start.second]};
  int row = start.first, col = start.second;

  for (int i=col-1, prev=col; i >= 0 and isdigit(m[row][i]); prev=i, i--)
    buf.insert(0, string{m[row][i]});

  for (int i=col+1, prev=col; i < m[row].size() and isdigit(m[row][i]); prev=i, endCol=i, i++)
    buf += m[row][i];

  return stoi(buf);
}

int gearRation(const vector<string> &m) {
  const vector<Coord> asterisks = extractAsterisks(m);
  int res = 0;

  for (int i=0; i<asterisks.size(); i++) {
    vector<int> adjV;
    for (int j=max(asterisks.at(i).first-1, 0); j<=min(asterisks.at(i).first+1, (int) m.size()-1); j++) {
      for (int k=max(asterisks.at(i).second-1, 0); k<=min(asterisks.at(i).second+1, (int) m[j].size()-1); k++) {
        if (isdigit(m[j][k])) {
          int endCol = k;
          adjV.emplace_back(extract(m, Coord{j, k}, endCol));
          k = endCol+1;
        }
      }
    }
    if (adjV.size() == 2) {
      int gearRatio = adjV[0]*adjV[1];
      res += gearRatio;
    }
  }

  return res;
}

int main() {
  ifstream in("input.txt");
  vector<string> input;
  string buf;

  while (in >> buf) {
    input.push_back(buf);
  }

  if (!in.flags()) {
    cerr << "Error in opening file" << endl;
    return 1;
  }  

  in.close();

  cout << gearRation(input) << endl;
}