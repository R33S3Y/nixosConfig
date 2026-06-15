#pragma once

#include <map>
#include <string>
#include <type_traits>
#include <vector>
using namespace std;

class utils {
public:
  struct result {
    string output;
    int exitCode;
    string error;
  };

  static result runCommand(string cmd);
  static string readFile(const string &path);

  static string error(string message);

  static string blankWithinTokens(string fileStr, string startToken,
                                  string endToken = "", char blankChar = ' ');
};
