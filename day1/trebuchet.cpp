#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <map>
#include <regex>

std::map<std::string, std::string> translate = {
    {"one", "o1e"},
    {"two", "t2o"},
    {"three", "t3e"},
    {"four", "4"},
    {"five", "5e"},
    {"six", "6"},
    {"seven", "7n"},
    {"eight", "e8t"},
    {"nine", "n9e"}
};

int extractCalibrationValue(std::string input) {
    std::erase_if(input, [](char c){return !std::isdigit(c);});
    return stoi(
        std::string{*input.begin(), *(input.end()-1)}
    );
}

std::string fixLetters(std::string s) {
  for (const auto &[num, repr] : translate) {
    s = std::regex_replace(s, std::regex(num), repr);
  }
  return s;
}

int main() {
    std::ifstream in("input.txt");
    std::string buf;
    int res = 0;

    while (in >> buf) {
      res += extractCalibrationValue(fixLetters(buf));
    }
    
    if (not in.flags()) {
      std::cerr << "Error in opening file" << std::endl;
      return 1;
    }
    
    in.close();
    
    std::cout << res << std::endl;
}