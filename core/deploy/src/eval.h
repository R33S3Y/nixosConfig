#pragma once

#include <any>
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
  result bracket(string test);

  // these ones are static simply because they can be,
  // and it allow them to be used outside of this class with out having to do
  // the very costly class init
  // (which I will probs need to make multi-threaded).
  static vector<string> list(string test, bool throwLazy = true);

private:
  struct key {
    string start;
    string end;
  };
  string flakePath;
  string flakeLink;
  string host;

  string fileStr;
  string filePath;
  string absoluteFilePath;
  vector<string> prettyFile;

  map<string, key> resolveMap;
  map<string, key> throwMap;

  static map<string, vector<string>> juniorInitWorker(map<string, key>);
  static map<string, vector<string>> seniorInitWorker(map<string, key>);
};
