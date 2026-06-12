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

  static string replace(string s, string from, string to);
  static string rReplace(string s, string from, string to);
  static string replaceAll(string s, string from, string to);

  static string trim(string s);

  static string blankWithinTokens(string fileStr, string startToken,
                                  string endToken = "", char blankChar = ' ');

  // string processing
  static vector<string> splitStrByChar(string inputStr, char inputChar);
  static vector<string> splitStrByChars(string inputStr,
                                        vector<char> inputChars);
  static vector<string>
  splitStrByCharByFilterStr(string inputStr, string filterStr, char inputChar);
  static vector<string> splitStrByCharsByFilterStr(string inputStr,
                                                   string filterStr,
                                                   vector<char> inputChars);

  template <typename type>
  static vector<vector<type>> splitVector(vector<type> vec, int splits);
  template <typename keyType, typename valueType>
  static vector<map<keyType, valueType>>
  splitMap(map<keyType, valueType> inputMap, int splits);
};

#include "utils.tpp"
