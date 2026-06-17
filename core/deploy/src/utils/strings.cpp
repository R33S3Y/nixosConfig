#include "strings.h"
#include <cstddef>
#include <cstdio>
#include <cstring>
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

  if (endToken == "") {
    endToken = startToken;
  }

  string holdStr;

  while (fileStr.size() > 0 && fileStr.find(startToken) != string::npos) {
    size_t startLeft = fileStr.find(startToken);
    size_t startRight = startLeft + startToken.size();
    size_t endLeft = fileStr.find(endToken, startRight);
    if (endLeft == string::npos) {
      break;
    }
    size_t endRight = endLeft + endToken.size();

    for (size_t i = startRight; i < endLeft; i++) {
      if (fileStr[i] != '\n') {
        fileStr[i] = blankChar;
      }
    }
    holdStr += fileStr.substr(0, endRight);
    fileStr = fileStr.substr(endRight);
  }
  holdStr += fileStr;
  return holdStr;
}
