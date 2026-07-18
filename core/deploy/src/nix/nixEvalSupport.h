#pragma once

#include <map>
#include <string>
#include <vector>
using namespace std;

class nixEvalSupport {
public:
  struct evalPackage {
    string attrset;
    string start;
    string end;
    bool throwable = false;
  };

  static string initWorker(string cmd);
};
