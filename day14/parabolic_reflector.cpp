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

auto reverse(const vector<string> &b) {
  vector<string> res;
  for (int i=0; i<b.at(0).size(); i++) {
    string tmp;
    for (const auto &l : b) tmp += l.at(i);
    res.emplace_back(tmp);
  }
  return res;
}

auto weight(const string &s) {
  long res = 0, count = 0;
  auto dim = s.size();

  int f = 0;  // first
  for (int l=0; l<dim; l++) {  // last
    if (s[l] == 'O') count++;
    if (s[l] == '#' or l == dim - 1) {
      auto leftMostIdx = dim - f;
      auto rightTail = leftMostIdx - count;
      res += (leftMostIdx * (leftMostIdx + 1) - rightTail * (rightTail + 1)) / 2;
      f = l+1;
      count = 0;
    }
  }
  return res;
  
}

auto totalWeight(vector<string> p) {
  p = reverse(p);
  return accumulate(p.begin(), p.end(), 0L, [](auto acc, auto e){return acc + weight(e);});
}

int main() {
  ifstream in("input.txt");
  vector<string> platform;
  string buf;

  while (getline(in, buf)) {
    platform.push_back(buf);
  }

  if (not in.flags()) {
    cerr << "Error while reading input" << endl;
    return 1;
  }

  in.close();

  cout << totalWeight(platform) << endl;
}