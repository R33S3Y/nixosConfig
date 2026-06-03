#include "resolve.h"
#include "eval.h"
#include "utils.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

resolve::resolve(const string &flakePath, const string &flakeLink,
                 const string &host)
    : ev({flakePath, flakeLink, host}), flakePath(flakePath),
      flakeLink(flakeLink), host(host) {}

void resolve::preprocessFile(const string &filePath) {
  resolve::filePath = filePath;
  resolve::absoluteFilePath = flakePath + filePath;

  string rawFileStr = utils::readFile(flakePath + filePath);
  vector<string> lineFile = utils::splitStrByChar(rawFileStr, '\n');

  resolve::fileStr = eval::removeComments(rawFileStr);

  ev.preProcessFile(rawFileStr, filePath);

  return;
}

resolve::result resolve::resolveImportStatements() {
  string workingFileStr = fileStr;

  result res;
  while (workingFileStr.length() > 0) {
    size_t pos = workingFileStr.find("import ");
    if (pos == string::npos) {
      break;
    }

    workingFileStr = workingFileStr.substr(pos);

    size_t lineEnd = workingFileStr.find(";");
    string lineStr = workingFileStr.substr(0, lineEnd);
    workingFileStr = workingFileStr.substr(lineEnd);

    lineStr = utils::replace(lineStr, "import ", "");
    lineStr = utils::replaceAll(lineStr, ";", "");
    lineStr = utils::trim(lineStr);

    eval::result tmp = ev.statement(lineStr);
    if (tmp.str != "") {
      res.paths.push_back(tmp.str);
    }
    if (tmp.error) {
      res.error = true;
      break; // see the imports statements func for why we break
    }
  }

  return res;
}

size_t getValidStatementPos(string statement, string s) {

  while (s.find(statement) != string::npos) {
    size_t pos = s.find(statement);

    s.replace(pos, statement.size(), statement.size(), '.');

    vector<char> validChars = {'(', ')', '{', '}'};
    bool validStart = false;
    if (pos == 0) {
      validStart = true;
    } else {
      if (isspace(s[pos - 1]) || find(validChars.begin(), validChars.end(),
                                      s[pos - 1]) != validChars.end()) {
        validStart = true;
      }
    }

    bool validEnd = false;
    if (pos + statement.size() >= s.size()) {
      validEnd = true;
    } else {
      if (isspace(s[pos + statement.size()]) ||
          find(validChars.begin(), validChars.end(),
               s[pos + statement.size()]) != validChars.end()) {
        validEnd = true;
      }
    }

    if (validEnd && validStart) {
      return pos;
    }
  }

  return string::npos;
}

resolve::result resolve::resolveImportsStatements() {
  string workingFileStr = resolve::fileStr;

  result res;
  // remove let in statement seeing as they are unable to contain a valid
  // imports statement
  size_t letPos = getValidStatementPos("let", workingFileStr);
  size_t inPos = string::npos;
  if (letPos != string::npos) {
    inPos = getValidStatementPos("in", workingFileStr.substr(letPos)) + letPos;
  }

  while (letPos != string::npos && inPos != string::npos) {
    workingFileStr =
        workingFileStr.substr(0, letPos) + workingFileStr.substr(inPos + 2);

    letPos = getValidStatementPos("let", workingFileStr);
    inPos = string::npos;
    if (letPos != string::npos) {
      inPos =
          getValidStatementPos("in", workingFileStr.substr(letPos)) + letPos;
    }
  }

  while (workingFileStr.length() > 0) {
    size_t pos = workingFileStr.find("imports");
    if (pos == string::npos) {
      break;
    }
    workingFileStr = workingFileStr.substr(pos);

    size_t lineEnd = workingFileStr.find(";");

    string lineStr = workingFileStr.substr(0, lineEnd);
    cout << "import line:" + lineStr + "\n";
    workingFileStr = workingFileStr.substr(lineEnd);

    lineStr = utils::replaceAll(lineStr, "imports", "");

    if (lineStr[0] !=
        '=') { // incase you end up with: thing = var.imports ++ [];
      continue;
    }
    lineStr = utils::replaceAll(lineStr, "=", "");
    lineStr = utils::trim(lineStr);

    eval::result tmp = ev.statement(lineStr);
    if (tmp.error == true) {
      // errors for resolvekey only happen when it falls to resolve something.
      // If we fail to resolve something then we will need to rebuild the
      // host no matter what be cause their is no longer any guaranty's as to
      // what files the host needs
      res.error = true;
      return res;
    }
    if (tmp.thrown == true) {
      continue;
    }
    if (tmp.type == "list") {
      res.paths.insert(res.paths.end(), tmp.list.begin(), tmp.list.end());
    } else {
      cout << tmp.str + "\n";
      res.paths.push_back(tmp.str);
    }
  }
  return res;
}
