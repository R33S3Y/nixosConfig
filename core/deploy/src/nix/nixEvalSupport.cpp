#include "nixEvalSupport.h"
#include "../utils/strings.h"
#include "../utils/systemHelper.h"
#include "../utils/threading.h"
#include "../utils/ttyHelper.h"
#include "staticGet.h"
#include <iostream>
#include <map>

map<string, map<string, nixEvalSupport::key>>
nixEvalSupport::seniorInitWorker(map<string, nixEvalSupport::key> input) {

  map<string, map<string, nixEvalSupport::key>> hold;
  for (auto &[key, value] : input) {
    hold.insert({key, {{key, value}}});
  }

  return threading::paralleliseMap<string, map<string, nixEvalSupport::key>,
                                   map<string, nixEvalSupport::key>,
                                   decltype(&nixEvalSupport::juniorInitWorker)>(
      hold, nixEvalSupport::juniorInitWorker);
}
map<string, nixEvalSupport::key>
nixEvalSupport::juniorInitWorker(map<string, nixEvalSupport::key> input) {
  string key = input.begin()->first;
  nixEvalSupport::key value = input[key];

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

  map<string, nixEvalSupport::key> output;
  vector<string> list = staticGet::listItems(cmdOut.output);
  for (int i = 0; i < list.size(); i++) {
    list[i] = strings::replaceAll(list[i], "\"", "");
    if (list[i] == "inputs")
      continue;
    output.insert({list[i], {value.start + key + ".", value.end}});
  }

  return output;
}
