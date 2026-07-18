#include "staticGet.h"
#include "../utils/split.h"
#include "../utils/strings.h"
#include "../utils/systemHelper.h"
#include "staticRemove.h"
#include <algorithm>
#include <cstddef>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using namespace std;

bool staticGet::filterCandidate(staticGet::evalPackage evalPackage) {
  // returns trues if this thing we know the thing is valid.
  vector<string> attrsetKeys = split::splitStrByChar(evalPackage.attrset, '.');
  if (attrsetKeys.size() <= 1) {
    return true; // could be anything. So yes valid
  }

  string attrsetPath = attrsetKeys[0];

  for (int i = 1; i < attrsetKeys.size(); i++) {
    string cmd = evalPackage.start + attrsetPath + evalPackage.end;

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

    vector<string> setList = staticGet::listItems(cmdItems.output);
    bool found = false;
    for (string setItem : setList) {
      if (strings::trim(setItem) == "\"" + attrsetKeys[i] + "\"")
        found = true;
    }
    if (found == false) {
      return false;
    }
    attrsetPath += "." + attrsetKeys[i];
  }
  return true;
}

vector<staticGet::evalPackage> staticGet::evalPackagesFromAttrsets(
    const vector<string> &attrsets,
    const vector<staticGet::evalPackage> &evalPrimitives) {

  vector<staticGet::evalPackage> output;
  output.reserve(attrsets.size());

  for (string attrset : attrsets) {
    string topAttr = attrset;
    if (attrset.find(".") != string::npos) {
      topAttr = attrset.substr(0, attrset.find("."));
    }

    for (const staticGet::evalPackage evalPrimitive : evalPrimitives) {
      if (evalPrimitive.attrset != topAttr) {
        continue;
      }

      output.push_back({attrset, evalPrimitive.start, evalPrimitive.end,
                        evalPrimitive.throwable});
    }
  }

  return output;
}

vector<string> staticGet::inputValues(string file) {

  // not 110% sure if letIn removal is needed. but it makes thinking about the
  // issue easier so I don't care
  file = staticRemove::letIn(file);

  file = file.substr(0, file.find("="));
  file = file.substr(0, file.rfind(":"));

  file = strings::replaceAll(file, ":", "");
  file = strings::trim(file);
  if (file[0] == '{' && *file.end() == '}') {
    file = file.substr(1, file.length() - 2);
  }

  return staticGet::listItems(file);
}
vector<string> staticGet::letInVariables(string file) {
  string letInStr = staticRemove::notLetIn(file);

  letInStr = strings::replace(letInStr, "let", "");
  letInStr = strings::rReplace(letInStr, "in", "");
  letInStr = strings::trim(letInStr);

  vector<string> letInItems = tokenizedTopLevel(letInStr);
}
vector<string> staticGet::inheritStatement(string file) {}

vector<string> staticGet::tokenizedTopLevel(const string test) {
  string mask = test;
  mask = strings::blankWithinTokens(mask, "\"", "\"", '!');
  mask = strings::blankWithinTokens(mask, "${", "}", '!');
  mask = strings::blankWithinTokens(mask, "{", "}", '!');
  mask = strings::blankWithinTokens(mask, "(", ")", '!');
  mask = strings::blankWithinTokens(mask, "[", "]", '!');

  vector<string> tokens =
      split::splitStrByCharsByFilterStr(test, mask, {' ', '.', '\n', ';'});

  return tokens;
}
size_t staticGet::validStatementPos(string statement, string s) {

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
vector<string> staticGet::listItems(string test, bool throwLazy) {
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
vector<string> staticGet::flakeHosts(string flakePath) {
  string cmd = "nix flake show " + flakePath + " --json";
  systemHelper::result cmdOut = systemHelper::runCommand(cmd);

  if (cmdOut.exitCode != 0) {
    return {};
  }
  auto json = nlohmann::json::parse(cmdOut.output);

  vector<string> configs;
  for (auto &[key, value] : json["nixosConfigurations"].items()) {
    configs.push_back(key);
  }

  return configs;
}
