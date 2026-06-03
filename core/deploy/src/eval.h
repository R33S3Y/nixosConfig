#pragma once

#include <map>
#include <string>
#include <type_traits>
#include <vector>
using namespace std;

class eval {
public:
  struct init {
    string flakePath;
    string flakeLink;
    string host;
  };
  eval(const init &i);
  void preProcessFile(string fileStr, string filePath);

  struct result {
    bool error = false;
    bool thrown = false;
    string type;
    string str;
    vector<string> list;
  };

  static string removeComments(string fileStr);

  result statement(string test, bool canThrow = true);
  string path(string test);
  result attrsetKey(string test, bool canThrow);

  // these ones are static simply because they can be,
  // and it allow them to be used outside of this class with out having to do
  // the very costly class init
  // (which I will probs need to make multi-threaded).
  static result bracket(string test);
  static vector<string> list(string test, bool throwLazy = true);

private:
  string flakePath;
  string flakeLink;
  string host;

  string fileStr;
  string filePath;
  string absoluteFilePath;
  vector<string> prettyFile;

  map<string, vector<string>> resolveMap;
  map<string, vector<string>> throwMap;
};
