#pragma once

#include "eval.h"
#include <map>
#include <shared_mutex>
#include <string>
#include <type_traits>
#include <vector>
using namespace std;

class resolve {
public:
  resolve(const string &flakePath, const string &flakeLink, const string &host);

  struct result {
    bool error = false;
    vector<string> paths;
  };

  void preprocessFile(const string &filepath);
  result resolveImportStatements();
  result resolveImportsStatements();

private:
  eval ev;
  string flakePath;
  string flakeLink;
  string absoluteFilePath;
  string filePath;
  string fileStr;
  string host;
};
