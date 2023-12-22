#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <optional>

using namespace std;
using Block = vector<string>;

optional<long> tolerantCmp(string x, string y) {
  assert(x.size() == y.size());

  const long OOR = x.size() + 1;  // Out Of Range
  long res = OOR;
  for (int i=0; i<x.size(); i++) {
    if (x.at(i) != y.at(i)) {
      if (res == OOR) res = i;
      else return nullopt;
    }
  }

  return res;
}

inline void toggle(char &c) {
  c = (c == '.' ? '#' : '.');
}

auto symmetry1(Block b) {
  long s = b.size();
  for (int r=1; r<s; r++) {
    int i;
    for (i=0; i<r; i++) {
      if (r+i >= s) {
        return r;
      }
      if (b.at(r-i-1) != b.at(r+i)) break;
    }
    if (i == r) return r;
  }
  return -1;
}

auto symmetry2(Block b) {
  long oldLine = symmetry1(b);
  long s = b.size();

  for (int r=1; r<s; r++) {
    int i;
    pair<long, long> stainCoord = {-1, -1};

    for (i=0; i<r; i++) {
      if (r+i >= s) {
        if (r != oldLine) return r;
        else break;
      }

      auto eq = tolerantCmp(b.at(r-i-1), b.at(r+i));
      bool isThereStain = eq.has_value() and eq.value() < b.at(0).size();
      if (not eq.has_value() or isThereStain and stainCoord.first != -1) break;
      else if (isThereStain) {  // fix stain
        stainCoord = {r+i, eq.value()};
        toggle(b[stainCoord.first][stainCoord.second]);
      }
    }
    if (i == r and r != oldLine) return r;

    if (stainCoord.first != -1) {
      toggle(b[stainCoord.first][stainCoord.second]);
    }
  }
  return 0;
}

auto reverse(const Block b) {
  Block res;
  for (int i=0; i<b.at(0).size(); i++) {
    string tmp;
    for (const auto &l : b) tmp += l.at(i);
    res.emplace_back(tmp);
  }
  return res;
}

auto patterns(const vector<Block> &v) {
  long res = 0;
  for (const auto &b : v) {
    auto horSym = symmetry2(b);
    auto verSym = not horSym ? symmetry2(reverse(b)) : 0;
    res += 100 * horSym + verSym;
  }
  return res;
}

int main() {
  ifstream in("input.txt");
  vector<Block> blocks;
  vector<string> tmp;
  string buf;

  while (getline(in, buf)) {
    if (buf.empty()) blocks.push_back(move(tmp));
    else tmp.emplace_back(buf);
  }
  blocks.push_back(move(tmp));

  if (not in.flags()) {
    cerr << "Error while reading input" << endl;
    return 1;
  }

  in.close();

  cout << patterns(blocks) << endl;
}