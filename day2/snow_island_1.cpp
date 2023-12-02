#include <iostream>
#include <sstream>
#include<vector>
#include <iterator>
#include <algorithm>
#include <fstream>

constexpr unsigned int MAX_R = 12;
constexpr unsigned int MAX_G = 13;
constexpr unsigned int MAX_B = 14;

int possible(std::string s) {
  int id, r, g, b;
  auto ss = std::istringstream(s);
  auto v = std::vector<std::string>(
    std::istream_iterator<std::string>(ss),
    std::istream_iterator<std::string>()
  );
  id = std::stoi(v[1]);

  for (int i=2; i<v.size(); i+=2) {
    auto count = std::stoi(v[i]);
    auto label = v[i+1];
    if (std::find(label.begin(), label.end(), ',') != label.end() || std::find(label.begin(), label.end(), ';') != label.end()) {
      auto terminator = label.back();
      label.pop_back();
    }
    
    // std::cout << label << ":" << count << " ";
    if (label == "red" && count > MAX_R || label == "green" && count > MAX_G || label == "blue" && count > MAX_B) {
      return -1;
    }
  }

  return id;
}

int main() {
  std::ifstream in("input.txt");
  std::string buf;
  int res = 0;

  while (getline(in, buf)) {
    auto p = possible(buf);
    res += p != -1 ? p : 0;
  }
  
  if (not in.flags()) {
    std::cerr << "Error in opening file" << std::endl;
    return 1;
  }
  
  in.close();
  
  std::cout << res << std::endl;
}