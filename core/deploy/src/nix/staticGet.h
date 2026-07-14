#pragma once

#include <map>
#include <string>
#include <vector>
using namespace std;

class staticGet {
public:
  struct candidate {
    vector<string> attrsetKeys;
    string start;
    string end;
  };

  static bool filterCandidate(candidate testingCandidate);
  static vector<string> tokenizedTopLevel(const string thing);

  static size_t validStatementPos(string statement, string s);
  static vector<string> listItems(string test, bool throwLazy = true);
  static vector<string> flakeHosts(string flakePath);
};
