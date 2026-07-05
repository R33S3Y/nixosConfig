#include "args.h";
#include "split.h"
#include "strings.h"
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

map<string, args::optionOut>
args::parse(vector<string> userInput, map<string, args::optionIn> argValues) {
  string userInputStr;
  for (string str : userInput) {
    userInputStr += " " + str;
  }
  if (userInputStr.size() == 0) {
    throw invalid_argument("No user args inputs");
  }

  map<string, args::optionOut> output;

  // resplitting and triming like this means that we dont need to worry about
  // = signs it all gets split the same
  userInput = split::splitStrByChars(userInputStr, {' ', '='});
  for (string &token : userInput)
    token = strings::trim(token);
  userInput.push_back("-");

  int lastArgToken;
  for (int i = userInput.size() - 2; i != 0; i--) {
    if (userInput[i].starts_with("-") == true) {
      lastArgToken = i;
      break;
    }
  }

  for (int i = 0; i < lastArgToken; i++) {
    string token = userInput[i];

    if (token.starts_with("-") == false) {
      continue;
    }

    bool longArg = true;
    if (token.starts_with("--") == false) {
      longArg = false;
    }

    token = strings::replaceAll(token, "-", "");
    string argName;
    args::optionIn argInfo;
    for (auto &[preposedArgName, preposedArgInfo] : argValues) {
      if (token.starts_with(preposedArgInfo.longName) && longArg == true) {
        argName = preposedArgName;
        argInfo = preposedArgInfo;
        break;
      }
      if (preposedArgInfo.shortName.has_value() == true &&
          token.starts_with(*preposedArgInfo.shortName) && longArg == false) {
        argName = preposedArgName;
        argInfo = preposedArgInfo;
        break;
      }
    }
  }

  // find --longname
  for (vector<string>::iterator it = userInput.begin();
       it != userInput.end() - 1; it++) {
    if (*it != "--" + curArgInfo.longName) {
      continue;
    }

    // if is maybe invoked
    if (userInputStr.find(curArgInfo.longName) == string::npos &&
        (!curArgInfo.shortName.has_value() ||
         userInputStr.find(*curArgInfo.shortName) == std::string::npos)) {
      // if this is true arg is not in
      output[curArg] = {
          .longName = curArgInfo.longName,
          .invoked = false,
          .shortName = *curArgInfo.shortName,
      };
      continue;
    }

    string value = "";
    if (*it++.starts_with("-") == false && curArgInfo.takesValue == true) {
      value = *it++;
      userInput.erase(it++);
    } else if (curArgInfo.takesValue == true) {
      // arg needed but not found
      throw invalid_argument("Arg: --" + curArgInfo.longName +
                             " takes a value");
    }
  }
  return output;
}
