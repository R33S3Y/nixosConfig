
#include "dynamic.h"

#include "utils/split.h"
#include "utils/strings.h"
#include "utils/systemHelper.h"
#include "utils/ttyHelper.h"
#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using namespace std;

vector<string> getFlakeInputs(string flakeLink) {
  string cmd = "nix flake show " + flakeLink + " --json";
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
int main(int argc, char const *argv[]) {

  string flakeLink = "/home/reese/Projects/nixosConfig";
  string flakePath = "/tmp/currentConfig";
  bool dynamicBuild = true;

  filesystem::create_directories(flakePath);
  if (filesystem::is_empty(flakePath) == false) {
    cerr << ttyHelper::error("flakePath (\033[35m" + flakePath +
                             "\033[0m) is not empty");
    return 1;
  }
  systemHelper::result cmdOut;
  cmdOut = systemHelper::runCommand("nix flake clone " + flakeLink +
                                    " --dest " + flakePath);
  if (cmdOut.exitCode != 0) {
    cerr << ttyHelper::error("failed to get flake (\033[35m" + flakeLink +
                             "\033[0m)");
    return 1;
  }

  vector<string> hosts = getFlakeInputs(flakePath);
  if (hosts.size() == 0) {
    cerr << ttyHelper::error("flake does not contain any hosts or no "
                             "hosts could be found");
    return 1;
  }

  cmdOut =
      systemHelper::runCommand("git -C " + flakePath + " diff --name-only");
  if (cmdOut.exitCode != 0) {
    cerr << ttyHelper::error("git diff failed");
    return 1;
  }
  cout << "cout: \"" + cmdOut.output + "\"\n";
  cout << "cerr: \"" + cmdOut.error + "\"\n";

  vector<string> gitDiff = split::splitStrByChar(cmdOut.output, '\n');

  for (vector<string>::iterator it = gitDiff.begin(); it != gitDiff.end();
       it++) {
    *it = strings::trim(*it);

    if (it->size() == 0) {
      gitDiff.erase(it);
      it--;
    }
  }
  if (gitDiff.size() == 0) {
    dynamicBuild = false;
    cerr << ttyHelper::error(
        "no changed items found. Skipping dynamic rebuild");
  }

  vector<bool> rebuild;
  for (string host : hosts) {
    if (dynamicBuild == false) {
      rebuild.push_back(true);
      continue;
    }
    cout << host + "\n";
    rebuild.push_back(dynamic::rebuild(host, flakePath, flakeLink, gitDiff));
    cout << rebuild.back();
  }

  filesystem::remove_all(flakePath);

  return 0;
}
