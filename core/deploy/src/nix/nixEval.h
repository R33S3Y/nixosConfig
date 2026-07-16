#pragma once

#include "nixEvalSupport.h"
#include <any>
#include <map>
#include <string>
#include <type_traits>
#include <vector>
using namespace std;

class nixEval {
public:
  struct init {
    string flakePath;
    string flakeLink;
    string host;
  };
  nixEval(const init &i);
  void preProcessFile(string fileStr, string filePath);

  struct result {
    bool error = false;
    bool thrown = false;
    string type;
    string str;
    vector<string> list;
  };

  result statement(string test, bool canThrow = true);
  string path(string test);
  result attrsetKey(string test, bool canThrow);
  result bracket(string test);
  result lambdaCall(string test, bool canThrow);

private:
  string flakePath;
  string flakeLink;
  string host;

  string fileStr;
  string filePath;
  string absoluteFilePath;
  vector<string> prettyFile;

  vector<nixEvalSupport::evalPackage> evalPrimitives;
  map<string, nixEvalSupport::key> resolveMap;
  map<string, nixEvalSupport::key> throwMap;
  map<string, map<string, nixEvalSupport::key>> inheritMap;

  struct commandStrCache {
    string attrset;
    vector<string> attrsetKeys;
    bool throwable;
    string command;
  };
  vector<commandStrCache> commandCache;
  result makeCommandStr(const string attrset, const vector<string> attrsetKeys,
                        const bool canThrow);
};
