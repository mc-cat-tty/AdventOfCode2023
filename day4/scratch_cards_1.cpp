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

using namespace std;

int totalPoints(const vector<string> &i) {
  int res = 0;

  // ranges::for_each(i, [&](const auto &line){cout << line << endl;});
  ranges::for_each(i, [&res](const auto &line){
    smatch m;
    regex_match(line, m, regex("^Card +(\\d)+: ([\\d ]+) \\| ([\\d ]+)$"));
    // ranges::for_each(m, [&](auto e){cout << e <<  endl;});
    istringstream ss1(m[2]), ss2(m[3]);
    auto s1 = set<int>(istream_iterator<int>(ss1), istream_iterator<int>());
    auto s2 = set<int>(istream_iterator<int>(ss2), istream_iterator<int>());
    vector<int> intersection;
    ranges::set_intersection(s1, s2, back_inserter(intersection));
    if (not intersection.empty()) res += pow(2, intersection.size()-1);
  });

  return res;
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