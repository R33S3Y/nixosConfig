#include "nixEvalStatic.h"
#include "utils/split.h"
#include "utils/strings.h"
#include "utils/systemHelper.h"
#include "utils/threading.h"
#include "utils/ttyHelper.h"
#include <iostream>
#include <map>
#include <string>

bool nixEvalStatic::filterCandidate(nixEvalStatic::candidate testingCandidate) {
  // returns trues if this thing we know the thing is valid.
  if (testingCandidate.attrsetKeys.size() <= 1) {
    return true; // could be anything. So yes valid
  }

  string attrsetPath = testingCandidate.attrsetKeys[0];
  for (int i = 1; i < testingCandidate.attrsetKeys.size(); i++) {
    string cmd = testingCandidate.start + attrsetPath + testingCandidate.end;

    systemHelper::result cmdType =
        systemHelper::runCommand(cmd + " --apply builtins.typeOf");
    if (cmdType.exitCode != 0 &&
        cmdType.error.find("evaluation warning:") == string::npos) {
      return true; // if we are not sure assume valid
    }
    if (strings::trim(cmdType.output) != "\"set\"") {
      return false;
    }

    systemHelper::result cmdItems =
        systemHelper::runCommand(cmd + " --apply builtins.attrNames");
    if (cmdItems.exitCode != 0 &&
        cmdType.error.find("evaluation warning:") == string::npos) {
      return true; // if we are not sure assume valid
    }

    vector<string> setList = nixEvalStatic::list(cmdItems.output);
    bool found = false;
    for (string setItem : setList) {
      if (strings::trim(setItem) ==
          "\"" + testingCandidate.attrsetKeys[i] + "\"")
        found = true;
    }
    if (found == false) {
      return false;
    }
    attrsetPath += "." + testingCandidate.attrsetKeys[i];
  }
  return true;
}
vector<string> nixEvalStatic::tokenize(const string test) {
  string mask = test;
  mask = strings::blankWithinTokens(mask, "${", "}", '!');
  mask = strings::blankWithinTokens(mask, "{", "}", '!');
  mask = strings::blankWithinTokens(mask, "(", ")", '!');
  mask = strings::blankWithinTokens(mask, "[", "]", '!');

  vector<string> tokens =
      split::splitStrByCharsByFilterStr(test, mask, {' ', '.', '\n'});

  return tokens;
}

string nixEvalStatic::removeComments(string fileStr) {

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
vector<string> nixEvalStatic::list(string test, bool throwLazy) {
  // mask and split
  string mask = test;
  mask = strings::blankWithinTokens(mask, "${", "}", '.');
  mask = strings::blankWithinTokens(mask, "(", ")", '.');
  vector<string> listItems =
      split::splitStrByCharsByFilterStr(test, mask, {' ', '\n'});

  // is list and cleanup
  if (listItems.size() == 0) {
    return {};
  }
  // erase empty and trim
  for (int i = 0; i < listItems.size(); i++) {
    listItems[i] = strings::trim(listItems[i]);
    if (listItems[i].size() == 0) {
      listItems.erase(listItems.begin() + i);
      i--;
    }
  }
  if (listItems.size() == 0)
    return {};
  if (listItems.front() != "[" || listItems.back() != "]") {
    return {};
  }
  listItems.erase(listItems.begin());
  listItems.pop_back();

  // throw lazy items
  for (int i = 0; i < listItems.size(); i++) {
    string listItem = listItems[i];

    if (listItem.find("«") != string::npos ||
        listItem.find("»") != string::npos) {
      // is lazy item like «thunk» or «lambda @ ...» or «github:...»
      if (throwLazy == false)
        continue;
      listItems.erase(listItems.begin() + i);
      i--;
      continue;
    }
  }
  return listItems;
}

map<string, map<string, nixEvalStatic::key>>
nixEvalStatic::seniorInitWorker(map<string, nixEvalStatic::key> input) {

  map<string, map<string, nixEvalStatic::key>> hold;
  for (auto &[key, value] : input) {
    hold.insert({key, {{key, value}}});
  }

  return threading::paralleliseMap<string, map<string, nixEvalStatic::key>,
                                   map<string, nixEvalStatic::key>,
                                   decltype(&nixEvalStatic::juniorInitWorker)>(
      hold, nixEvalStatic::juniorInitWorker);
}
map<string, nixEvalStatic::key>
nixEvalStatic::juniorInitWorker(map<string, nixEvalStatic::key> input) {
  string key = input.begin()->first;
  nixEvalStatic::key value = input[key];

  string cmd = value.start + key + value.end;
  if (key != "modulesPath") {
    cmd = cmd + " --apply builtins.attrNames";
  }

  systemHelper::result cmdOut = systemHelper::runCommand(cmd);

  if (cmdOut.exitCode != 0) {
    ostringstream oss;
    oss << ttyHelper::error("Failed to eval attrs of: (\033[35m" + key +
                            "\033[0m)");
    cerr << oss.str();

    return {};
  }

  if (key == "modulesPath") {
    return {{{key, {cmdOut.output, ""}}}};
  }

  map<string, nixEvalStatic::key> output;
  vector<string> list = nixEvalStatic::list(cmdOut.output);
  for (int i = 0; i < list.size(); i++) {
    list[i] = strings::replaceAll(list[i], "\"", "");
    if (list[i] == "inputs")
      continue;
    output.insert({list[i], {value.start + key + ".", value.end}});
  }

  return output;
}
