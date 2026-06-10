#include "eval.h"
#include "threading.h"
#include "utils.h"
#include <cstddef>
#include <filesystem>
#include <format>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <syncstream>
#include <vector>
using namespace std;

eval::eval(const init &i) {
  flakePath = i.flakePath;
  flakeLink = i.flakeLink;
  host = i.host;

  resolveMap = {
      {"config",
       {"nix eval " + flakePath + "#nixosConfigurations." + host + ".", ""}},
      {"options",
       {"nix eval " + flakePath + "#nixosConfigurations." + host + ".", ""}},
      {"lib",
       {"nix eval " + flakePath + "#nixosConfigurations." + host + ".", ""}},
      {"pkgs",
       {"nix eval " + flakePath + "#nixosConfigurations." + host + ".", ""}},
  };
  throwMap = {
      {"builtins", {"nix eval --expr '", "'"}},
      {"inputs",
       {"nix eval " + flakePath + "#nixosConfigurations." + host +
            "._module.specialArgs.",
        ""}},
  };

  map<string, map<string, key>> parallelWork = {
      {"resolve", resolveMap},
      {"throw", throwMap},
      {"modulesPath",
       map<string, eval::key>{
           {"modulesPath",
            eval::key{"nix eval " + flakePath + "#nixosConfigurations." + host +
                          "._module.specialArgs.",
                      ""}}}},
      {"specialArgs",
       map<string, eval::key>{
           {"specialArgs",
            eval::key{"nix eval " + flakePath + "#nixosConfigurations." + host +
                          "._module.",
                      ""}}}}};

  map<string, map<string, map<string, eval::key>>> parallelOut =
      threading::paralleliseMap<string, map<string, eval::key>,
                                map<string, map<string, eval::key>>,
                                decltype(&eval::seniorInitWorker)>(
          parallelWork, eval::seniorInitWorker);

  bool canThrowModulesPath = false;
  // yes really
  if (parallelOut["modulesPath"]["modulesPath"]["modulesPath"].start.find(
          "/nix/store/") != string::npos)
    canThrowModulesPath = true;
  parallelOut.erase("modulesPath");

  for (auto &[itemsType, valueTop] : parallelOut) {
    for (auto &[itemsParent, items] : valueTop) {
      if (itemsType == "specialArgs") {
        for (auto &[item, cmd] : items) {
          if (canThrowModulesPath == true && item == "modulesPath") {
            eval::throwMap.insert({item, cmd});

          } else {
            eval::resolveMap.insert({item, cmd});
          }
        }
        continue;
      }
      // must be from throw or resolve at this point
      eval::inheritMap.insert({itemsParent, items});
    }
  }
}
map<string, map<string, eval::key>>
eval::seniorInitWorker(map<string, eval::key> input) {

  map<string, map<string, eval::key>> hold;
  for (auto &[key, value] : input) {
    hold.insert({key, {{key, value}}});
  }

  return threading::paralleliseMap<string, map<string, eval::key>,
                                   map<string, eval::key>,
                                   decltype(&eval::juniorInitWorker)>(
      hold, eval::juniorInitWorker);
}
map<string, eval::key> eval::juniorInitWorker(map<string, eval::key> input) {
  string key = input.begin()->first;
  eval::key value = input[key];

  string cmd = value.start + key + value.end;
  if (key != "modulesPath") {
    cmd = cmd + " --apply builtins.attrNames";
  }

  utils::result cmdOut = utils::runCommand(cmd);

  if (cmdOut.exitCode != 0) {
    ostringstream oss;
    oss << utils::error("Failed to eval attrs of: (\033[35m" + key +
                        "\033[0m)");
    cerr << oss.str();

    return {};
  }

  if (key == "modulesPath") {
    return {{{key, {cmdOut.output, ""}}}};
  }

  map<string, eval::key> output;
  vector<string> list = eval::list(cmdOut.output);
  for (int i = 0; i < list.size(); i++) {
    list[i] = utils::replaceAll(list[i], "\"", "");
    if (list[i] == "inputs")
      continue;
    output.insert({list[i], {value.start + key + ".", value.end}});
  }

  return output;
}

void eval::preProcessFile(string fileStr, string filePath) {

  eval::fileStr = removeComments(fileStr);
  eval::filePath = filePath;
  eval::absoluteFilePath = flakePath + filePath;

  // make pretty string for error logs
  vector<string> lineFile = utils::splitStrByChar(fileStr, '\n');

  eval::prettyFile = {};
  for (int i = 0; i < lineFile.size(); i++) {
    string line = lineFile[i];

    eval::prettyFile.push_back("\033[35m" + format("{:4}", i + 1) +
                               ":\033[0m " + line + "\n");
  }
  return;
}
string eval::removeComments(string fileStr) {

  // gets the things before the strings are moved
  vector<string> lineFile = utils::splitStrByChar(fileStr, '\n');

  // removes the contents inside str
  fileStr = utils::blankWithinTokens(fileStr, "\"");
  fileStr = utils::blankWithinTokens(fileStr, "''");

  // removes  comments from filestr so it can be useful
  vector<string> stringlessLineFile = utils::splitStrByChar(fileStr, '\n');
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
eval::result eval::makeCommandStr(string attrset, vector<string> attrsetKeys,
                                  bool canThrow) {
  struct keyCandidate {
    string start;
    string end;
    string topAttr;
  };
  vector<keyCandidate> candidates;

  if (eval::resolveMap.count(attrsetKeys[0])) {
    candidates.push_back({eval::resolveMap[attrsetKeys[0]].start,
                          eval::resolveMap[attrsetKeys[0]].end});
  }
  if (eval::throwMap.count(attrsetKeys[0])) {
    candidates.push_back({eval::throwMap[attrsetKeys[0]].start,
                          eval::throwMap[attrsetKeys[0]].end, attrsetKeys[0]});
  }
  for (auto &[key, value] : eval::inheritMap) {
    if (value.count(attrsetKeys[0])) {
      candidates.push_back(
          {value[attrsetKeys[0]].start, value[attrsetKeys[0]].end, key});
    }
  }
  for (keyCandidate candidate : candidates) {
    cout << candidate.start + "\n";
  }
  if (candidates.size() > 1) {
    vector<eval::candidate> testingCandidates;
    for (keyCandidate candidate : candidates) {
      testingCandidates.push_back(
          {attrsetKeys, candidate.start, candidate.end});
    }
    vector<bool> validCandidates =
        threading::paralleliseVector<eval::candidate, bool,
                                     decltype(&eval::filterCandidate)>(
            testingCandidates, eval::filterCandidate);
    for (int i = 0; i < validCandidates.size(); i++) {
      if (validCandidates[i] == true)
        continue;
      validCandidates.erase(validCandidates.begin() + i);
      candidates.erase(candidates.begin() + i);
      i--;
    }
  }
  if (candidates.size() > 1) {
    cerr << utils::error(
        "Mutiple candidates found for attrset (\033[35m" + attrset +
        "\033[0m).\n Please implement\033[94m lambda input parsing\033[0m to "
        "determine the\033[94m winning candidate\033[0m. \n"
        "( translation: screw you, sincerely, past Reesey ) \033[35m:3\033[0m");

    eval::result res;
    res.error = true;
    return res;
  }

  keyCandidate candidate = candidates[0];

  if (eval::throwMap.count(candidate.topAttr) && canThrow == true) {
    eval::result res;
    res.thrown = true;
    return res;
  }
  if (candidates.size() == 0) {
    cerr << utils::error("no candidates found for Attrset (\033[35m" + attrset +
                         "\033[0m)\n Please implement more parsing for maps. "
                         "It is currently missing parsing for let ... in.");
    eval::result res;
    res.error = true;
    return res;
  }
  cout << candidate.start + attrset + candidate.end + "\n";
  return {.type = "command", .str = candidate.start + attrset + candidate.end};
}
bool eval::filterCandidate(eval::candidate testingCandidate) {
  if (testingCandidate.attrsetKeys.size() <= 1) {
    return true; // could be anything. So yes valid
  }

  string attrsetPath = testingCandidate.attrsetKeys[0];
  for (int i = 1; i < testingCandidate.attrsetKeys.size(); i++) {
    string cmd = testingCandidate.start + attrsetPath + testingCandidate.end;

    utils::result cmdType = utils::runCommand(cmd + " --apply builtins.typeOf");
    if (cmdType.exitCode != 0 &&
        cmdType.error.find("evaluation warning:") == string::npos) {
      return true; // if we are not sure assume valid
    }
    if (utils::trim(cmdType.output) != "\"set\"") {
      cout << "throw: " + cmd + "\n";
      return false;
    }

    utils::result cmdItems =
        utils::runCommand(cmd + " --apply builtins.attrNames");
    if (cmdItems.exitCode != 0 &&
        cmdType.error.find("evaluation warning:") == string::npos) {
      return true; // if we are not sure assume valid
    }

    vector<string> setList = eval::list(cmdItems.output);
    bool found = false;
    for (string setItem : setList) {
      if (utils::trim(setItem) == "\"" + testingCandidate.attrsetKeys[i] + "\"")
        found = true;
    }
    if (found == false) {
      return false;
    }
    attrsetPath += "." + testingCandidate.attrsetKeys[i];
  }
  return true;
}

eval::result eval::statement(string test, bool canThrow) {

  test = utils::trim(test);

  if (test.front() == '\"' && test.back() == '\"') {
    // is string

    // cut quotes eg: "thing" -> thing
    test.erase(test.begin());
    test.pop_back();

    result res;
    res.error = false;
    res.type = "str";
    res.str = test;
    return res;
  }

  if (test.front() == '(' && test.back() == ')') {
    // is bracket
    return eval::bracket(test);
  }

  vector<string> list = eval::list(test);
  if (list.size() != 0) {
    // is list
    eval::result res;
    for (string item : list) {
      eval::result hold = eval::statement(item);
      if (hold.error == true) {
        eval::result tmp;
        tmp.error = true;
        return tmp;
      }
      if (hold.type == "list") {
        res.list.insert(res.list.end(), hold.list.begin(), hold.list.end());
      } else if (hold.str.size() != 0) {
        res.list.push_back(hold.str);
      }
    }
    res.type = "list";
    return res;
  }

  string path = eval::path(test);
  if (path != "") {
    // is path
    result res;
    res.str = path;
    res.type = "path";
    res.error = false;
    return res;
  }

  eval::result hold = eval::attrsetKey(test, canThrow);
  if (hold.error == false) {
    return hold;
  }

  // error land
  if (canThrow == false) {
    result res;
    res.error = true;
    return res;
  }
  cerr << "\n" + utils::error("Failed to resolve the following in "
                              "(\033[35m" +
                              eval::flakeLink + eval::filePath + "\033[0m)");
  string errorCode;
  vector<string> tokenTest = utils::splitStrByChar(test, '\n');
  for (int i = 0; i < eval::prettyFile.size(); i++) {

    if (eval::prettyFile[i].find(tokenTest[0]) == string::npos) {
      continue;
    }

    for (int j = i - 2; j < i + tokenTest.size() + 2; j++) {
      string line = eval::prettyFile[j];

      if (i == j) {
        line = utils::replace(line, "\n", "");

        line += "    \033[31m<---\033[0m\n";
      }

      errorCode += line;
    }
  }
  if (errorCode.size() == 0) {
    errorCode = utils::trim(test) + "\n";
  }
  cerr << errorCode;
  result res;
  res.error = true;
  return res;
}
string eval::path(string test) {
  if (test.find(" ") != string::npos) {
    test = test.substr(0, test.find(" "));
  }

  if (test[0] == '/') {
    // is absolute file path
    if (test.rfind(eval::flakePath, 0) == 0) {
      test = utils::replace(test, eval::flakePath, "");
      return test;
    }
    if (test.rfind(eval::flakeLink, 0) == 0) {
      test = utils::replace(test, eval::flakeLink, "");
      return test;
    }
  }

  string absoluteFolderPath =
      eval::absoluteFilePath.substr(0, eval::absoluteFilePath.rfind('/'));
  vector<string> folders;
  for (auto &entry : std::filesystem::directory_iterator(absoluteFolderPath)) {
    if (entry.is_directory())
      folders.push_back(entry.path().string());
  }

  size_t pos = test.find('/');
  if (pos != string::npos) {

    string firstItem = test.substr(pos);

    if (test[0] == '.' ||
        find(folders.begin(), folders.end(), firstItem) != folders.end()) {
      // relative file path

      std::filesystem::path declaredIn = absoluteFilePath;
      std::filesystem::path relative = test;
      string path =
          filesystem::weakly_canonical(declaredIn.parent_path() / relative)
              .string();

      if (path.rfind(eval::flakePath, 0) == 0) {
        path = utils::replace(path, eval::flakePath, "");
        return path;
      }
      if (path.rfind(eval::flakeLink, 0) == 0) {
        return path;
      }
    }
  }
  return "";
}
eval::result eval::attrsetKey(string test, bool canThrow) {
  eval::result res;
  string attrset;

  // does preprocessing to resolve funny statements like ${ } and ( ) and
  // get a clean attrset Split
  string mask = test;
  mask = utils::blankWithinTokens(mask, "${", "}");
  mask = utils::blankWithinTokens(mask, "(", ")");
  mask = utils::blankWithinTokens(mask, "[", "]");
  vector<string> attrsetKeys =
      utils::splitStrByCharByFilterStr(test, mask, '.');

  // go though key by key and resolve thing like ${ }
  for (int i = 0; i < attrsetKeys.size(); i++) {
    string attrsetKey = utils::trim(attrsetKeys[i]);

    // resolve ${ }
    if (attrsetKey.find("${") != string::npos && attrsetKey.back() == '}') {
      attrsetKey = utils::replace(attrsetKey, "${", "");
      attrsetKey = utils::rReplace(attrsetKey, "}", "");

      eval::result hold = eval::statement(attrsetKey, false);
      if (hold.error == true) {
        return {true};
      }
      attrsetKey = hold.str;
    }

    attrset += attrsetKey + ".";
  }
  if (res.error == true) {
    res.str = "";
    result res;
  }

  // remove trailing dot.
  if (attrset.back() == '.') {
    attrset = attrset.substr(0, attrset.size() - 1);
  }

  // get cmd str
  eval::result cmdStr = eval::makeCommandStr(attrset, attrsetKeys, canThrow);
  if (cmdStr.thrown == true || cmdStr.error == true) {
    return {cmdStr.error, cmdStr.thrown};
  }
  // run cmd (nix eval)
  utils::result cmdOut = utils::runCommand(cmdStr.str);
  if (cmdOut.exitCode != 0 &&
      cmdOut.error.find("evaluation warning:") == string::npos) {
    return {true};
  }
  // parse output
  eval::result hold = eval::statement(cmdOut.output, false);
  if (hold.error == true) {
    res.error = true;
    return res;
  }
  return hold;
}
eval::result eval::bracket(string test) {
  // input check
  if (test.front() != '(' && test.back() != ')') {
    cerr << utils::error(
        "Non bracket has been inserted into the bracket function");
  }

  // remove the starting and end brackets eg: "(thing)" -> "thing"
  test.erase(test.begin());
  test.pop_back();

  // throw error on unsupported statements seeing as this isn't a real
  // parser. (and I don't want it to be)
  string mask = test;
  mask = utils::blankWithinTokens(mask, "\"", "\"");
  mask = utils::blankWithinTokens(mask, "(", ")");
  mask = utils::blankWithinTokens(mask, "[", "]");
  vector<string> operators = {"-", "?", "++", "*",  "!",  "//",
                              "<", ">", "==", "!=", "&&", "||"};
  for (string value : operators) {
    if (mask.find(value) != string::npos) {
      cerr << utils::error("Bracket has unsupported operators");
      eval::result res;
      res.error = true;
      return res;
    }
  }

  // process into str
  vector<string> items = utils::splitStrByChar(test, '+');
  test = "";
  for (string item : items) {
    eval::result hold = eval::statement(item, true);
    if (hold.error == true) {
      eval::result res;
      res.error = true;
      return res;
    }
    if (hold.thrown == true) {
      eval::result res;
      res.thrown = true;
      return res;
    }
    if (hold.type == "list") {
      cerr << utils::error("Nested lists are not supported in this "
                           "context. \033[35m:3\033[0m");
      // yes, I really had to put a colourful :3 in their. (UwU)
      // I what future me will see that and suffer... So yeah...
      eval::result res;
      res.error = true;
      return res;
    }
    test += hold.str;
  }

  eval::result res;
  res.type = "bracket";
  res.str = test;
  return res;
}
vector<string> eval::list(string test, bool throwLazy) {
  // mask and split
  string mask = test;
  mask = utils::blankWithinTokens(mask, "${", "}", '.');
  mask = utils::blankWithinTokens(mask, "(", ")", '.');
  vector<string> listItems =
      utils::splitStrByCharsByFilterStr(test, mask, {' ', '\n'});

  // is list and cleanup
  if (listItems.size() == 0) {
    return {};
  }
  // erase empty and trim
  for (int i = 0; i < listItems.size(); i++) {
    listItems[i] = utils::trim(listItems[i]);
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
eval::result eval::lambdaCall(string test, bool canThrow) {
  string mask = test;
  mask = utils::blankWithinTokens(mask, "{", "}");
  mask = utils::blankWithinTokens(mask, "(", ")");
  mask = utils::blankWithinTokens(mask, "[", "]");

  vector<string> tokens =
      utils::splitStrByCharsByFilterStr(test, mask, {' ', '\n'});

  eval::result cmdStr = eval::makeCommandStr(tokens[0], tokens, canThrow);
  if (cmdStr.thrown == true || cmdStr.error == true) {
    return {cmdStr.error, cmdStr.thrown};
  }

  cerr << utils::error(
      "Function cannot be thrown. Please implement function processing");
  return {true};
}
