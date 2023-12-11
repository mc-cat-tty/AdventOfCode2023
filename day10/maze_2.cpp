#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <numeric>
#include <algorithm>

/**
 * S's replacement has to be changed manually because it's
 * faster than writing a piece of code to do that
*/
constexpr char START_REPLACEMENT = '7';  // Change this before running the program

using namespace std;
using Coord = pair<long, long>;
using Seg = pair<Coord, Coord>;

bool operator== (const Coord &a, const Coord &b) {
  return a.first == b.first and a.second == b.second;
}

auto findStart(vector<string> m) {
  for (long i=0; i<m.size(); i++) {
   for (long j=0; j<m[0].size(); j++) {
    if (m.at(i).at(j) == 'S') return make_pair(i, j);
   } 
  }
  return make_pair(-1L, -1L);
}

auto findNext(vector<string> m, long pr, long pc, long r, long c) {
  char ch = m[r][c];

  switch (ch) {
  case '|': if (r==pr+1 and c==pc) return make_pair(pr+2, pc); else return make_pair(pr-2, pc);
  case '-': if (r==pr and c==pc+1) return make_pair(pr, pc+2); else return make_pair(pr, pc-2);
  case 'L': if (r==pr+1 and c==pc) return make_pair(pr+1, pc+1); else return make_pair(pr-1, pc-1);
  case 'J': if (r==pr+1 and c==pc) return make_pair(pr+1, pc-1); else return make_pair(pr-1, pc+1);
  case '7': if (r==pr and c==pc+1) return make_pair(pr+1, pc+1); else return make_pair(pr-1, pc-1);
  case 'F': if (r==pr and c==pc-1) return make_pair(pr+1, pc-1); else return make_pair(pr-1, pc+1);
  }

  return make_pair(-1L, -1L);
}

auto findNearestValid(vector<string> m, long i, long j) {
  if (m[i][j+1] == '-') return make_pair(i, ++j);
  if (m[i][j-1] == '-') return make_pair(i, --j);

  if (m[i+1][j] == '|') return make_pair(++i, j);
  if (m[i-1][j] == '|') return make_pair(--i, j);
  
  if (m[i+1][j] == 'L') return make_pair(++i, j);
  if (m[i][j-1] == 'L') return make_pair(i, --j);
  
  if (m[i+1][j] == 'J') return make_pair(++i, j);
  if (m[i][j+1] == 'J') return make_pair(i, ++j);

  if (m[i-1][j] == '7') return make_pair(--i, j);
  if (m[i][j+1] == '7') return make_pair(i, ++j);

  if (m[i-1][j] == 'F') return make_pair(--i, j);
  if (m[i][j-1] == 'F') return make_pair(i, --j);

  return make_pair(-1L, -1L);
}

auto hitHorSegment(vector<Seg> segs, long r, long c) {
  for (const auto &s : segs) {
    if (r == s.first.first and c >= min(s.first.second, s.second.second) and c <= max(s.first.second, s.second.second)) return true;
  }
  return false;
}

auto isInMaze(vector<Coord> maze, long r, long c) {
  for (const auto &m : maze) {
    if (m.first == r and m.second == c) return true;
  }
  return false;
}

auto concav(char c) {
  switch (c) {
  case 'J': return -1;
  case '7': return -1;
  case 'F': return 1;
  case 'L': return 1;
  }
  return 0;
}

auto farthestPoint(vector<string> m) {
  auto [previ, prevj] = findStart(m);
  auto [i, j] = findNearestValid(m, previ, prevj);
  Coord start{previ, prevj};
  vector<Seg> HSegs;
  vector<Coord> maze{{previ, prevj}, {i, j}};
  // cout << previ << " " << prevj << endl;
  // cout << i << " " << j << endl;

  do {
    if (m[previ][prevj] != '-' and m[i][j] == '-') HSegs.push_back({{previ, prevj}, {-1L, -1L}});
    else if (m[previ][prevj] == '-' and m[i][j] != '-') (*(HSegs.end()-1)).second = {i, j};
    else if (previ == i and m[i][j] != m[previ][prevj]) HSegs.push_back({{previ, prevj}, {i, j}});

    auto [ni, nj] = findNext(m, previ, prevj, i, j);
    previ = i; prevj = j;
    i = ni; j = nj;
    maze.emplace_back(i, j);
    // cout << i << " " << j << endl;
  } while (Coord{i, j} != start);
  maze.emplace_back(i, j);
  if (m[previ][prevj] == '-' and m[i][j] != '-') (*(HSegs.end()-1)).second = {i, j};
  else if (previ == i and m[i][j] != m[previ][prevj]) HSegs.push_back({{previ, prevj}, {i, j}});

  // for (auto s : HSegs) {
  //   cout << s.first.first << " " << s.first.second << " - " << s.second.first << " " << s.second.second << endl; 
  // }


  m[start.first][start.second] = START_REPLACEMENT;

  long res = 0;
  for (long c=0; c<m[0].size(); c++) {
    long hitCount = 0;
    char prevHit = '\0';
    for (long r=0; r<m.size(); r++) {
      if (hitHorSegment(HSegs, r, c) and (prevHit == '\0' or concav(prevHit)*concav(m[r][c]) >= 0)) {
        hitCount++;
        prevHit = m[r][c];
        // cout << "Hit: " << r << "," << c << endl;
      }

      if (not isInMaze(maze, r, c) and hitCount%2) {
        res++;
        // cout << "Tile: " << r << " " << c << endl;
      }
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

  cout << farthestPoint(m) << endl;
}