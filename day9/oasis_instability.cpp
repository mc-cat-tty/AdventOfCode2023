#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <numeric>
#include <algorithm>

using namespace std;
using Timeseries = vector<int>;

auto prediction(const Timeseries &v) {
  if (v.empty()) return 0;

  Timeseries t;
  bool allZero = true;
  for (int i=0, e=v.size()-1; i<e; i++) {
    int diff = v.at(i+1) - v.at(i);
    t.emplace_back(diff);

    if (allZero and diff != 0) allZero = false;
  }

  return *(v.end()-1) + (allZero ? 0 : prediction(t));
}

auto sumPrediction(const vector<Timeseries> &v) {
  return accumulate(v.begin(), v.end(), 0, [](auto acc, auto x){ return acc + prediction(x); });
}

int main() {
  ifstream in("input.txt");
  vector<Timeseries> v;
  string buf;

  while (getline(in, buf)) {
    stringstream ss(buf);
    v.emplace_back(
      Timeseries(istream_iterator<int>(ss), istream_iterator<int>())
    );
  }

  if (not in.flags()) {
    cerr << "Error while reading input" << endl;
    return 1;
  }

  in.close();

  cout << sumPrediction(v) << endl;
}