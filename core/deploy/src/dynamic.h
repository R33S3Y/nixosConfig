#pragma once

#include <string>
#include <vector>
using namespace std;

class dynamic {
public:
  bool rebuild(const string &host, const string &flakePath,
               const string &flakeLink, const vector<string> &gitDiff);

private:
  static vector<string> getNixFiles(const string &flakePath, const string host);
  static vector<string> filter(vector<string> imports,
                               vector<string> processedFiles);
  static vector<string> merge(vector<string> imports,
                              vector<string> unprocessedFiles);
};
