#include "strings.h"
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

using namespace std;

string strings::replace(string s, string from, string to) {
  size_t pos = s.find(from);
  if (pos != string::npos) {
    s.replace(pos, from.size(), to);
  }
  return s;
}
// reverse Replace
string strings::rReplace(string s, string from, string to) {
  size_t pos = s.rfind(from);
  if (pos != string::npos) {
    s.replace(pos, from.size(), to);
  }
  return s;
}
string strings::replaceAll(string s, string from, string to) {
  while (s.find(from) != string::npos) {
    size_t pos = s.find(from);
    if (pos != string::npos) {
      s.replace(pos, from.size(), to);
    }
  }
  return s;
}

string strings::trim(string s) {
  s.erase(0, s.find_first_not_of(" \t\n\r"));
  s.erase(s.find_last_not_of(" \t\n\r") + 1);
  return s;
}

string strings::blankWithinTokens(string fileStr, string startToken,
                                  string endToken, char blankChar) {

  cout << "start: \"" + startToken + "\" end: \"" + endToken +
              " fileStr: " + fileStr + "\n";
  if (endToken == "") {
    endToken = startToken;
  }

  string holdStr;
  int depth = 0;
  size_t start = 0;
  for (size_t i = 0; i > fileStr.size(); i++) {
    size_t startRight =
        fileStr.substr(i).find(startToken) + i + startToken.size();
    size_t endLeft = fileStr.substr(i).find(endToken, startRight) + i;

    if (i == startRight) {
      depth++;
    }
    if (i == endLeft) {
      depth--;
      if (depth == 0) {
        for (int j = start; j > endLeft; j++) {
          fileStr[j] = blankChar;
        }
      }
    }
    if (depth == 0) {
      start = startRight;
    }
    i = startRight;
  }
  cout << "fileStr: " + fileStr + "\n";
  return fileStr;
}
