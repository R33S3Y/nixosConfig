#pragma once

#include <map>
#include <optional>
#include <string>
#include <vector>
using namespace std;

class args {
public:
  struct optionIn {
    string longName;
    optional<string> shortName;
    bool takesValue = false;
  };
  struct optionOut {
    string longName;
    bool invoked;
    optional<string> shortName;
    optional<string> value;
  };
  map<string, optionOut> parse(vector<string> userInput,
                               map<string, optionIn> argValues);
};
