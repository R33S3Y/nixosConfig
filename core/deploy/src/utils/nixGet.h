#pragma once

#include <map>
#include <string>
#include <vector>
using namespace std;

class nixGet {
public:
  static vector<string> flakeHosts(string flakePath);
  static string futureDerivationPath(string flakePath, string host);
  static string currentDerivationPath();
};
