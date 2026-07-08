#pragma once

#include <map>
#include <string>
#include <vector>
using namespace std;

class nixEvalStatic {
public:
  struct key {
    string start;
    string end;
  };
  struct candidate {
    vector<string> attrsetKeys;
    string start;
    string end;
  };

  static bool filterCandidate(candidate testingCandidate);
  static vector<string> tokenize(const string thing);

  static string removeComments(string fileStr);
  static string removeLetIn(string fileStr);

  static vector<string> list(string test, bool throwLazy = true);
  static vector<string> getFlakeHosts(string flakePath);

  static map<string, nixEvalStatic::key> juniorInitWorker(map<string, key>);
  static map<string, map<string, nixEvalStatic::key>>
      seniorInitWorker(map<string, key>);
};
