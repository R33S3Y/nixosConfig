#pragma once

#include "nixEvalSupport.h"
#include <map>
#include <string>
#include <vector>
using namespace std;

class staticGet {
public:
  struct evalPackage {
    string attrset; // is like "abc.xyz"
    string start;
    string end;
    bool throwable = false;
  };

  static bool filterCandidate(staticGet::candidate testingCandidate);
  // Takes attrsets { "abc.xyz", "config"} and gets their other info from
  // evalPrimitives
  static vector<staticGet::evalPackage> evalPackagesFromAttrsets(
      const vector<string> &attrsets,
      const vector<staticGet::evalPackage> &evalPrimitives);
  // Returns vector of attrsets Eg: { "abc.xyz", "config"}
  static vector<string> inputValues(string file);
  // Returns vector of attrsets Eg: { "abc.xyz", "config"}
  static vector<string> inheritStatement(string file);
  // Returns vector of attrsets Eg: { "abc.xyz", "config"}
  static vector<string> letInVariables(string file);

  static vector<string> tokenizedTopLevel(const string statement);

  static size_t validStatementPos(string statement, string s);
  static vector<string> listItems(string test, bool throwLazy = true);
  static vector<string> flakeHosts(string flakePath);
};
