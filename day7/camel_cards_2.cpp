#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

auto getHandRank(string h) {
  map<char, int> m;
  ranges::for_each(h, [&m](const auto &c){ m[c]++; });

  int boost = m['J'];
  m['J'] = 0;
  
  vector<pair<int, char>> v;
  ranges::transform(m, back_inserter(v), [](const auto &e){return make_pair(e.second, e.first);});

  ranges::sort(v, greater());


  if (v.at(0).first + boost == 5) return 7;
  if (v.at(0).first + boost == 4) return 6;
  if (v.at(0).first + boost == 3 and v.at(1).first == 2) return 5;
  if (v.at(0).first + boost == 3) return 4;
  if (v.at(0).first == 2 and v.at(1).first == 2) return 3;
  if (v.at(0).first + boost == 2) return 2;
  return 1;
}

auto getSuitRank(char s) {
  if (isdigit(s)) {
    return stoi(string{s});
  }
  switch (s) {
    case 'J': return 1;
    case 'T': return 10;
    case 'Q': return 12;
    case 'K': return 13;
    case 'A': return 14;
  }
  return 0;
}

int cmpHands(string lh, string rh) {
  auto rankL = getHandRank(lh); 
  auto rankR = getHandRank(rh); 

  if (rankL != rankR) return rankL-rankR;

  for (int i=0; i<lh.size(); i++) {
    if (getSuitRank(lh.at(i)) != getSuitRank(rh.at(i))) {
      return getSuitRank(lh.at(i)) - getSuitRank(rh.at(i));
    }
  }

  return 0;
}

int totalWinnings(vector<pair<string, int>> hands) {
  cout << cmpHands("77888", "77788") << endl;
  ranges::sort(hands, [](const auto &l, const auto &r){return cmpHands(l.first, r.first) < 0;});
  // ranges::for_each(hands, [](auto e){cout << e << endl;});
  
  int res = 0, count = 1;
  for (const auto &[hand, bid] : hands) {
    res += count*bid;
    count++;
  }
  
  return res;
}

int main() {
  ifstream in("input.txt");

  vector<pair<string, int>> hands;
  string buf;
  int tmp;

  while (in >> buf) {
    in >> tmp;
    hands.emplace_back(buf, tmp);
  }

  if (not in.flags()) {
    cerr << "Error while reading input" << endl;
    return 1;
  }

  in.close();

  cout << totalWinnings(hands) << endl;
}