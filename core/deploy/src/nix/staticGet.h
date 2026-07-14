#pragma once

#include "nixEvalSupport.h"
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
  struct key {
    string start;
    string end;
  };

  static bool filterCandidate(staticGet::candidate testingCandidate);
  static vector<staticGet::candidate>
  candidatesFromAttrsets(vector<string> attrsets,
                         map<string, staticGet::key> resolveMap,
                         staticGet::key throwMap);
  static vector<string> inputValues(string file);
  static vector<staticGet::candidate> inheritStatement(string file);
  static vector<staticGet::candidate> letInVariables(string file);

  static vector<string> tokenizedTopLevel(const string thing);

  static size_t validStatementPos(string statement, string s);
  static vector<string> listItems(string test, bool throwLazy = true);
  static vector<string> flakeHosts(string flakePath);
};
