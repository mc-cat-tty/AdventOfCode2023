#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>
#include <set>
#include <algorithm>
#include <regex>
#include <numeric>
#include <cmath>
#include <map>

using namespace std;

int copiesNumber(const vector<set<int>> &s1, const vector<set<int>> &s2, int selL, int selH) {
  // cout << selL << " " << selH << endl;
  
  int res = 0;
  for (int s=selL; s<=selH; s++) {
    vector<int> intersection;
    ranges::set_intersection(s1[s-1], s2[s-1], back_inserter(intersection));
    res += 1;
    res += copiesNumber(s1, s2, s+1, s+intersection.size());
  }

  return res;
}

int totalPoints(const vector<string> &i) {
  vector<set<int>> s1;
  vector<set<int>> s2;
  vector<int> selected;

  // ranges::for_each(i, [&](const auto &line){cout << line << endl;});
  for (const auto &line : i) {
    smatch m;
    regex_match(line, m, regex("^Card +(\\d)+: ([\\d ]+) \\| ([\\d ]+)$"));
    // ranges::for_each(m, [&](auto e){cout << e <<  endl;});
    istringstream ss1(m[2]), ss2(m[3]);
    auto _s1 = set<int>(istream_iterator<int>(ss1), istream_iterator<int>());
    auto _s2 = set<int>(istream_iterator<int>(ss2), istream_iterator<int>());
    s1.emplace_back(_s1);
    s2.emplace_back(_s2);
    selected.emplace_back(stoi(m[1]));
  };

  return copiesNumber(s1, s2, 1, i.size());
}

int main() {
  ifstream in("input.txt");
  vector<string> input;
  string buf;

  while (getline(in, buf)) {
    input.emplace_back(buf);
  }

  if (!in.flags()) {
    cerr << "Error in reading input file" << endl;
    return 1;
  }

  in.close();

  cout << totalPoints(input) << endl;
}