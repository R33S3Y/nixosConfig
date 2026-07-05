#include "args.h";
#include "split.h"
#include "strings.h"
#include <exception>
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
    string argName = "";
    args::optionIn argInfo;
    for (auto &[preposedArgName, preposedArgInfo] : argValues) {
      if (token.starts_with(preposedArgInfo.longName) && longArg == true) {
        argName = preposedArgName;
        argInfo = preposedArgInfo;
        break;
      }
      if (preposedArgInfo.shortName.has_value() == true &&
          token[0] == *preposedArgInfo.shortName && longArg == false) {
        argName = preposedArgName;
        argInfo = preposedArgInfo;
        break;
      }
    }

    if (argName.size() == 0) {
      if (longArg == true) {
        throw invalid_argument("Arg: --" + token + " is not valid.");
      }
      throw invalid_argument("Arg/s: -" + token + " is not valid.");
    }

    args::optionOut invokedOutput = {
        .longName = argInfo.longName,
        .invoked = true,
        .shortName = argInfo.shortName,
    };

    if (argInfo.takesValue == true) {
      if (userInput[i + 1].starts_with("-") == true) {
        if (longArg == true) {
          throw invalid_argument("Arg: --" + argInfo.longName +
                                 " is needs a value");
        }
        string throwable = "Arg: - is needs a value";
        throw invalid_argument(throwable.substr(0, 6) + *argInfo.shortName +
                               throwable.substr(6));
      }
      invokedOutput.value = userInput[i + 1];
      userInput.erase(userInput.begin() + i + 1);
      lastArgToken--;
    }
  }

  return output;
}
