#pragma once

#include <map>
#include <string>
#include <vector>
using namespace std;

class nixEvalSupport {
public:
  struct evalPackage {
    vector<string> attrset;
    string start;
    string end;
    bool throwable = false;
  };

  static map<string, nixEvalSupport::evalPackage>
      juniorInitWorker(map<string, nixEvalSupport::evalPackage>);
  static map<string, map<string, nixEvalSupport::evalPackage>>
      seniorInitWorker(map<string, nixEvalSupport::evalPackage>);
};
