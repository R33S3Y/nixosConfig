
#include "split.h"
#include "ttyHelper.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

vector<string> split::splitStrByChar(string inputStr, char inputChar) {
  vector<string> output;
  string currentStr;

  for (char currentChar : inputStr) {
    if (currentChar == inputChar) {
      output.push_back(currentStr);
      currentStr.clear();
    } else {
      currentStr += currentChar;
    }
  }
  output.push_back(currentStr);
  return output;
}
vector<string> split::splitStrByChars(string inputStr,
                                      vector<char> inputChars) {
  vector<string> output;
  string currentStr;

  for (char currentChar : inputStr) {
    if (find(inputChars.begin(), inputChars.end(), currentChar) !=
        inputChars.end()) {
      output.push_back(currentStr);
      currentStr.clear();
    } else {
      currentStr += currentChar;
    }
  }
  output.push_back(currentStr);
  return output;
}
vector<string> split::splitStrByCharByFilterStr(string inputStr,
                                                string filterStr,
                                                char inputChar) {

  if (inputStr.size() != filterStr.size()) {
    cerr << ttyHelper::error(
        "systemHelper::splitStrByCharByFilterStr : inputStr and "
        "fileStr does not match");
    return {};
  }

  vector<string> output;
  string currentStr;

  for (int i = 0; i < filterStr.size(); i++) {
    if (filterStr[i] == inputChar) {
      output.push_back(currentStr);
      currentStr.clear();
    } else {
      currentStr += inputStr[i];
    }
  }
  output.push_back(currentStr);
  return output;
}
vector<string> split::splitStrByCharsByFilterStr(string inputStr,
                                                 string filterStr,
                                                 vector<char> inputChars) {

  if (inputStr.size() != filterStr.size()) {
    cerr << ttyHelper::error(
        "systemHelper::splitStrByCharByFilterStr : inputStr and fileStr "
        "does not match");
    return {};
  }

  vector<string> output;
  string currentStr;

  for (int i = 0; i < filterStr.size(); i++) {
    if (find(inputChars.begin(), inputChars.end(), filterStr[i]) !=
        inputChars.end()) {
      output.push_back(currentStr);
      currentStr.clear();
    } else {
      currentStr += inputStr[i];
    }
  }
  output.push_back(currentStr);
  return output;
}
