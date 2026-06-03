#pragma once

#include <string>
#include <vector>
using namespace std;

class utils {
public:
  struct result {
    string output;
    int exitCode;
    string error;
    bool ok() const { return exitCode == 0; }
  };

  static result runCommand(string cmd);
  static string readFile(const string &path);

  static string error(string message);

  // string processing
  static vector<string> splitStrByChar(string inputStr, char inputChar);
  static vector<string> splitStrByChars(string inputStr,
                                        vector<char> inputChars);
  static vector<string>
  splitStrByCharByFilterStr(string inputStr, string filterStr, char inputChar);
  static vector<string> splitStrByCharsByFilterStr(string inputStr,
                                                   string filterStr,
                                                   vector<char> inputChars);

  static string replace(string s, string from, string to);
  static string rReplace(string s, string from, string to);
  static string replaceAll(string s, string from, string to);

  static string trim(string s);

  static string blankWithinTokens(string fileStr, string startToken,
                                  string endToken = "", char blankChar = ' ');
};
