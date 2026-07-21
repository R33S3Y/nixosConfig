#include "nixGet.h"
#include "./utils/systemHelper.h"
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using namespace std;

vector<string> nixGet::flakeHosts(string flakePath) {
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
