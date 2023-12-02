#include <iostream>
#include <sstream>
#include<vector>
#include <iterator>
#include <algorithm>
#include <fstream>

auto powerFewest(std::string s) {
  int r, g, b;
  int maxr = 0, maxg = 0, maxb = 0;
  auto ss = std::istringstream(s);
  auto v = std::vector<std::string>(
    std::istream_iterator<std::string>(ss),
    std::istream_iterator<std::string>()
  );

  for (int i=2; i<v.size(); i+=2) {
    auto count = std::stoi(v[i]);
    auto label = v[i+1];
    char terminator;

    if (std::find(label.begin(), label.end(), ',') != label.end() || std::find(label.begin(), label.end(), ';') != label.end()) {
      terminator = label.back();
      label.pop_back();
    }
    else {
      terminator = '\0';
    }

    if (label == "red") r = count;
    if (label == "green") g = count;
    if (label == "blue") b = count;

    maxr = std::max(maxr, r);
    maxg = std::max(maxg, g);
    maxb = std::max(maxb, b);
  }

  return maxr*maxg*maxb;
}

int main() {
  std::ifstream in("input.txt");
  std::string buf;
  int res = 0;

  while (getline(in, buf)) {
    res += powerFewest(buf);
  }
  
  if (not in.flags()) {
    std::cerr << "Error in opening file" << std::endl;
    return 1;
  }
  
  in.close();
  
  std::cout << res << std::endl;
}