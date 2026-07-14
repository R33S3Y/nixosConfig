#pragma once

#include <map>
#include <string>
#include <vector>
using namespace std;

class nixEvalSupport {
public:
  struct key {
    string start;
    string end;
  };

  static map<string, nixEvalSupport::key> juniorInitWorker(map<string, key>);
  static map<string, map<string, nixEvalSupport::key>>
      seniorInitWorker(map<string, key>);
};
