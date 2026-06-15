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
