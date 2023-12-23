#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <regex>
#include <iterator>
#include <ranges>

using namespace std;
using Block = vector<string>;

auto symmetry(Block b) {
  long s = b.size();
  for (int r=1; r<s; r++) {
    int i;
    for (i=0; i<r; i++) {
      if (r+i >= s) return r;
      if (b.at(r-i-1) != b.at(r+i)) break;
    }
    if (i == r) return r;
  }
  return 0;
}

auto reverse(const Block &b) {
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
    res += 100 * symmetry(b) + symmetry(reverse(b));
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