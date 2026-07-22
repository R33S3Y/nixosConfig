#include "nixGet.h"
#include "systemHelper.h"
#include "ttyHelper.h"
#include <iostream>
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
string nixGet::futureDerivationPath(string flakePath, string host) {
  string cmd = "nix eval " + flakePath + "#nixosConfigurations." + host +
               ".config.system.build.toplevel.drvPath --raw";
  systemHelper::result cmdOut = systemHelper::runCommand(cmd);

  if (cmdOut.exitCode != 0) {
    cerr << ttyHelper::error("unable to build to derivation\n" + cmdOut.error);
    return "";
  }

  return cmdOut.output;
}
string nixGet::currentDerivationPath() {
  string cmd = "nix-store -q --deriver /run/current-system";
  systemHelper::result cmdOut = systemHelper::runCommand(cmd);

  if (cmdOut.exitCode != 0) {
    cerr << ttyHelper::error("unable to build to derivation\n" + cmdOut.error);
    return "";
  }

  return cmdOut.output;
}
