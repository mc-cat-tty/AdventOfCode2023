#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

auto findStart(vector<string> m) {
  for (int i=0; i<m.size(); i++) {
   for (int j=0; j<m[0].size(); j++) {
    if (m.at(i).at(j) == 'S') return make_pair(i, j);
   } 
  }
  return make_pair(-1, -1);
}

auto findNext(vector<string> m, int pr, int pc, int r, int c) {
  char ch = m[r][c];

  switch (ch) {
  case '|': if (r==pr+1 and c==pc) return make_pair(pr+2, pc); else return make_pair(pr-2, pc);
  case '-': if (r==pr and c==pc+1) return make_pair(pr, pc+2); else return make_pair(pr, pc-2);
  case 'L': if (r==pr+1 and c==pc) return make_pair(pr+1, pc+1); else return make_pair(pr-1, pc-1);
  case 'J': if (r==pr+1 and c==pc) return make_pair(pr+1, pc-1); else return make_pair(pr-1, pc+1);
  case '7': if (r==pr and c==pc+1) return make_pair(pr+1, pc+1); else return make_pair(pr-1, pc-1);
  case 'F': if (r==pr and c==pc-1) return make_pair(pr+1, pc-1); else return make_pair(pr-1, pc+1);
  }

  return make_pair(-1, -1);
}

auto findNearestValid(vector<string> m, int i, int j) {
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

  return make_pair(-1, -1);
}

auto farthestPoint(vector<string> m) {
  auto [previ, prevj] = findStart(m);
  auto [i, j] = findNearestValid(m, previ, prevj);
  // cout << i << " " << j << endl;
  int res = 0;

  do {
    auto [ni, nj] = findNext(m, previ, prevj, i, j);
    previ = i; prevj = j;
    i = ni; j = nj;
    // cout << i << " " << j << endl;
    res++;
  } while (m[i][j] != 'S');

  res++;

  return res/2;
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