#pragma once

#include <map>
#include <string>
#include <type_traits>
#include <vector>
using namespace std;

class split {
public:
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

#include "split.tpp"
