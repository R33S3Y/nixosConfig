#include "args.h";
#include "split.h"
#include "strings.h"
#include <exception>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

map<string, args::optionOut>
args::parse(vector<string> userInput, map<string, args::optionIn> argValues) {
  userInput.erase(userInput.begin());

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
      lastArgToken = i + 1;
      break;
    }
  }

  cout << "lastArgToken: ";
  cout << lastArgToken;
  cout << "\n";

  for (int i = 0; i < lastArgToken; i++) {
    string token = userInput[i];

    cout << token + "\n";

    if (token.starts_with("-") == false) {
      continue;
    }

    // is longArg
    bool longArg = true;
    if (token.starts_with("--") == false) {
      longArg = false;
    }

    // find str
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

    // make value
    args::optionOut invokedOutput = {
        .longName = argInfo.longName,
        .invoked = true,
        .shortName = argInfo.shortName,
    };

    // get value
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
      cout << "hit!";
      userInput.erase(userInput.begin() + i + 1);
      lastArgToken--;
    }

    // delete old stuff
    if (longArg == false) {
      token = token.substr(1);
      userInput[i] = token;
      if (token.size() != 0) {
        i--;
      }
    }
    if (longArg == true || token.size() == 0) {
      cout << "hit!!";
      userInput.erase(userInput.begin() + i);
      lastArgToken--;
    }

    cout << "lastArgToken: ";
    cout << lastArgToken;
    cout << "\n";

    output[argName] = invokedOutput;
  }

  userInput.erase(userInput.end() - 1);

  string remainder;
  for (string str : userInput) {
    remainder += " " + str;
  }
  cout << "\nremainder: " + remainder + "\n";
  cout << "found Flags: \n";
  for (auto &[name, value] : output) {
    cout << "  --" + name + "\n";
  }
  cout << "\n";

  // add uninvoked args
  for (auto &[name, value] : argValues) {
    if (output.find(name) == output.end()) {
      continue;
    }
    output[name] = {
        .longName = value.longName,
        .invoked = false,
        .shortName = value.shortName,
    };
  }

  return output;
}
