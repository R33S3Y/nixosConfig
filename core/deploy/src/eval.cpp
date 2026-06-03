#include "eval.h"
#include "utils.h"
#include <cstddef>
#include <filesystem>
#include <format>
#include <iostream>
#include <iterator>
#include <string>
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
      {"builtins", {"nix eval --expr 'builtins", "'"}},
      {"inputs",
       {"nix eval " + flakePath + "#nixosConfigurations." + host +
            "._module.specialArgs.",
        ""}},
  };

  // maybe this can be done in parallel
  bool canThrowModulesPath = false;
  utils::result cmdOut;
  cmdOut = utils::runCommand("nix eval " + flakePath + "#nixosConfigurations." +
                             host + "._module.specialArgs.modulesPath");
  if (!cmdOut.ok()) {
    cerr << utils::error("Failed to eval special args");
  }
  if (cmdOut.output.find("/nix/store/") != string::npos)
    canThrowModulesPath = true;

  cmdOut = utils::runCommand("nix eval " + flakePath + "#nixosConfigurations." +
                             host +
                             "._module.specialArgs --apply builtins.attrNames");

  if (!cmdOut.ok()) {
    cerr << utils::error("Failed to eval special args");
  }
  vector<string> list = eval::list(cmdOut.output);
  for (string item : list) {
    item = utils::replaceAll(item, "\"", "");
    if (item == "inputs")
      continue;
    if (canThrowModulesPath == true && item == "modulesPath") {
      eval::throwMap.insert(
          {item,
           {"nix eval " + flakePath + "#nixosConfigurations." + host +
                "._module.specialArgs.",
            ""}});
      continue;
    }
    eval::resolveMap.insert(
        {item,
         {"nix eval " + flakePath + "#nixosConfigurations." + host +
              "._module.specialArgs.",
          ""}});
  }
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
vector<string> eval::list(string test, bool throwLazy) {

  // mask and split
  string mask = test;
  mask = utils::blankWithinTokens(mask, "${", "}");
  mask = utils::blankWithinTokens(mask, "(", ")");
  vector<string> listItems = utils::splitStrByCharByFilterStr(test, mask, ' ');

  // is list and cleanup
  if (listItems.size() == 0) {
    return {};
  }
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

eval::result eval::statement(string test, bool canThrow) {

  test = utils::trim(test);

  if (test.front() == '\"' && test.back() == '\"') {
    // is string
    test.erase(test.begin());
    test.pop_back();

    result res;
    res.error = false;
    res.type = "str";
    res.str = test;
    return res;
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
      } else {
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

  // does preproccessing to resolve funny statements like ${ } and ( ) and get
  // a clean attrset Split
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
        res.error = true;
        break;
      }
      attrsetKey = hold.str;
    }

    // resolve ( )
    if (attrsetKey.front() == '(' && attrsetKey.back() == ')') {
      attrsetKey.erase(attrsetKey.begin());
      attrsetKey.pop_back();

      string mask = attrsetKey;
      mask = utils::blankWithinTokens(mask, "\"", "\"");
      mask = utils::blankWithinTokens(mask, "(", ")");
      mask = utils::blankWithinTokens(mask, "[", "]");
      vector<string> operators = {"-", "?", "++", "*",  "!",  "//",
                                  "<", ">", "==", "!=", "&&", "||"};
      for (string value : operators) {
        if (mask.find(value) != string::npos) {
          cerr << utils::error("Bracket has unsupported operators");
          res.error = true;
          return res;
        }
      }

      vector<string> items = utils::splitStrByChar(attrsetKey, '+');
      attrsetKey = "";
      for (string item : items) {
        eval::result hold = eval::statement(item);
        if (hold.error == true) {
          res.error = true;
          return res;
        }
        if (hold.thrown == true) {
          res.thrown = true;
          return res;
        }
        if (hold.type == "list") {
          cerr << utils::error("Nested lists are not supported in this "
                               "context. \033[35m:3\033[0m\n");
          // yes, I really had to put a colourful :3 in their. (UwU)
          // I what future me will see that and suffer... So yeah...
          res.error = true;
          return res;
        }
        attrsetKey += hold.str;
      }
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

  // make cmd
  string cmd;
  if (resolveMap.count(attrsetKeys[0])) {
    cmd =
        resolveMap[attrsetKeys[0]][0] + attrset + resolveMap[attrsetKeys[0]][1];
  }
  if (throwMap.count(attrsetKeys[0])) {
    if (canThrow == false)
      cmd = throwMap[attrsetKeys[0]][0] + attrset + throwMap[attrsetKeys[0]][1];
    else {
      res.thrown = true;
      return res;
    }
  }
  if (cmd == "") {
    res.error = true;
    return res;
  }

  utils::result cmdOut = utils::runCommand(cmd);
  if (!cmdOut.ok()) {
    res.error = true;
    return res;
  }

  eval::result hold = eval::statement(cmdOut.output, false);

  if (hold.error == true) {
    res.error = true;
    return res;
  }
  return hold;
}
