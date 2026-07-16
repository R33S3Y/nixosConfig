#include "staticRemove.h"
#include "../utils/split.h"
#include "../utils/strings.h"
#include "staticGet.h"

string staticRemove::comments(string fileStr) {

  // gets the things before the strings are moved
  vector<string> lineFile = split::splitStrByChar(fileStr, '\n');

  // removes the contents inside str
  fileStr = strings::blankWithinTokens(fileStr, "\"");
  fileStr = strings::blankWithinTokens(fileStr, "''");

  // removes  comments from filestr so it can be useful
  vector<string> stringlessLineFile = split::splitStrByChar(fileStr, '\n');
  string output;
  for (int i = 0; i < lineFile.size(); i++) {
    string line = lineFile[i];
    if (stringlessLineFile[i].find("#") != string::npos) {
      line = line.substr(0, stringlessLineFile[i].find("#"));
    }
    output += line + "\n";
  }
  return output;
}
string staticRemove::letIn(string fileStr) {

  size_t letPos = staticGet::validStatementPos("let", fileStr);
  size_t inPos = string::npos;
  if (letPos != string::npos) {
    inPos = staticGet::validStatementPos("in", fileStr.substr(letPos)) + letPos;
  }

  while (letPos != string::npos && inPos != string::npos) {
    fileStr = fileStr.substr(0, letPos) + fileStr.substr(inPos + 2);

    letPos = staticGet::validStatementPos("let", fileStr);
    inPos = string::npos;
    if (letPos != string::npos) {
      inPos =
          staticGet::validStatementPos("in", fileStr.substr(letPos)) + letPos;
    }
  }
  return fileStr;
}
string staticRemove::notLetIn(string file) {

  size_t letPos = staticGet::validStatementPos("let", file);
  size_t letInLen = string::npos;
  if (letPos != string::npos) {
    letInLen = staticGet::validStatementPos("in", file.substr(letPos)) + 2;
  }

  if (letPos != string::npos && letInLen != string::npos) {
    file = file.substr(letPos, letInLen);
  } else {
    file = "";
  }
  return file;
}
