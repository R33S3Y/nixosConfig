#pragma once

#include <string>
using namespace std;

class strings {
public:
  static string replace(string s, string from, string to);
  // reverse Replace
  static string rReplace(string s, string from, string to);
  static string replaceAll(string s, string from, string to);

  static string trim(string s);
};
