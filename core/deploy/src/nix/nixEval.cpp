#include "nixEval.h"
#include "../utils/split.h"
#include "../utils/strings.h"
#include "../utils/systemHelper.h"
#include "../utils/threading.h"
#include "../utils/ttyHelper.h"
#include "nixEvalSupport.h"
#include "staticGet.h"
#include "staticRemove.h"
#include <algorithm>
#include <cstddef>
#include <filesystem>
#include <format>
#include <iostream>
#include <map>
#include <ranges>
#include <string>
#include <vector>
using namespace std;

nixEval::nixEval(const init &i) {
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

  map<string, map<string, nixEvalSupport::key>> parallelWork = {
      {"resolve", resolveMap},
      {"throw", throwMap},
      {"modulesPath",
       map<string, nixEvalSupport::key>{
           {"modulesPath",
            nixEvalSupport::key{"nix eval " + flakePath +
                                    "#nixosConfigurations." + host +
                                    "._module.specialArgs.",
                                ""}}}},
      {"specialArgs",
       map<string, nixEvalSupport::key>{
           {"specialArgs", nixEvalSupport::key{"nix eval " + flakePath +
                                                   "#nixosConfigurations." +
                                                   host + "._module.",
                                               ""}}}}};

  map<string, map<string, map<string, nixEvalSupport::key>>> parallelOut =
      threading::paralleliseMap<string, map<string, nixEvalSupport::key>,
                                map<string, map<string, nixEvalSupport::key>>,
                                decltype(&nixEvalSupport::seniorInitWorker)>(
          parallelWork, nixEvalSupport::seniorInitWorker);

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
            nixEval::throwMap.insert({item, cmd});

          } else {
            nixEval::resolveMap.insert({item, cmd});
          }
        }
        continue;
      }
      // must be from throw or resolve at this point
      nixEval::inheritMap.insert({itemsParent, items});
    }
  }
}

void nixEval::preProcessFile(string fileStr, string filePath) {

  nixEval::fileStr = staticRemove::comments(fileStr);
  nixEval::filePath = filePath;
  nixEval::absoluteFilePath = flakePath + filePath;

  // make pretty string for error logs
  vector<string> lineFile = split::splitStrByChar(fileStr, '\n');

  nixEval::prettyFile = {};
  for (int i = 0; i < lineFile.size(); i++) {
    string line = lineFile[i];

    nixEval::prettyFile.push_back("\033[35m" + format("{:4}", i + 1) +
                                  ":\033[0m " + line + "\n");
  }
  return;
}

nixEval::result nixEval::makeCommandStr(const string attrset,
                                        const vector<string> attrsetKeys,
                                        const bool canThrow) {

  // it doesn't matter that the commandcache is per class / per thread (each
  // thread has it's own class) cause cmd are mostly unique between hosts.
  for (nixEval::commandStrCache cacheItem : nixEval::commandCache) {
    if (cacheItem.attrset != attrset || cacheItem.attrsetKeys != attrsetKeys)
      continue;
    if (cacheItem.throwable == true && canThrow == true)
      return {.thrown = true};

    return {.type = "command", .str = cacheItem.command};
  }

  struct keyCandidate {
    string start;
    string end;
    string topAttr;
  };

  vector<keyCandidate> candidates;

  if (nixEval::resolveMap.count(attrsetKeys[0])) {
    candidates.push_back({nixEval::resolveMap[attrsetKeys[0]].start,
                          nixEval::resolveMap[attrsetKeys[0]].end});
  }
  if (nixEval::throwMap.count(attrsetKeys[0])) {
    candidates.push_back({nixEval::throwMap[attrsetKeys[0]].start,
                          nixEval::throwMap[attrsetKeys[0]].end,
                          attrsetKeys[0]});
  }
  for (auto &[key, value] : nixEval::inheritMap) {
    if (value.count(attrsetKeys[0])) {
      candidates.push_back(
          {value[attrsetKeys[0]].start, value[attrsetKeys[0]].end, key});
    }
  }
  if (candidates.size() > 1) {
    vector<staticGet::candidate> testingCandidates;
    for (keyCandidate candidate : candidates) {
      testingCandidates.push_back(
          {attrsetKeys, candidate.start, candidate.end});
    }
    vector<bool> validCandidates =
        threading::paralleliseVector<staticGet::candidate, bool,
                                     decltype(&staticGet::filterCandidate)>(
            testingCandidates, staticGet::filterCandidate);
    for (int i = 0; i < validCandidates.size(); i++) {
      if (validCandidates[i] == true) {
        continue;
      }
      validCandidates.erase(validCandidates.begin() + i);
      candidates.erase(candidates.begin() + i);
      i--;
    }
  }

  if (candidates.size() > 1) {
    cerr << ttyHelper::error(
        "Mutiple candidates found for attrset (\033[35m" + attrset +
        "\033[0m).\n Please implement\033[94m lambda input parsing\033[0m to "
        "determine the\033[94m winning candidate\033[0m. \n"
        "( translation: screw you, sincerely, past Reesey ) \033[35m:3\033[0m");
    // 12/06/26 - AR - if you ever run into this error and fix this properly
    // then you should drop the filterCandidate thing and the commandCache
    // thing. As the are very compute expensive ways of trying to avoid solving
    // this issue for real. (as shown by the fact theirs a cache). If you do
    // this for real I expect that it will be pure string manipulation that in
    // comparison to nix eval is blazing fast.

    // 14/07/26 - AR - I'm sorry past me. I might do both.

    nixEval::result res;
    res.error = true;
    return res;
  }

  if (candidates.size() == 0) {
    cerr << ttyHelper::error(
        "no candidates found for Attrset (\033[35m" + attrset +
        "\033[0m)\n Please implement more parsing for maps. "
        "It is currently missing parsing for let ... in.");
    nixEval::result res;
    res.error = true;
    return res;
  }

  keyCandidate candidate = candidates[0];

  bool throwable = false;
  if (nixEval::throwMap.count(candidate.topAttr)) {
    throwable = true;
  }
  nixEval::commandCache.push_back({attrset, attrsetKeys, throwable,
                                   candidate.start + attrset + candidate.end});
  if (throwable == true && canThrow == true) {
    nixEval::result res;
    res.thrown = true;
    return res;
  }

  return {.type = "command", .str = candidate.start + attrset + candidate.end};
}

nixEval::result nixEval::statement(string test, bool canThrow) {

  test = strings::trim(test);

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
    return nixEval::bracket(test);
  }

  vector<string> list = staticGet::listItems(test);
  if (list.size() != 0) {
    // is list
    nixEval::result res;
    for (string item : list) {
      nixEval::result hold = nixEval::statement(item);
      if (hold.error == true) {
        nixEval::result tmp;
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

  string path = nixEval::path(test);
  if (path != "") {
    // is path
    result res;
    res.str = path;
    res.type = "path";
    res.error = false;
    return res;
  }

  nixEval::result hold;
  hold = nixEval::attrsetKey(test, canThrow);
  if (hold.error == false) {
    return hold;
  }

  hold = nixEval::lambdaCall(test, canThrow);
  if (hold.error == false) {
    return hold;
  }

  // error land
  if (canThrow == false) {
    result res;
    res.error = true;
    return res;
  }
  cerr << "\n" + ttyHelper::error("Failed to resolve the following in "
                                  "(\033[35m" +
                                  nixEval::flakeLink + nixEval::filePath +
                                  "\033[0m)");
  string errorCode;
  vector<string> tokenTest = split::splitStrByChar(test, '\n');
  for (int i = 0; i < nixEval::prettyFile.size(); i++) {

    if (nixEval::prettyFile[i].find(tokenTest[0]) == string::npos) {
      continue;
    }

    for (int j = i - 2; j < i + tokenTest.size() + 2; j++) {
      string line = nixEval::prettyFile[j];

      if (i == j) {
        line = strings::replace(line, "\n", "");

        line += "    \033[31m<---\033[0m\n";
      }

      errorCode += line;
    }
  }
  if (errorCode.size() == 0) {
    errorCode = strings::trim(test) + "\n";
  }
  cerr << errorCode;
  result res;
  res.error = true;
  return res;
}
string nixEval::path(string test) {
  if (test.find(" ") != string::npos) {
    test = test.substr(0, test.find(" "));
  }

  size_t pos = test.find('/');
  if (pos != string::npos) {
    string firstItem = test.substr(0, pos);

    string absoluteFolderPath = nixEval::absoluteFilePath.substr(
        0, nixEval::absoluteFilePath.rfind('/'));

    vector<string> folders;
    for (const filesystem::directory_entry &entry :
         filesystem::directory_iterator(absoluteFolderPath)) {
      if (entry.is_directory()) {
        folders.push_back(entry.path().filename().string());
      }
    }

    if (ranges::contains(folders, firstItem)) {
      // for relative filepaths like folder/file
      test = absoluteFolderPath + "/" + test;
    }
  }

  if (test[0] == '/') {
    // is absolute file path
    if (test.rfind(nixEval::flakePath, 0) == 0) {
      test = strings::replace(test, nixEval::flakePath, "");
      return test;
    }
    if (test.rfind(nixEval::flakeLink, 0) == 0) {
      test = strings::replace(test, nixEval::flakeLink, "");
      return test;
    }
  }

  if (pos != string::npos) {
    if (test[0] == '.') {
      // relative file path

      std::filesystem::path declaredIn = absoluteFilePath;
      std::filesystem::path relative = test;
      string path =
          filesystem::weakly_canonical(declaredIn.parent_path() / relative)
              .string();

      if (path.rfind(nixEval::flakePath, 0) == 0) {
        path = strings::replace(path, nixEval::flakePath, "");
        return path;
      }
      if (path.rfind(nixEval::flakeLink, 0) == 0) {
        return path;
      }
    }
  }
  return "";
}
nixEval::result nixEval::attrsetKey(string test, bool canThrow) {
  nixEval::result res;
  string attrset;

  // does preprocessing to resolve funny statements like ${ } and ( ) and
  // get a clean attrset Split
  vector<string> attrsetKeys = staticGet::tokenizedTopLevel(test);
  // go though key by key and resolve thing like ${ }
  for (int i = 0; i < attrsetKeys.size(); i++) {
    string attrsetKey = strings::trim(attrsetKeys[i]);

    // resolve ${ }
    if (attrsetKey.find("${") != string::npos && attrsetKey.back() == '}') {
      attrsetKey = strings::replace(attrsetKey, "${", "");
      attrsetKey = strings::rReplace(attrsetKey, "}", "");

      nixEval::result hold = nixEval::statement(attrsetKey, false);
      if (hold.error == true) {
        return {true};
      }
      attrsetKey = hold.str;
    }
    attrsetKeys[i] = attrsetKey;
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
  nixEval::result cmdStr =
      nixEval::makeCommandStr(attrset, attrsetKeys, canThrow);
  if (cmdStr.thrown == true || cmdStr.error == true) {
    return {cmdStr.error, cmdStr.thrown};
  }
  // run cmd (nix eval)
  systemHelper::result cmdOut = systemHelper::runCommand(cmdStr.str);
  if (cmdOut.exitCode != 0 &&
      cmdOut.error.find("evaluation warning:") == string::npos) {
    return {true};
  }
  // parse output
  nixEval::result hold = nixEval::statement(cmdOut.output, false);
  if (hold.error == true) {
    res.error = true;
    return res;
  }
  return hold;
}
nixEval::result nixEval::bracket(string test) {
  // input check
  if (test.front() != '(' && test.back() != ')') {
    cerr << ttyHelper::error(
        "Non bracket has been inserted into the bracket function");
  }

  // remove the starting and end brackets eg: "(thing)" -> "thing"
  test.erase(test.begin());
  test.pop_back();

  // throw error on unsupported statements seeing as this isn't a real
  // parser. (and I don't want it to be)
  string mask = test;
  mask = strings::blankWithinTokens(mask, "\"", "\"");
  mask = strings::blankWithinTokens(mask, "(", ")");
  mask = strings::blankWithinTokens(mask, "[", "]");
  vector<string> operators = {"-", "?", "++", "*",  "!",  "//",
                              "<", ">", "==", "!=", "&&", "||"};
  for (string value : operators) {
    if (mask.find(value) != string::npos) {
      cerr << ttyHelper::error("Bracket has unsupported operators");
      nixEval::result res;
      res.error = true;
      return res;
    }
  }

  // process into str
  vector<string> items = split::splitStrByChar(test, '+');
  test = "";
  for (string item : items) {
    nixEval::result hold = nixEval::statement(item, true);
    if (hold.error == true) {
      nixEval::result res;
      res.error = true;
      return res;
    }
    if (hold.thrown == true) {
      nixEval::result res;
      res.thrown = true;
      return res;
    }
    if (hold.type == "list") {
      cerr << ttyHelper::error("Nested lists are not supported in this "
                               "context. \033[35m:3\033[0m");
      // yes, I really had to put a colourful :3 in their. (UwU)
      // I what future me will see that and suffer... So yeah...
      nixEval::result res;
      res.error = true;
      return res;
    }
    test += hold.str;
  }

  nixEval::result res;
  res.type = "bracket";
  res.str = test;
  return res;
}

nixEval::result nixEval::lambdaCall(string test, bool canThrow) {
  vector<string> tokens = staticGet::tokenizedTopLevel(test);

  if (tokens.size() <= 1) {
    return {.type = "skip"};
  }

  nixEval::result cmdStr =
      nixEval::makeCommandStr(tokens[0], {tokens[0]}, canThrow);
  if (cmdStr.thrown == true || cmdStr.error == true) {
    return {cmdStr.error, cmdStr.thrown};
  }

  cerr << ttyHelper::error(
      "Function cannot be thrown. Please implement function processing");
  return {true};
}
